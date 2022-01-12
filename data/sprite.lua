local class = require "class"

local Sprite = class()

function Sprite:new(desc)
    self.texture = desc.texture
    self.ms_per_frame = desc.ms_per_frame
    self.uvs = desc.uvs
    self.elapsed = 0
end

function Sprite:update(dt)
    self.elapsed = math.fmod(self.elapsed + dt * 1000, self.ms_per_frame * #self.uvs)
end

function Sprite:uv()
    local frame = self.elapsed // self.ms_per_frame
    return self.uvs[frame + 1]
end

function Sprite:reset()
    self.elapsed = 0
end

return Sprite
