local class = require "class"
local vec2 = require "vec".vec2

local Bullet = class()

function Bullet:new(x, z, dx, dz)
    self.uv = atl_entities:uv "bullet"
    self.x = x
    self.z = z
    self.dx = dx
    self.dz = dz
    self.lifetime = 5
    self.rot = 0
end

function Bullet:update(dt)
    local spd = dt * 20
    self.x = self.x + self.dx * spd
    self.z = self.z + self.dz * spd
    self.rot = self.rot + 20 * dt

    self.lifetime = self.lifetime - dt
    if self.lifetime <= 0 then
        self.dead = true
    end
end

function Bullet:draw()
    local x1, z1 = vec2.rotate(self.rot, self.x - 0.5, self.z - 0.5, self.x, self.z)
    local x2, z2 = vec2.rotate(self.rot, self.x - 0.5, self.z + 0.5, self.x, self.z)
    local x3, z3 = vec2.rotate(self.rot, self.x + 0.5, self.z + 0.5, self.x, self.z)
    local x4, z4 = vec2.rotate(self.rot, self.x + 0.5, self.z - 0.5, self.x, self.z)
    local uv = self.uv

    gfx.bind_texture(atl_entities.texture.id)
    gfx.v3_t2(x1, 0.4, z1, uv.u1, uv.v1)
    gfx.v3_t2(x2, 0.4, z2, uv.u1, uv.v2)
    gfx.v3_t2(x3, 0.4, z3, uv.u2, uv.v2)
    gfx.v3_t2(x4, 0.4, z4, uv.u2, uv.v1)
end

return Bullet
