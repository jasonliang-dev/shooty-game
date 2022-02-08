local class = require "class"
local vec2 = require "vec".vec2
local lume = require "deps.lume"
local Sprite = require "sprite"
local StateMachine = require "state_machine"
local Progress = require "progress"
local enemy_common = require "enemy_common"

local FlyingEnemy = class()
FlyingEnemy.classname = "FlyingEnemy"

function FlyingEnemy:new(desc)
    enemy_common.init(self, desc)

    self.y_init = 1.5
    self.x = desc.x
    self.y = self.y_init
    self.z = desc.z
    self.begin_x = 0
    self.begin_z = 0
    self.target_x = 0
    self.target_z = 0
    self.p_attack = Progress(desc.attack_time)
    self.shadow = atl_entities:uv "circle_small"

    self.sprite = Sprite {
        atlas = atl_entities,
        initial = "fly",
        animations = {
            fly = {ms_per_frame = 150, frames = {"bat_1", "bat_2", "bat_3", "bat_2"}},
            attack = {ms_per_frame = 150, frames = {"bat_2"}},
        }
    }

    self.fsm = StateMachine {
        owner = self,
        initial = "fly",
        states = {
            fly = {enter = self.fsm_enter_fly, update = self.fsm_fly},
            attack = {enter = self.fsm_enter_attack, update = self.fsm_attack},
        }
    }
end

function FlyingEnemy:update(dt)
    enemy_common.update(self, {
        dt = dt,
        collision_distance = 0.9,
    })
    self.sprite:update(dt)
    self.fsm:update(dt)
end

function FlyingEnemy:fsm_enter_fly()
    self.sprite:play "fly"
    self.y = self.y_init
end

function FlyingEnemy:fsm_fly(dt)
    local player = self.group.player

    local dx = player.x - self.x
    local dz = player.z - self.z
    self.dx = dx

    if math.sqrt(dx * dx + dz * dz) < 3 then
        self.fsm:transition "attack"
    else
        local fly_speed = 2
        dx, dz = vec2.normalize(dx, dz)
        self.x = self.x + dx * fly_speed * dt
        self.z = self.z + dz * fly_speed * dt
    end
end

function FlyingEnemy:fsm_enter_attack()
    self.sprite:play "attack"

    local player = self.group.player
    self.begin_x = self.x
    self.begin_z = self.z
    self.target_x = self.x + (player.x - self.x) * 2
    self.target_z = self.z + (player.z - self.z) * 2
    self.p_attack.time = 0

    snd.play "data/content/woosh.wav"
end

function FlyingEnemy:fsm_attack(dt)
    self.p_attack:update(dt)
    local perc = self.p_attack:percent()
    if perc < 1 then
        self.x = lume.smooth(self.begin_x, self.target_x, perc)
        self.z = lume.smooth(self.begin_z, self.target_z, perc)

        local ay = lume.smooth(0, math.pi, perc)
        self.y = (1 - math.sin(ay)) * self.y_init
    else
        self.fsm:transition "fly"
    end
end

function FlyingEnemy:on_death()
    enemy_common.on_death(self)
end

function FlyingEnemy:draw()
    enemy_common.draw(self)

    local x1 = self.x - 0.4
    local z1 = self.z - 0.4
    local x2 = self.x + 0.4
    local z2 = self.z + 0.4
    local uv = self.shadow

    gfx.v3_t2_c4(x1, 0.01, z1, uv.u1, uv.v1, 240, 240, 255, 32)
    gfx.v3_t2_c4(x1, 0.01, z2, uv.u1, uv.v2, 240, 240, 255, 32)
    gfx.v3_t2_c4(x2, 0.01, z2, uv.u2, uv.v2, 240, 240, 255, 32)
    gfx.v3_t2_c4(x2, 0.01, z1, uv.u2, uv.v1, 240, 240, 255, 32)
end

return FlyingEnemy
