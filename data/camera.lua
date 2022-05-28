local class = require "class"
local mat = require "mat"
local quat = require "quat"
local lume = require "deps.lume"

local core = _G["core"]

local Camera = class()

function Camera:new(x, y, z, rot)
    self.x = x
    self.y = y
    self.z = z
    self.rot = rot or quat.identity()
    self.shake_str = 0
    self.shake_amp = 0
end

function Camera:set_target(x, y, z)
    self.target = {x = x, y = y, z = z}
end

function Camera:shake(str, amp)
    self.shake_str = math.max(self.shake_str, str)
    self.shake_amp = math.max(self.shake_amp, amp or 0.5)
end

function Camera:update(dt)
    if not self.target then
        return
    end

    local blend = 1 - 0.85 ^ (dt * 40)
    self.x = lume.lerp(self.x, self.target.x, blend)
    self.y = lume.lerp(self.y, self.target.y, blend)
    self.z = lume.lerp(self.z, self.target.z, blend)
    self.shake_str = self.shake_str - dt
end

function Camera:view_projection()
    local x = -self.x
    local y = -self.y
    local z = -self.z

    if self.shake_str > 0 then
        x = x + lume.random(-self.shake_str, self.shake_str) * self.shake_amp
        y = y + lume.random(-self.shake_str, self.shake_str) * self.shake_amp
        z = z + lume.random(-self.shake_str, self.shake_str) * self.shake_amp
    end

    local view = mat.mul(mat.translate(x, y, z), mat.rotate(self.rot))
    local projection = mat.perspective(math.pi / 4, core.sys.window_width() / core.sys.window_height(), 0.05, 1000)
    return mat.mul(view, projection)
end

return Camera
