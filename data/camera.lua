local class = require "class"
local mat = require "mat"
local lume = require "deps.lume"

local Camera = class()

function Camera:new(x, y, z, mat_rot)
    self.x = x
    self.y = y
    self.z = z
    self.mat_rot = mat_rot or mat.identity()
end

function Camera:set_target(x, y, z)
    self.target = {x = x, y = y, z = z}
end

function Camera:set_pos(x, y, z)
    self.x = x
    self.y = y
    self.z = z
end

function Camera:set_rot(mat_rot)
    self.mat_rot = mat_rot
end

function Camera:shake()

end

function Camera:update(dt)
    if not self.target then
        return
    end

    local blend = 1 - 0.85 ^ (dt * 40)
    self.x = lume.lerp(self.x, self.target.x, blend)
    self.y = lume.lerp(self.y, self.target.y, blend)
    self.z = lume.lerp(self.z, self.target.z, blend)
end

function Camera:view_projection()
    local view = mat.mul(mat.translate(-self.x, -self.y, -self.z), self.mat_rot)
    local projection = mat.perspective(math.pi / 4, sys.window_width() / sys.window_height(), 0.05, 1000)
    return mat.mul(view, projection)
end

return Camera
