local class = require "class"

local Sprite = class()

function Sprite:new(desc)
    self.atlas = desc.atlas

    self.animations = {}
    for name, animation in pairs(desc.animations) do
        self.animations[name] = {
            ms_per_frame = animation.ms_per_frame,
            frames = {},
        }
        for _, str in ipairs(animation.frames) do
            table.insert(self.animations[name].frames, self.atlas:uv(str))
        end
    end

    self.current_animation = desc.initial
    self.elapsed = 0
end

function Sprite:play(name, frame)
    frame = frame or 1
    self.current_animation = name
    local animation = self.animations[self.current_animation]
    self.elapsed = (frame - 1) * animation.ms_per_frame
end

function Sprite:update(dt)
    local animation = self.animations[self.current_animation]

    local elapsed = self.elapsed
    self.elapsed = math.fmod(elapsed + dt * 1000, animation.ms_per_frame * #animation.frames)
    return elapsed > self.elapsed
end

function Sprite:uv()
    local animation = self.animations[self.current_animation]
    local frame = self.elapsed // animation.ms_per_frame
    return animation.frames[frame + 1]
end

function Sprite:reset()
    self.elapsed = 0
end

return Sprite
