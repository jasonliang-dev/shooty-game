local mat = require "mat"
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
    self.group = EntityGroup()
    self.player = self.group:add(Player, 3, 3)
    self.group:add(Plant, 5, 4, "tile_0124")
    self.group:add(Plant, 8, 7, "tile_0124")

    self.camera = Camera(self.player.x, 0, self.player.z, mat.axis_rotate(1, 0, 0, math.pi / 6))
end

function RoomTest:update(dt)
    self.group:update(dt)
    self.camera:set_target(self.player.x + 0.5, 8, self.player.z + 13)
    self.camera:update(dt)

    local raycast = ray.vs_quad {
        ray = ray.from_screen(mouse.x, mouse.y, self.camera),
        v1 = {x = 1, y = 0, z = 1},
        v2 = {x = 1, y = 0, z = 25},
        v3 = {x = 25, y = 0, z = 25},
        v4 = {x = 25, y = 0, z = 1},
    }
    if raycast then
        self.cast_point = raycast.point
    else
        self.cast_point = nil
    end

    if keyboard.down "n" then
        self.rooms.go "test2"
    end
end

function RoomTest:draw()
    gfx.bind_mvp(self.camera:view_projection())

    map_test:draw()
    self.group:draw()

    if self.cast_point then
        gfx.bind_white_texture()
        local x1 = self.cast_point.x - 0.1
        local z1 = self.cast_point.z - 0.1
        local x2 = self.cast_point.x + 0.1
        local z2 = self.cast_point.z + 0.1
        gfx.v3_t2_c4_f4(x1, 0.01, z1, 0, 0, 255, 0, 0, 255, 0, 0, 0, 0)
        gfx.v3_t2_c4_f4(x1, 0.01, z2, 0, 0, 255, 0, 0, 255, 0, 0, 0, 0)
        gfx.v3_t2_c4_f4(x2, 0.01, z2, 0, 0, 255, 0, 0, 255, 0, 0, 0, 0)
        gfx.v3_t2_c4_f4(x2, 0.01, z1, 0, 0, 255, 0, 0, 255, 0, 0, 0, 0)
    end

    gfx.bind_mvp(mat.ortho(0, sys.window_width(), sys.window_height(), 0, -1, 1))
    x1 = mouse.x - 16
    y1 = mouse.y - 16
    x2 = mouse.x + 16
    y2 = mouse.y + 16
    local uv = atl_atlas:uv "tile_0027"
    gfx.bind_texture(atl_atlas.texture.id)
    gfx.v3_t2(x1, y1, 0, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, 0, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, 0, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, 0, uv.u2, uv.v1)
end

return RoomTest
