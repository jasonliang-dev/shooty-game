local class = require "class"

local Sprite = class()

function Sprite:new(desc)
    self.atlas = desc.atlas
    self.ms_per_frame = desc.ms_per_frame

    self.animations = {}
    for name, frames in pairs(desc.animations) do
        self.animations[name] = {}
        for _, str in ipairs(frames) do
            table.insert(self.animations[name], self.atlas:uv(str))
        end
    end

    self.current_animation = desc.current_animation or next(self.animations)
    self.elapsed = 0
end

function Sprite:play(name, frame)
    frame = frame or 1
    self.current_animation = name
    self.elapsed = (frame - 1) * self.ms_per_frame
end

function Sprite:update(dt)
    self.elapsed = math.fmod(self.elapsed + dt * 1000, self.ms_per_frame * #self.animations[self.current_animation])
end

function Sprite:uv()
    local frame = self.elapsed // self.ms_per_frame
    return self.animations[self.current_animation][frame + 1]
end

function Sprite:reset()
    self.elapsed = 0
end

return Sprite
