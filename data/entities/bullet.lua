local class = require "class"

local Bullet = class()

function Bullet:new(x, z, dx, dz)
    self.uv = atl_entities:uv "bullet"
    self.x = x
    self.z = z
    self.dx = dx
    self.dz = dz
    self.lifetime = 5
end

function Bullet:update(dt)
    local spd = dt * 20
    self.x = self.x + self.dx * spd
    self.z = self.z + self.dz * spd

    self.lifetime = self.lifetime - dt
    if self.lifetime <= 0 then
        self.dead = true
    end
end

function Bullet:draw()
    local x1 = self.x - 0.5
    local z1 = self.z - 0.5
    local x2 = self.x + 0.5
    local z2 = self.z + 0.5
    local uv = self.uv

    gfx.bind_texture(atl_entities.texture.id)
    gfx.v3_t2(x1, 0.4, z1, uv.u1, uv.v1)
    gfx.v3_t2(x1, 0.4, z2, uv.u1, uv.v2)
    gfx.v3_t2(x2, 0.4, z2, uv.u2, uv.v2)
    gfx.v3_t2(x2, 0.4, z1, uv.u2, uv.v1)
end

return Bullet
