local class = require "class"

local Plant = class()

function Plant:new(x, z, name)
    self.x = x
    self.z = z
    self.uv = atl_plants:uv(name)
end

function Plant:update(dt) end

function Plant:draw()
    local x1 = self.x
    local y1 = 1
    local z1 = self.z
    local x2 = self.x + 1
    local y2 = 0
    local z2 = self.z

    local uv = self.uv
    gfx.bind_texture(atl_plants.texture.id)
    gfx.v3_t2(x1, y1, z1 + 0, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, z2 + 0, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, z2 + 1, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, z1 + 1, uv.u2, uv.v1)

    gfx.v3_t2(x1, y1, z1 + 1, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, z2 + 1, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, z2 + 0, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, z1 + 0, uv.u2, uv.v1)
end

return Plant
