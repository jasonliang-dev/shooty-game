local class = require "class"

local Progress = class()

function Progress:new(limit)
    self.limit = limit
    self.time = 0
end

function Progress:update(dt)
    self.time = self.time + dt
end

function Progress:percent()
    return self.time / self.limit
end

return Progress
