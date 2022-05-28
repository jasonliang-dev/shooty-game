local class = require "class"
local vec2 = require "vec".vec2
local Sprite = require "sprite"

local core = _G["core"]

local Explode = class()
Explode.classname = "Explode"

local Bullet = class()
Bullet.classname = "Bullet"

function Explode:new(desc)
    self.x = desc.x
    self.y = desc.y
    self.z = desc.z

    self.sprite = Sprite {
        atlas = ATL_ENTITIES,
        initial = "default",
        animations = {
            default = {ms_per_frame = 50, frames = {"explode_1", "explode_2", "explode_3", "explode_4"}}
        }
    }

    core.snd.play "data/content/boom.wav"
end

function Explode:update(dt)
    local looped = self.sprite:update(dt)
    if looped then
        self.dead = true
    end
end

function Explode:draw()
    local pad = 0.3
    local x1 = self.x - pad
    local y1 = self.y + pad
    local z1 = self.z - pad
    local x2 = self.x + pad
    local y2 = self.y - pad
    local z2 = self.z + pad

    local uv = self.sprite:uv()
    core.gfx.bind_texture(self.sprite.atlas.texture.id)
    core.gfx.v3_t2(x1, y1, z1, uv.u1, uv.v1)
    core.gfx.v3_t2(x1, y2, z1, uv.u1, uv.v2)
    core.gfx.v3_t2(x2, y2, z2, uv.u2, uv.v2)
    core.gfx.v3_t2(x2, y1, z2, uv.u2, uv.v1)

    core.gfx.v3_t2(x1, y1, z2, uv.u1, uv.v1)
    core.gfx.v3_t2(x1, y2, z2, uv.u1, uv.v2)
    core.gfx.v3_t2(x2, y2, z1, uv.u2, uv.v2)
    core.gfx.v3_t2(x2, y1, z1, uv.u2, uv.v1)
end

function Bullet:new(desc)
    self.uv = ATL_ENTITIES:uv "bullet_1"
    self.x = desc.x
    self.y = 0.4
    self.z = desc.z
    self.dx = desc.dx
    self.dz = desc.dz
    self.lifetime = 5
    self.rot = desc.rot - math.pi / 2

    core.snd.play "data/content/beep.wav"
end

function Bullet:update(dt)
    local speed = 30
    self.x = self.x + self.dx * speed * dt
    self.z = self.z + self.dz * speed * dt

    self.lifetime = self.lifetime - dt
    if self.lifetime <= 0 then
        self.dead = true
    end
end

function Bullet:on_death()
    if self.lifetime <= 0 then
        return
    end

    self.group:add(Explode, {
        x = self.x,
        y = self.y,
        z = self.z,
    })
end

function Bullet:draw()
    local x1, z1 = vec2.rotate(self.rot, self.x - 0.5, self.z - 0.5, self.x, self.z)
    local x2, z2 = vec2.rotate(self.rot, self.x - 0.5, self.z + 0.5, self.x, self.z)
    local x3, z3 = vec2.rotate(self.rot, self.x + 0.5, self.z + 0.5, self.x, self.z)
    local x4, z4 = vec2.rotate(self.rot, self.x + 0.5, self.z - 0.5, self.x, self.z)
    local uv = self.uv

    core.gfx.bind_texture(ATL_ENTITIES.texture.id)
    core.gfx.v3_t2(x1, self.y, z1, uv.u1, uv.v1)
    core.gfx.v3_t2(x2, self.y, z2, uv.u1, uv.v2)
    core.gfx.v3_t2(x3, self.y, z3, uv.u2, uv.v2)
    core.gfx.v3_t2(x4, self.y, z4, uv.u2, uv.v1)
end

return Bullet
