local class = require "class"
local Sprite = require "sprite"

local BasicEnemy = class()
BasicEnemy.classname = "BasicEnemy"

function BasicEnemy:new(desc)
    self.map = desc.map

    self.x = desc.x
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
    self.sprite:update(dt)
    local collided = false
    self.x, self.z, collided = self.map:point_move(self.x, self.z, self.dx * 2 * dt, self.dz * 2 * dt, 4)
    if collided then
        self.dx = -self.dx
        self.dz = -self.dz
    end
end

function BasicEnemy:draw()
    local x1 = self.x - 0.5
    local y1 = 1.2
    local z1 = self.z
    local x2 = self.x + 0.5
    local y2 = 0
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
end

return BasicEnemy
