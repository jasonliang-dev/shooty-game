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
    -- local spawn = map_test:object_by_name "spawn"
    -- local enemy = map_test:object_by_name "enemy"

    self.camera = Camera(5, 0, 5, quat.euler(math.pi / 6, 0, 0))

    self.group = EntityGroup()
    -- map_test:populate_entity_group(self.group)

    self.group:add(BasicEnemy, {
        x = 0,
        z = 0,
        dx = -1,
        dz = 0,
        -- map = map_test,
    })

    self.player = self.group:add(Player, {
        x = 5,
        z = 5,
        camera = self.camera,
        -- map = map_test,
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
    map_test1:draw(mvp, atl_tiles.texture.id)
    self.group:draw()
end

return RoomTest
