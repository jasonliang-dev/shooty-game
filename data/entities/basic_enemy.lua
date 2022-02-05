local class = require "class"
local Sprite = require "sprite"
local Spring = require "spring"
local Progress = require "progress"
local common = require "enemy_common"

local BasicEnemy = class()
BasicEnemy.classname = "BasicEnemy"

function BasicEnemy:new(desc)
    common.init(self, desc)

    self.map = desc.map

    self.x = desc.x
    self.y = 0
    self.z = desc.z
    self.dx = desc.dx
    self.dz = desc.dz

    self.sprite = Sprite {
        atlas = atl_entities,
        initial = "walk",
        animations = {
            walk = {ms_per_frame = 150, frames = {"robot1_1", "robot1_2"}}
        }
    }
end

function BasicEnemy:update(dt)
    common.update(self, {
        dt = dt,
        collision_radius = 0.9,
    })
    self.sprite:update(dt)

    local collided = false
    self.x, self.z, collided = self.map:point_move(self.x, self.z, self.dx * 2 * dt, self.dz * 2 * dt, 4)
    if collided then
        self.dx = -self.dx
        self.dz = -self.dz
    end
end

function BasicEnemy:on_death()
    common.on_death(self)
end

function BasicEnemy:draw()
    common.draw(self)
end

return BasicEnemy
