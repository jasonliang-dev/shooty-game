local class = require "class"
local vec2 = require "vec".vec2
local Progress = require "progress"
local Spring = require "spring"

local SpinSpikeEnemy = class()
SpinSpikeEnemy.classname = "SpinSpikeEnemy"

function SpinSpikeEnemy:new(desc)
    self.origin_x = desc.origin_x
    self.origin_z = desc.origin_z
    self.rot = math.random() * math.pi * 2
    self.orbit = math.random() * math.pi * 2
    self.radius = desc.radius

    self.hit_spring = Spring()
    self.p_hit = Progress(0.08)
    self.p_hit.time = 1 / 0

    self.x = 1 / 0
    self.y = 0.4
    self.z = 1 / 0

    self.spike_uv = atl_entities:uv "spike_1"
    self.shadow_uv = atl_entities:uv "circle_small"
end

function SpinSpikeEnemy:update(dt)
    self.hit_spring:update(dt)
    self.p_hit:update(dt)
    self.rot = self.rot + 5 * dt
    self.orbit = self.orbit - dt

    self.x = self.origin_x + math.cos(self.orbit) * self.radius
    self.z = self.origin_z + math.sin(self.orbit) * self.radius

    local bullets = self.group:nearby_classname("Bullet", self.x, self.y, self.z, 0.8)
    for _, bullet in pairs(bullets) do
        -- self.p_hit.time = 0
        -- self.hit_spring:pull(0.1)
        bullet.dead = true
    end
end

function SpinSpikeEnemy:draw()
    local off = 0.5 + self.hit_spring.x
    local x1, z1 = vec2.rotate(self.rot, self.x - off, self.z - off, self.x, self.z)
    local x2, z2 = vec2.rotate(self.rot, self.x - off, self.z + off, self.x, self.z)
    local x3, z3 = vec2.rotate(self.rot, self.x + off, self.z + off, self.x, self.z)
    local x4, z4 = vec2.rotate(self.rot, self.x + off, self.z - off, self.x, self.z)

    local y = self.y + self.hit_spring.x

    local uv = self.spike_uv

    local p = 1 - math.min(1, self.p_hit:percent())
    p = math.floor(p * 255)

    gfx.bind_texture(atl_entities.texture.id)
    gfx.v3_t2(x1, y, z1, uv.u1, uv.v1, p, p, p, p)
    gfx.v3_t2(x2, y, z2, uv.u1, uv.v2, p, p, p, p)
    gfx.v3_t2(x3, y, z3, uv.u2, uv.v2, p, p, p, p)
    gfx.v3_t2(x4, y, z4, uv.u2, uv.v1, p, p, p, p)

    x1 = self.x - 0.4
    z1 = self.z - 0.4
    x2 = self.x + 0.4
    z2 = self.z + 0.4
    uv = self.shadow_uv

    gfx.v3_t2_c4(x1, 0.01, z1, uv.u1, uv.v1, 240, 240, 255, 32)
    gfx.v3_t2_c4(x1, 0.01, z2, uv.u1, uv.v2, 240, 240, 255, 32)
    gfx.v3_t2_c4(x2, 0.01, z2, uv.u2, uv.v2, 240, 240, 255, 32)
    gfx.v3_t2_c4(x2, 0.01, z1, uv.u2, uv.v1, 240, 240, 255, 32)
end

return SpinSpikeEnemy
