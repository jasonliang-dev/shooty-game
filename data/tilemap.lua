local json = require "deps.json"
local class = require "class"
local Plant = require "entities.plant"
local PriorityQueue = require "priority_queue"

-- top left of tilemap is at 0,0 even though arrays start at 1

local Tilemap = class()

Tilemap.collision_table = {
    [6] = "full",
    [1] = "out top left",
    [2] = "out top",
    [3] = "out top right",
    [13] = "out left",
    [15] = "out right",
    [25] = "out bottom left",
    [26] = "out bottom",
    [27] = "out bottom right",
    [4] = "in bottom right",
    [5] = "in bottom left",
    [16] = "in top right",
    [17] = "in top left",
}

local function create_tiles(self, export, layer, tileset)
    local len = 0
    for _, cell in pairs(layer.data) do
        if cell ~= 0 then
            len = len + 1
        end
    end

    assert(self.vertices == nil)
    self.vertices = gfx.make_vertex_array(len * 4)
    self.vertex_count = 0

    assert(self.colllision == nil)
    self.collision = {}

    for y = 1, export.height do
        for x = 1, export.width do
            local index = layer.data[(y - 1) * export.width + x]
            if index == 0 then
                goto continue
            end

            -- unset flags?
            local id = index - tileset.firstgid

            local atlas_x = id % tileset.columns
            local atlas_y = id // tileset.columns

            local inset = 0.004

            local x1 = x - 1
            local y1 = y - 1
            local x2 = x
            local y2 = y

            local u1 = inset + (tileset.margin + (atlas_x * tileset.spacing) + (atlas_x * export.tilewidth)) / tileset.imagewidth
            local v1 = inset + (tileset.margin + (atlas_y * tileset.spacing) + (atlas_y * export.tileheight)) / tileset.imageheight
            local u2 = -inset + (tileset.margin + (atlas_x * tileset.spacing) + ((atlas_x + 1) * export.tilewidth)) / tileset.imagewidth
            local v2 = -inset + (tileset.margin + (atlas_y * tileset.spacing) + ((atlas_y + 1) * export.tileheight)) / tileset.imageheight

            self.vertices:write_at(self.vertex_count + 1, x1, 0, y1, u1, v1)
            self.vertices:write_at(self.vertex_count + 2, x1, 0, y2, u1, v2)
            self.vertices:write_at(self.vertex_count + 3, x2, 0, y2, u2, v2)
            self.vertices:write_at(self.vertex_count + 4, x2, 0, y1, u2, v1)
            self.vertex_count = self.vertex_count + 4

            local collision = Tilemap.collision_table[id]
            if collision then
                self.collision[(y - 1) * export.width + x] = collision
            end

            ::continue::
        end
    end
end

local function create_graph(self, export, layer, firstgid)
    self.graph_nodes = {}

    for y = 1, export.height do
        for x = 1, export.width do
            local index = layer.data[(y - 1) * export.width + x]
            if index == 0 then
                goto continue
            end

            local id = index - firstgid
            local collision = Tilemap.collision_table[id]
            if not collision then
                self.graph_nodes[(y - 1) * export.width + x] = {
                    x = x - 1,
                    y = y - 1,
                    f = 0,
                    g = 0,
                    visited = false,
                }
            end

            ::continue::
        end
    end
end

local function push_objects(self, export, objects)
    for _, object in ipairs(objects) do
        table.insert(self.objects, {
            name = object.name,
            type = object.type,
            x = object.x / export.tilewidth,
            y = object.y / export.tileheight,
        })
    end
end

