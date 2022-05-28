local class = require "class"
local Sprite = require "sprite"
local enemy_common = require "enemy_common"

local BasicEnemy = class()
BasicEnemy.classname = "BasicEnemy"

function BasicEnemy:new(desc)
    enemy_common.init(self, desc)

    self.map = desc.map

    self.x = desc.x
    self.y = 0
    self.z = desc.z
    self.dx = desc.dx
    self.dz = desc.dz

    self.sprite = Sprite {
        atlas = ATL_ENTITIES,
        initial = "walk",
        animations = {
            walk = {ms_per_frame = 150, frames = {"robot1_1", "robot1_2"}}
        }
    }
end

function BasicEnemy:update(dt)
    enemy_common.update(self, {
        dt = dt,
        collision_distance = 0.8,
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
    enemy_common.on_death(self)
end

function BasicEnemy:draw()
    enemy_common.draw(self)
end

return BasicEnemy
