local class = require "class"

local Plant = class()

function Plant:new(x, z, name)
    self.x = x
    self.z = z
    self.uv = atl_entities:uv(name)
end

function Plant:update(dt) end

function Plant:draw()
    local x1 = self.x - 0.5
    local y1 = 1
    local z1 = self.z - 0.5
    local x2 = self.x + 0.5
    local y2 = 0
    local z2 = self.z + 0.5

    local uv = self.uv
    gfx.bind_texture(atl_entities.texture.id)
    gfx.v3_t2(x1, y1, z1, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, z1, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, z2, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, z2, uv.u2, uv.v1)

    gfx.v3_t2(x1, y1, z2, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, z2, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, z1, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, z1, uv.u2, uv.v1)
end

return Plant
