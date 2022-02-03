local class = require "class"
local vec2 = require "vec".vec2
local lume = require "deps.lume"
local Sprite = require "sprite"
local StateMachine = require "state_machine"
local Spring = require "spring"

local FlyingEnemy = class()
FlyingEnemy.classname = "FlyingEnemy"

function FlyingEnemy:new(desc)
    self.y_init = 2
    self.x = desc.x
    self.y = self.y_init
    self.z = desc.z
    self.begin_x = 0
    self.begin_z = 0
    self.target_x = 0
    self.target_z = 0
    self.attack_time = 0
    self.attack_time_init = desc.attack_time
    self.shadow = atl_entities:uv "circle_small"
    self.spring = Spring()

    self.sprite = Sprite {
        atlas = atl_entities,
        initial = "fly",
        animations = {
            fly = {ms_per_frame = 150, frames = {"bat_1", "bat_2", "bat_3"}},
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
        dx, dz = vec2.normalize(dx, dz)
        self.x = self.x + dx * 3 * dt
        self.z = self.z + dz * 3 * dt
    end
end

function FlyingEnemy:fsm_enter_attack()
    self.sprite:play "attack"

    local player = self.group.player
    self.begin_x = self.x
    self.begin_z = self.z
    self.target_x = self.x + (player.x - self.x) * 2
    self.target_z = self.z + (player.z - self.z) * 2
    self.attack_time = self.attack_time_init
end

function FlyingEnemy:fsm_attack(dt)
    self.attack_time = self.attack_time - dt
    if self.attack_time > 0 then
        self.x = lume.lerp(self.target_x, self.begin_x, self.attack_time / self.attack_time_init)
        self.z = lume.lerp(self.target_z, self.begin_z, self.attack_time / self.attack_time_init)

        local ay = lume.lerp(0, math.pi, self.attack_time / self.attack_time_init)
        self.y = (1 - math.sin(ay)) * self.y_init
    else
        self.fsm:transition "fly"
    end
end

function FlyingEnemy:draw()
    local x1 = self.x - 0.5
    local y1 = self.y + 1.2
    local z1 = self.z
    local x2 = self.x + 0.5
    local y2 = self.y
    local z2 = self.z

    local uv = self.sprite:uv()
    local u1 = self.dx < 0 and uv.u1 or uv.u2
    local v1 = uv.v1
    local u2 = self.dx < 0 and uv.u2 or uv.u1
    local v2 = uv.v2

    gfx.bind_texture(self.sprite.atlas.texture.id)
    gfx.v3_t2(x1, y1, z1, u1, v1)
    gfx.v3_t2(x1, y2, z2, u1, v2)
    gfx.v3_t2(x2, y2, z2, u2, v2)
    gfx.v3_t2(x2, y1, z1, u2, v1)

    x1 = self.x - 0.4
    z1 = self.z - 0.4
    x2 = self.x + 0.4
    z2 = self.z + 0.4
    uv = self.shadow

    gfx.v3_t2_c4(x1, 0.01, z1, uv.u1, uv.v1, 240, 240, 255, 32)
    gfx.v3_t2_c4(x1, 0.01, z2, uv.u1, uv.v2, 240, 240, 255, 32)
    gfx.v3_t2_c4(x2, 0.01, z2, uv.u2, uv.v2, 240, 240, 255, 32)
    gfx.v3_t2_c4(x2, 0.01, z1, uv.u2, uv.v1, 240, 240, 255, 32)
end

return FlyingEnemy
