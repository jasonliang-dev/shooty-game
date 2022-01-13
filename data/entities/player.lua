local class = require "class"
local keyboard = require "keyboard"
local vec2 = require "vec".vec2
local Sprite = require "sprite"

local Player = class()

function Player:new()
    self.x = 3
    self.z = 3
    self.facing_right = false
    self.sprite = Sprite {
        atlas = atl_characters,
        ms_per_frame = 120,
        current_animation = "idle",
        animations = {
            idle = {
                "character_0000",
            },
            walk = {
                "character_0001",
                "character_0000",
            },
        },
    }
end

function Player:update(dt)
    local dx = 0
    local dz = 0
    if keyboard.down "w" then dz = dz - 1 end
    if keyboard.down "s" then dz = dz + 1 end
    if keyboard.down "a" then dx = dx - 1 end
    if keyboard.down "d" then dx = dx + 1 end
    dx, dz = vec2.normalize(dx, dz)

    local spd = dt * 4
    self.x = self.x + dx * spd
    self.z = self.z + dz * spd

    if dx == 0 and dz == 0 then
        self.sprite:play "idle"
    elseif self.sprite.current_animation == "idle" then
        self.sprite:play "walk"
    end

    if dx > 0 then
        self.facing_right = true
    elseif dx < 0 then
        self.facing_right = false
    end

    self.sprite:update(dt)
end

function Player:draw()
    local x1 = self.x
    local y1 = 1
    local z1 = self.z - 0.5
    local x2 = self.x + 1
    local y2 = 0
    local z2 = self.z

    local uv = self.sprite:uv()
    local u1 = self.facing_right and uv.u2 or uv.u1
    local v1 = uv.v1
    local u2 = self.facing_right and uv.u1 or uv.u2
    local v2 = uv.v2

    gfx.bind_texture(self.sprite.atlas.texture.id)
    gfx.v3_t2(x1, y1, z1, u1, v1)
    gfx.v3_t2(x1, y2, z2, u1, v2)
    gfx.v3_t2(x2, y2, z2, u2, v2)
    gfx.v3_t2(x2, y1, z1, u2, v1)
end

return Player
