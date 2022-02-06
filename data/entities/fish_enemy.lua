local class = require "class"
local common = require "enemy_common"
local lume = require "deps.lume"
local Sprite = require "sprite"

local FishEnemy = class()

function FishEnemy:new(desc)
    common.init(self, desc)

    self.x = desc.x
    self.y = 1 / 0
    self.z = desc.z
    self.dx = 0
    self.dy = 0
    self.time_y = math.random() * math.pi

    self.sprite = Sprite {
        atlas = atl_entities,
        initial = "bite",
        animations = {
            bite = {ms_per_frame = 150, frames = {"fish_1", "fish_2"}},
        }
    }
end

function FishEnemy:update(dt)
    self.sprite:update(dt)

    self.time_y = self.time_y + dt
    local y = self.y
    self.y = math.cos(self.time_y * 4)

    common.update(self, {
        dt = dt,
        collision_distance = 0.8,
    })

    local dy = self.y - y
    if self.dy > dy and lume.sign(dy) ~= lume.sign(self.dy) then

    end
    self.dy = dy

end

function FishEnemy:on_death()
    common.on_death(self)
end

function FishEnemy:draw()
    common.draw(self)
end

return FishEnemy
