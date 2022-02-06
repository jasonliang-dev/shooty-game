local class = require "class"
local ray = require "ray"
local mouse = require "mouse"
local vec2 = require "vec".vec2

local Crosshair = class()

function Crosshair:new(desc)
    self.camera = desc.camera
    self.x = 1 / 0
    self.y = 0.41
    self.z = 1 / 0
    self.crosshair_uv = atl_entities:uv "cursor"
    self.rot = 0
end

function Crosshair:update(dt)
    self.rot = self.rot + dt

    local width = 50
    local raycast = ray.vs_quad {
        ray = ray.from_screen(mouse.x, mouse.y, self.camera),
        v1 = {x = -width, y = self.y, z = -width},
        v2 = {x = -width, y = self.y, z = width},
        v3 = {x = width, y = self.y, z = width},
        v4 = {x = width, y = self.y, z = -width},
    }

    if raycast then
        self.x = raycast.point.x
        self.z = raycast.point.z
    end
end

function Crosshair:draw()
    local off = 0.5
    local x1, z1 = vec2.rotate(self.rot, self.x - off, self.z - off, self.x, self.z)
    local x2, z2 = vec2.rotate(self.rot, self.x - off, self.z + off, self.x, self.z)
    local x3, z3 = vec2.rotate(self.rot, self.x + off, self.z + off, self.x, self.z)
    local x4, z4 = vec2.rotate(self.rot, self.x + off, self.z - off, self.x, self.z)

    local uv = self.crosshair_uv

    gfx.bind_texture(atl_entities.texture.id)
    gfx.v3_t2(x1, self.y, z1, uv.u1, uv.v1)
    gfx.v3_t2(x2, self.y, z2, uv.u1, uv.v2)
    gfx.v3_t2(x3, self.y, z3, uv.u2, uv.v2)
    gfx.v3_t2(x4, self.y, z4, uv.u2, uv.v1)

    local r, g, b, a = 0, 0, 20, 16
    gfx.v3_t2_c4(x1, 0.01, z1, uv.u1, uv.v1, r, g, b, a)
    gfx.v3_t2_c4(x2, 0.01, z2, uv.u1, uv.v2, r, g, b, a)
    gfx.v3_t2_c4(x3, 0.01, z3, uv.u2, uv.v2, r, g, b, a)
    gfx.v3_t2_c4(x4, 0.01, z4, uv.u2, uv.v1, r, g, b, a)
end

return Crosshair
