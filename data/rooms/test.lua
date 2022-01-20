local mat = require "mat"
local quat = require "quat"
local keyboard = require "keyboard"
local mouse = require "mouse"
local class = require "class"
local vec3 = require "vec".vec3
local ray = require "ray"
local Player = require "entities.player"
local Plant = require "entities.plant"
local EntityGroup = require "entity_group"
local Camera = require "camera"

local RoomTest = class()

function RoomTest:new()
    local spawn = map_test:object_by_name "spawn"

    self.camera = Camera(spawn.x, 0, spawn.y, quat.euler(math.pi / 6, 0, 0))

    self.group = EntityGroup()
    map_test:populate_entity_group(self.group)
    self.player = self.group:add(Player, {
        x = spawn.x,
        z = spawn.y,
        camera = self.camera,
        map = map_test,
    })
end

function RoomTest:update(dt)
    self.group:update(dt)

    self.camera.target = {
        x = self.player.x,
        y = 8,
        z = self.player.z + 13
    }
    self.camera:update(dt)

    local raycast = ray.vs_quad {
        ray = ray.from_screen(mouse.x, mouse.y, self.camera),
        v1 = {x = -50, y = 0, z = -50},
        v2 = {x = -50, y = 0, z = 50},
        v3 = {x = 50, y = 0, z = 50},
        v4 = {x = 50, y = 0, z = -50},
    }

    if raycast then
        local x1 = self.player.x
        local z1 = self.player.z
        local x2 = raycast.point.x
        local z2 = raycast.point.z
        self.pathfind = map_test:dijkstra(x1, z1, x2, z2)
    else
        self.pathfind = nil
    end
end

function RoomTest:draw()
    gfx.bind_mvp(self.camera:view_projection())

    map_test:draw()
    self.group:draw()

    if self.pathfind then
        gfx.bind_white_texture()
        for _, node in pairs(map_test.graph_nodes) do
            local x1 = node.x
            local z1 = node.y
            local x2 = node.x + 1
            local z2 = node.y + 1
            gfx.v3_t2_c4(x1, 0.001, z1, 0, 0, 0, 255, 0, 64)
            gfx.v3_t2_c4(x1, 0.001, z2, 0, 0, 0, 255, 0, 64)
            gfx.v3_t2_c4(x2, 0.001, z2, 0, 0, 0, 255, 0, 64)
            gfx.v3_t2_c4(x2, 0.001, z1, 0, 0, 0, 255, 0, 64)
        end

        for _, tile in ipairs(self.pathfind) do
            local x1 = tile.x
            local z1 = tile.y
            local x2 = tile.x + 1
            local z2 = tile.y + 1
            gfx.v3_t2_c4(x1, 0.001, z1, 0, 0, 255, 0, 0, 128)
            gfx.v3_t2_c4(x1, 0.001, z2, 0, 0, 255, 0, 0, 128)
            gfx.v3_t2_c4(x2, 0.001, z2, 0, 0, 255, 0, 0, 128)
            gfx.v3_t2_c4(x2, 0.001, z1, 0, 0, 255, 0, 0, 128)
        end
    end
end

return RoomTest
