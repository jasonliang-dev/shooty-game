local mat = require "mat"
local quat = require "quat"
local keyboard = require "keyboard"
local class = require "class"
local Player = require "entities.player"
local BasicEnemy = require "entities.basic_enemy"
local FollowEnemy = require "entities.follow_enemy"
local FlyingEnemy = require "entities.flying_enemy"
local SpinSpikeEnemy = require "entities.spin_spike_enemy"
local FishEnemy = require "entities.fish_enemy"
local BlockEnemy = require "entities.block_enemy"
local Crosshair = require "entities.crosshair"
local EntityGroup = require "entity_group"
local Camera = require "camera"

local core = _G["core"]

local RoomTest = class()

function RoomTest:new()
    core.sys.show_mouse(false)
    DEFAULT_PASS_CLEAR_COLOR = {128, 128, 128, 255}

    local spawn = TMX_LV1:object_by_type "spawn"

    self.camera = Camera(spawn.x, 0, spawn.y, quat.euler(math.pi / 6, 0, 0))

    self.group = EntityGroup()
    -- map_test:populate_entity_group(self.group)

    self.group:add(BasicEnemy, {
        map = TMX_LV1,
        x = 20,
        z = 20,
        dx = -1,
        dz = 0,
        health = 3,
    })

    self.group:add(FollowEnemy, {
        map = TMX_LV1,
        x = 20,
        z = 20,
        health = 3,
    })

    self.group:add(FlyingEnemy, {
        x = 10,
        z = 10,
        attack_time = 1.2,
        health = 3,
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

    self.group:add(BlockEnemy, {
        x = 29,
        z = 27,
        health = 5,
    })

    self.player = self.group:add(Player, {
        map = TMX_LV1,
        camera = self.camera,
        x = spawn.x,
        z = spawn.y,
        health = 5,
    })

    self.group:add(Crosshair, {
        camera = self.camera,
    })
end

function RoomTest:update(dt)
    if keyboard.pressed "escape" then
        -- self.rooms.go "menu"
    end

    self.group:update(dt)

    self.camera.target = {
        x = self.player.x,
        y = 6,
        z = self.player.z + 10
    }
    self.camera:update(dt)
end

function RoomTest:draw()
    local view_projection = self.camera:view_projection()

    local water_width = 160
    local mvp = mat.chain(
       view_projection,
       mat.translate(-water_width / 2, -0.01, -water_width / 2),
       mat.scale(water_width, 1, water_width),
       mat.axis_rotate(1, 0, 0, math.pi / 2)
    )
    core.gfx.draw_water(mvp, water_width)

    mvp = mat.mul(mat.axis_rotate(1, 0, 0, math.pi / 2), view_projection)
    TMX_LV1:draw(mvp, TSX_TILES.texture_id)

    core.gfx.bind_mvp(view_projection)
    self.group:draw()

    core.gfx.bind_mvp(mat.ortho(0, core.sys.window_width(), core.sys.window_height(), 0, -1, 1))
    self.player:draw_hp_bar(32, 16)
end

return RoomTest
