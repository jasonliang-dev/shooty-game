local class = require "class"
local keyboard = require "keyboard"
local vec2 = require "vec".vec2
local ray = require "ray"

local Ray = class()

function Ray:new(desc)
    self.origin = desc.origin
    self.direction = desc.direction
end

function Ray:update(dt)
    local dx = 0
    local dz = 0
    if keyboard.down "left" then dx = dx - 1 end
    if keyboard.down "right" then dx = dx + 1 end
    if keyboard.down "up" then dz = dz - 1 end
    if keyboard.down "down" then dz = dz + 1 end
    if dx ~= 0 or dz ~= 0 then
        dx, dz = vec2.normalize(dx, dz)
    end

    local spd = dt * 4
    self.origin.x = self.origin.x + dx * spd
    self.origin.z = self.origin.z + dz * spd

    local raycast = ray.vs_triangle {
        ray = {origin = self.origin, direction = self.direction},
        v1 = {x = 1, y = 0, z = 1},
        v2 = {x = 1, y = 0, z = 9},
        v3 = {x = 9, y = 0, z = 1},
    }

    if raycast then
        self.collision = raycast.point
    else
        self.collision = nil
    end
end

function Ray:draw()
    local x1 = self.origin.x
    local y1 = self.origin.y
    local z1 = self.origin.z
    local x2 = self.origin.x + self.direction.x + 0.1
    local y2 = self.origin.y + self.direction.y + 0.1
    local z2 = self.origin.z + self.direction.z + 0.1

    local r, g, b, a
    if self.collision then
        r, g, b, a = 0, 255, 0, 255
    else
        r, g, b, a = 255, 0, 0, 255
    end

    gfx.bind_white_texture()
    gfx.v3_t2_c4_f4(x1, y1, z1, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0)
    gfx.v3_t2_c4_f4(x1, y2, z2, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0)
    gfx.v3_t2_c4_f4(x2, y2, z2, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0)
    gfx.v3_t2_c4_f4(x2, y1, z1, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0)

    if self.collision then
        x1 = self.collision.x
        z1 = self.collision.z
        x2 = self.collision.x + 0.1
        z2 = self.collision.z + 0.1

        gfx.v3_t2_c4_f4(x1, 0.01, z1, 0, 0, 0, 255, 0, 255, 0, 0, 0, 0)
        gfx.v3_t2_c4_f4(x1, 0.01, z2, 0, 0, 0, 255, 0, 255, 0, 0, 0, 0)
        gfx.v3_t2_c4_f4(x2, 0.01, z2, 0, 0, 0, 255, 0, 255, 0, 0, 0, 0)
        gfx.v3_t2_c4_f4(x2, 0.01, z1, 0, 0, 0, 255, 0, 255, 0, 0, 0, 0)
    end
end

return Ray
