local json = require "deps.json"
local class = require "class"

local Tilemap = class()

local function count_tiles(data)

    return sum
end

function Tilemap:new(filename)
    local handle = assert(io.open(filename))
    local export = json.decode(handle:read "*a")

    assert(#export.tilesets == 1)
    local tileset = export.tilesets[1]

    local path = filename:match "(.*/)"
    local image = tileset.image
    self.texture = gfx.make_texture(path .. image)

    for _, layer in ipairs(export.layers) do
        if layer.type ~= "tilelayer" then
            goto next_layer
        end

        local len = 0
        for _, cell in pairs(layer.data) do
            if cell ~= 0 then
                len = len + 1
            end
        end

        assert(self.vertices == nil)
        self.vertices = gfx.make_vertex_array(len * 4)
        self.vertex_count = 0

        for y = 1, export.height do
            for x = 1, export.width do
                local index = layer.data[((y - 1) * export.width) + x]
                if index == 0 then
                    goto next_tile
                end

                -- unset flags?
                local id = index - tileset.firstgid

                local atlas_x = id % tileset.columns
                local atlas_y = id // tileset.columns

                local inset = 0.002

                local x1 = x
                local y1 = y
                local x2 = x + 1
                local y2 = y + 1

                local u1 = inset + (tileset.margin + (atlas_x * tileset.spacing) + (atlas_x * export.tilewidth)) / tileset.imagewidth
                local v1 = inset + (tileset.margin + (atlas_y * tileset.spacing) + (atlas_y * export.tileheight)) / tileset.imageheight
                local u2 = -inset + (tileset.margin + (atlas_x * tileset.spacing) + ((atlas_x + 1) * export.tilewidth)) / tileset.imagewidth
                local v2 = -inset + (tileset.margin + (atlas_y * tileset.spacing) + ((atlas_y + 1) * export.tileheight)) / tileset.imageheight

                self.vertices:write_at(self.vertex_count + 1, x1, 0, y1, u1, v1)
                self.vertices:write_at(self.vertex_count + 2, x1, 0, y2, u1, v2)
                self.vertices:write_at(self.vertex_count + 3, x2, 0, y2, u2, v2)
                self.vertices:write_at(self.vertex_count + 4, x2, 0, y1, u2, v1)
                self.vertex_count = self.vertex_count + 4

                ::next_tile::
            end
        end

        ::next_layer::
    end

    self.width = export.width
    self.height = export.height

    handle:close()
end

function Tilemap:draw()
    gfx.bind_texture(self.texture.id)
    self.vertices:draw(self.vertex_count)
end

return Tilemap
