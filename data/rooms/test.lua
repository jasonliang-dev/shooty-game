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
end

function RoomTest:draw()
    gfx.bind_mvp(self.camera:view_projection())

    map_test:draw()
    self.group:draw()
end

return RoomTest
