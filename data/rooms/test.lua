local mat = require "mat"
local keyboard = require "keyboard"
local mouse = require "mouse"
local class = require "class"
local lume = require "deps.lume"
local vec3 = require "vec".vec3
local Player = require "entities.player"
local Plant = require "entities.plant"
local Ray = require "entities.ray"
local EntityGroup = require "entity_group"
local Camera = require "camera"

local RoomTest = class()

function RoomTest:new()
    self.group = EntityGroup()
    self.player = self.group:add(Player, 3, 3)
    self.group:add(Plant, 5, 4, "tile_0124")
    self.group:add(Plant, 8, 7, "tile_0124")
    self.group:add(Ray, {
        origin = {x = -1, y = 1, z = -1},
        direction = {x = 0, y = -1, z = 0},
    })

    self.camera = Camera(self.player.x, 0, self.player.z, mat.rotate(1, 0, 0, math.pi / 6))
end

function RoomTest:update(dt)
    self.group:update(dt)
    self.camera:set_target(self.player.x + 0.5, 8, self.player.z + 13)
    self.camera:update(dt)

    if keyboard.down "n" then
        self.rooms.go "test2"
    end
end

function RoomTest:draw()
    local projection = mat.perspective(math.pi / 4, sys.window_width() / sys.window_height(), 0.05, 1000)
    gfx.bind_mvp(mat.mul(self.camera:mat_view(), projection))

    map_test:draw()
    self.group:draw()

    gfx.bind_mvp(mat.ortho(0, sys.window_width(), sys.window_height(), 0, -1, 1))
    local x1 = mouse.x - 16
    local y1 = mouse.y - 16
    local x2 = mouse.x + 16
    local y2 = mouse.y + 16
    local uv = atl_atlas:uv "tile_0027"
    gfx.bind_texture(atl_atlas.texture.id)
    gfx.v3_t2(x1, y1, 0, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, 0, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, 0, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, 0, uv.u2, uv.v1)
end

return RoomTest
