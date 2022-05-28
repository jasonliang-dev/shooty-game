local class = require "class"
local enemy_common = require "enemy_common"
local lume = require "deps.lume"
local Sprite = require "sprite"
local Progress = require "progress"
local StateMachine = require "state_machine"

local BlockEnemy = class()
BlockEnemy.classname = "BlockEnemy"

function BlockEnemy:new(desc)
    enemy_common.init(self, desc)

    self.x = desc.x
    self.z = desc.z
    self.y_init = 2
    self.y = self.y_init
    self.dy = 0
    self.p_attack = Progress(0.6, true)
    self.p_rest = Progress(1.2, true)

    self.sprite = Sprite {
        atlas = ATL_ENTITIES,
        initial = "idle",
        animations = {
            idle = {ms_per_frame = 150, frames = {"block_1"}},
            attack = {ms_per_frame = 150, frames = {"block_2"}},
        }
    }

    self.fsm = StateMachine {
        owner = self,
        initial = "idle",
        states = {
            idle = {enter = self.fsm_enter_idle, update = self.fsm_idle},
            attack = {enter = self.fsm_enter_attack, update = self.fsm_attack},
            rest = {enter = self.fsm_enter_rest, update = self.fsm_rest},
        }
    }
end

function BlockEnemy:update(dt)
    self.sprite:update(dt)
    self.fsm:update(dt)

    self.spring:update(dt)

    local bullets = self.group:nearby_classname("Bullet", self.x, self.y, self.z, 0.5)
    for _, bullet in pairs(bullets) do
        bullet.dead = true
    end

    enemy_common.player_collision(self, 0.8)
end

function BlockEnemy:fsm_enter_idle()
    self.sprite:play "idle"
    self.y = self.y_init
end

function BlockEnemy:fsm_idle(dt)
    local player = self.group.player
    local dist = lume.distance(player.x, player.z, self.x, self.z)
    if dist <= 2 then
        self.fsm:transition "attack"
    end
end

function BlockEnemy:fsm_enter_attack()
    self.p_attack.time = 0
    self.sprite:play "attack"
end

function BlockEnemy:fsm_attack(dt)
    self.p_attack:update(dt)
    self.y = (1 - self.p_attack:percent()) * self.y_init
    if self.y <= 0 then
        self.spring:pull(0.2)
        self.fsm:transition "rest"
    end
end

function BlockEnemy:fsm_enter_rest()
    self.p_rest.time = 0
    self.sprite:play "idle"
end

function BlockEnemy:fsm_rest(dt)
    self.p_rest:update(dt)
    self.y = self.p_rest:percent() * self.y_init
    if self.y >= self.y_init then
        self.fsm:transition "idle"
    end
end

function BlockEnemy:on_death()
    enemy_common.on_death(self)
end

function BlockEnemy:draw()
    enemy_common.draw(self)
end

return BlockEnemy
