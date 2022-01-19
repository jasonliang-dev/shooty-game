local class = require "class"

local Spring = class()

function Spring:new(k, d, x)
    self.stiffness = k or 400
    self.damping = d or 28
    self.x = x or 0
    self.target_x = self.x
    self.v = 0
end

function Spring:update(dt)
    local a = -self.stiffness * (self.x - self.target_x) - self.damping * self.v
    self.v = self.v + a * dt
    self.x = self.x + self.v * dt
end

function Spring:pull(f)
    self.x = self.x + f
end

return Spring
