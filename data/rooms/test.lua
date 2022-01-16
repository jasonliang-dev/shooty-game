local mat = require "mat"
local keyboard = require "keyboard"
local class = require "class"
local lume = require "deps.lume"
local vec3 = require "vec".vec3
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
end

return RoomTest
