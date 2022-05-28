local class = require "class"
local vec2 = require "vec".vec2

local core = _G["core"]

local Corpse = class()
Corpse.classname = "Corpse"

function Corpse:new(desc)
    self.x = desc.x
    self.y = desc.y
    self.dy = 8
    self.z = desc.z
    self.rot = 0
    self.uv = desc.uv
end

function Corpse:update(dt)
    self.dy = self.dy - 30 * dt
    self.y = self.y + self.dy * dt
    self.rot = self.rot + dt

    if self.y < -1 then
        self.dead = true
    end
end

function Corpse:draw()
    local x1, y1 = vec2.rotate(self.rot, self.x - 0.5, self.y + 1.2, self.x, self.y + 0.6)
    local x2, y2 = vec2.rotate(self.rot, self.x - 0.5, self.y + 0.0, self.x, self.y + 0.6)
    local x3, y3 = vec2.rotate(self.rot, self.x + 0.5, self.y + 0.0, self.x, self.y + 0.6)
    local x4, y4 = vec2.rotate(self.rot, self.x + 0.5, self.y + 1.2, self.x, self.y + 0.6)

    local uv = self.uv
    core.gfx.bind_texture(ATL_ENTITIES.texture.id)
    core.gfx.v3_t2(x1, y1, self.z, uv.u1, uv.v1)
    core.gfx.v3_t2(x2, y2, self.z, uv.u1, uv.v2)
    core.gfx.v3_t2(x3, y3, self.z, uv.u2, uv.v2)
    core.gfx.v3_t2(x4, y4, self.z, uv.u2, uv.v1)
end

return Corpse
