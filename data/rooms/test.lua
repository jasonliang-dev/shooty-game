local mat = require "mat"
local quat = require "quat"
local keyboard = require "keyboard"
local mouse = require "mouse"
local class = require "class"
local ray = require "ray"
local Player = require "entities.player"
local Plant = require "entities.plant"
local BasicEnemy = require "entities.basic_enemy"
local FollowEnemy = require "entities.follow_enemy"
local FlyingEnemy = require "entities.flying_enemy"
local SpinSpikeEnemy = require "entities.spin_spike_enemy"
local FishEnemy = require "entities.fish_enemy"
local EntityGroup = require "entity_group"
local Camera = require "camera"

local RoomTest = class()

function RoomTest:new()
    local spawn = tmx_lv1:object_by_type "spawn"

    self.camera = Camera(spawn.x, 0, spawn.y, quat.euler(math.pi / 6, 0, 0))

    self.group = EntityGroup()
    -- map_test:populate_entity_group(self.group)

    self.group:add(BasicEnemy, {
        map = tmx_lv1,
        x = 20,
        z = 20,
        dx = -1,
        dz = 0,
        health = 3,
    })

    self.group:add(FollowEnemy, {
        map = tmx_lv1,
        x = 20,
        z = 20,
        health = 3,
    })

    self.group:add(FlyingEnemy, {
        x = 10,
        z = 10,
        attack_time = 1.2,
        health = 2,
    })

    self.group:add(SpinSpikeEnemy, {
        origin_x = 14,
        origin_z = 25,
        radius = 3,
    })

    self.group:add(SpinSpikeEnemy, {
        origin_x = 11,
        origin_z = 32,
        radius = 3,
    })

    self.group:add(SpinSpikeEnemy, {
        origin_x = 32,
        origin_z = 32,
        radius = 2,
    })

    self.group:add(FishEnemy, {
        x = 10,
        z = 19,
        health = 3,
    })

    self.group:add(FishEnemy, {
        x = 26,
        z = 23,
        health = 3,
    })

    self.group:add(FishEnemy, {
        x = 8,
        z = 25,
        health = 3,
    })

    self.player = self.group:add(Player, {
        map = tmx_lv1,
        camera = self.camera,
        x = spawn.x,
        z = spawn.y,
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
