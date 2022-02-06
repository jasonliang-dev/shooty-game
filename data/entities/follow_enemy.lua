local class = require "class"
local Sprite = require "sprite"
local vec2 = require "vec".vec2
local common = require "enemy_common"

local FollowEnemy = class()
FollowEnemy.classname = "FollowEnemy"

function FollowEnemy:new(desc)
    common.init(self, desc)

    self.map = desc.map

    self.x = desc.x
    self.y = 0
    self.z = desc.z
    self.speed = 1.5
    self.dx = -1

    self.sprite = Sprite {
        atlas = atl_entities,
        initial = "walk",
        animations = {
            walk = {ms_per_frame = 150, frames = {"robot2_1", "robot2_2"}}
        }
    }
end

function FollowEnemy:update(dt)
    common.update(self, {
        dt = dt,
        collision_distance = 0.9,
    })
    self.sprite:update(dt)

    local player = self.group.player
    local path = self.map:a_star(self.x, self.z, player.x, player.z)
    self.path = path
    if path and #path > 1 then
        local dx = path[2].x + 0.5 - self.x
        local dz = path[2].y + 0.5 - self.z
        dx, dz = vec2.normalize(dx, dz)
        self.x, self.z = self.map:point_move(self.x, self.z, dx * self.speed * dt, dz * self.speed * dt, 4)
        self.dx = dx
    end
end

function FollowEnemy:on_death()
    common.on_death(self)
end

function FollowEnemy:draw()
    common.draw(self)
end

return FollowEnemy
