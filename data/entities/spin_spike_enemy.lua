local class = require "class"
local vec2 = require "vec".vec2
local enemy_common = require "enemy_common"

local core = _G["core"]

local SpinSpikeEnemy = class()
SpinSpikeEnemy.classname = "SpinSpikeEnemy"

function SpinSpikeEnemy:new(desc)
    self.origin_x = desc.origin_x
    self.origin_z = desc.origin_z
    self.rot = math.random() * math.pi * 2
    self.orbit = math.random() * math.pi * 2
    self.radius = desc.radius

    self.x = 1 / 0
    self.y = 0.4
    self.z = 1 / 0

    self.spike_uv = ATL_ENTITIES:uv "spike_1"
    self.shadow_uv = ATL_ENTITIES:uv "circle_small"
end

function SpinSpikeEnemy:update(dt)
    self.rot = self.rot + 5 * dt
    self.orbit = self.orbit - dt

    self.x = self.origin_x + math.cos(self.orbit) * self.radius
    self.z = self.origin_z + math.sin(self.orbit) * self.radius

    local bullets = self.group:nearby_classname("Bullet", self.x, self.y, self.z, 0.8)
    for _, bullet in pairs(bullets) do
        bullet.dead = true
    end

    enemy_common.player_collision(self, 0.8)
end

function SpinSpikeEnemy:draw()
    local off = 0.5
    local x1, z1 = vec2.rotate(self.rot, self.x - off, self.z - off, self.x, self.z)
    local x2, z2 = vec2.rotate(self.rot, self.x - off, self.z + off, self.x, self.z)
    local x3, z3 = vec2.rotate(self.rot, self.x + off, self.z + off, self.x, self.z)
    local x4, z4 = vec2.rotate(self.rot, self.x + off, self.z - off, self.x, self.z)

    local y = self.y

    local uv = self.spike_uv

    core.gfx.bind_texture(ATL_ENTITIES.texture.id)
    core.gfx.v3_t2(x1, y, z1, uv.u1, uv.v1)
    core.gfx.v3_t2(x2, y, z2, uv.u1, uv.v2)
    core.gfx.v3_t2(x3, y, z3, uv.u2, uv.v2)
    core.gfx.v3_t2(x4, y, z4, uv.u2, uv.v1)

    x1 = self.x - 0.4
    z1 = self.z - 0.4
    x2 = self.x + 0.4
    z2 = self.z + 0.4
    uv = self.shadow_uv

    core.gfx.v3_t2_c4(x1, 0.01, z1, uv.u1, uv.v1, 240, 240, 255, 32)
    core.gfx.v3_t2_c4(x1, 0.01, z2, uv.u1, uv.v2, 240, 240, 255, 32)
    core.gfx.v3_t2_c4(x2, 0.01, z2, uv.u2, uv.v2, 240, 240, 255, 32)
    core.gfx.v3_t2_c4(x2, 0.01, z1, uv.u2, uv.v1, 240, 240, 255, 32)
end

return SpinSpikeEnemy