function Tilemap:new(filename)
    local handle = assert(io.open(filename))
    local export = json.decode(handle:read "*a")

    assert(#export.tilesets == 1)
    local tileset = export.tilesets[1]

    local path = filename:match "(.*/)"
    local image = tileset.image
    self.texture = gfx.make_texture(path .. image)
    self.objects = {}

    for _, layer in ipairs(export.layers) do
        if layer.type == "tilelayer" then
            create_tiles(self, export, layer, tileset)
            create_graph(self, export, layer, tileset.firstgid)
        elseif layer.type == "objectgroup" then
            push_objects(self, export, layer.objects)
        end
    end

    self.width = export.width
    self.height = export.height

    handle:close()
end

function Tilemap:object_by_name(name)
    for _, object in ipairs(self.objects) do
        if object.name == name then
            return object
        end
    end
end

function Tilemap:populate_entity_group(group)
    for _, object in ipairs(self.objects) do
        if object.type == "grass" then
            group:add(Plant, object.x, object.y, "grass1")
        elseif object.type == "tree" then
            group:add(Plant, object.x, object.y, "tree")
        end
    end
end

function Tilemap:point_collision(x, y)
    local ix, fx = math.modf(x)
    local iy, fy = math.modf(y)

    local collision = self.collision[iy * self.width + ix + 1]

    if not collision then
        return
    elseif collision == "full" then
        return true
    elseif collision == "out top left" then
        return fx <= 0.5 or fy <= 0.5
    elseif collision == "out top" then
        return fy <= 0.5
    elseif collision == "out top right" then
        return fx > 0.5 or fy <= 0.5
    elseif collision == "out left" then
        return fx <= 0.5
    elseif collision == "out right" then
        return fx > 0.5
    elseif collision == "out bottom left" then
        return fx <= 0.5 or fy > 0.5
    elseif collision == "out bottom" then
        return fy > 0.5
    elseif collision == "out bottom right" then
        return fx > 0.5 or fy > 0.5
    elseif collision == "in top left" then
        return fx <= 0.5 and fy <= 0.5
    elseif collision == "in top right" then
        return fx > 0.5 and fy <= 0.5
    elseif collision == "in bottom left" then
        return fx <= 0.5 and fy > 0.5
    elseif collision == "in bottom right" then
        return fx > 0.5 and fy > 0.5
    end

    error "unreachable"
end

local function heuristic(ax, ay, bx, by)
    local dx = bx - ax
    local dy = ay - by
    return math.sqrt(dx * dx + dy * dy)
end

local function tile_neighbors(self, x, y)
    local neighbors = {}

    for yy = y - 1, y + 1 do
        for xx = x - 1, x + 1 do
            local node = self.graph_nodes[yy * self.width + xx + 1]
            if node then
                table.insert(neighbors, node)
            end
        end
    end

    return neighbors
end

function Tilemap:a_star(start_x, start_y, end_x, end_y)
    start_x, start_y = math.floor(start_x), math.floor(start_y)
    end_x, end_y = math.floor(end_x), math.floor(end_y)

    local path = {}

    if start_x == end_x and start_y == end_y then
        table.insert(path, {x = start_x, y = start_y})
        return path
    end

    for _, node in pairs(self.graph_nodes) do
        node.g = 0
        node.f = 1 / 0
        node.parent = nil
        node.visited = false
    end

    local open = PriorityQueue()

    local begin = self.graph_nodes[start_y * self.width + start_x + 1]
    if not begin then return end
    begin.g = 0
    begin.f = heuristic(start_x, start_y, end_x, end_y)
    begin.visited = true

    open:push_min(begin, begin.f)

    while open:count() ~= 0 do
        local top = open:pop_min()

        if top.x == end_x and top.y == end_y then
            while top.parent do
                table.insert(path, {x = top.x, y = top.y})
                top = top.parent
            end
            return path
        end

        local neighbors = tile_neighbors(self, top.x, top.y)
        for _, node in ipairs(neighbors) do
            if node.visited then goto continue end

            node.f = heuristic(node.x, node.y, end_x, end_y)
            node.parent = top
            node.visited = true
            open:push_min(node, node.f)

            ::continue::
        end
    end
end

function Tilemap:point_move(x, y, dx, dy, sub_steps)
    sub_steps = sub_steps or 4
    dx, dy = dx / sub_steps, dy / sub_steps

    local tx, ty
    local collided = false
    for i = 1, sub_steps do
        tx, ty = x, y
        x = x + dx
        if self:point_collision(x, y) then
            x, y = tx, ty
            collided = true
        end

        tx, ty = x, y
        y = y + dy
        if self:point_collision(x, y) then
            x, y = tx, ty
            collided = true
        end
    end

    return x, y, collided
end

function Tilemap:draw()
    gfx.bind_texture(self.texture.id)
    self.vertices:draw(self.vertex_count)
end

return Tilemap
