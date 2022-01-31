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
end

function RoomTest:draw()
    local view_projection = self.camera:view_projection()
    gfx.bind_mvp(view_projection)

    local mvp = mat.mul(mat.rotate(quat.euler(math.pi / 2, 0, 0)), view_projection)
    tmx_lv1:draw(mvp, tsx_tiles.texture_id)
    self.group:draw()
end

return RoomTest
