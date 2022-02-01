local mat = require "mat"
local quat = require "quat"
local keyboard = require "keyboard"
local mouse = require "mouse"
local class = require "class"
local vec3 = require "vec".vec3
local ray = require "ray"
local Player = require "entities.player"
local Plant = require "entities.plant"
local BasicEnemy = require "entities.basic_enemy"
local EntityGroup = require "entity_group"
local Camera = require "camera"

local RoomTest = class()

function RoomTest:new()
    local spawn = tmx_lv1:object_by_type "spawn"

    self.camera = Camera(spawn.x, 0, spawn.y, quat.euler(math.pi / 6, 0, 0))

    self.group = EntityGroup()
    -- map_test:populate_entity_group(self.group)

    self.group:add(BasicEnemy, {
        x = spawn.x,
        z = spawn.y,
        dx = -1,
        dz = 0,
        map = tmx_lv1,
    })

    self.player = self.group:add(Player, {
        x = spawn.x,
        z = spawn.y,
        camera = self.camera,
        map = tmx_lv1,
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
        self.cast_point = {
             x = math.floor(raycast.point.x),
             z = math.floor(raycast.point.z),
        }

        self.path = tmx_lv1:a_star(self.player.x, self.player.z, raycast.point.x, raycast.point.z)
    else
        self.cast_point = nil
        self.path = nil
    end
end

function RoomTest:draw()
    local view_projection = self.camera:view_projection()
    gfx.bind_mvp(view_projection)

    local mvp = mat.mul(mat.rotate(quat.euler(math.pi / 2, 0, 0)), view_projection)
    tmx_lv1:draw(mvp, tsx_tiles.texture_id)
    self.group:draw()

    if self.cast_point then
        gfx.bind_white_texture()
        local x1 = self.cast_point.x
        local z1 = self.cast_point.z
        local x2 = self.cast_point.x + 1
        local z2 = self.cast_point.z + 1
        gfx.v3_t2_c4(x1, 0.001, z1, 0, 0, 255, 255, 0, 128)
        gfx.v3_t2_c4(x1, 0.001, z2, 0, 0, 255, 255, 0, 128)
        gfx.v3_t2_c4(x2, 0.001, z2, 0, 0, 255, 255, 0, 128)
        gfx.v3_t2_c4(x2, 0.001, z1, 0, 0, 255, 255, 0, 128)
    end

    if self.path then
        gfx.bind_white_texture()
        for _, tile in ipairs(self.path) do
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
