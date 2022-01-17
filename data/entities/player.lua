local class = require "class"
local keyboard = require "keyboard"
local vec2 = require "vec".vec2
local lume = require "deps.lume"
local Sprite = require "sprite"
local StateMachine = require "state_machine"
local Deque = require "deque"

local Player = class()

function Player:new(x, z)
    self.x = x
    self.z = z
    self.dx = 0
    self.dz = 0
    self.dash_time = 0
    self.dash_time_init = 0.24
    self.facing_right = false

    self.sprite = Sprite {
        atlas = atl_characters,
        initial = "idle",
        animations = {
            idle = {ms_per_frame = 110, frames = {"character_0000"}},
            walk = {ms_per_frame = 110, frames = {"character_0001", "character_0000"}},
            dash = {ms_per_frame = 40, frames = {"character_0001"}},
        },
    }

    self.fsm = StateMachine {
        owner = self,
        initial = "idle",
        states = {
            idle = {enter = self.fsm_enter_idle, update = self.fsm_idle},
            walk = {enter = self.fsm_enter_walk, update = self.fsm_walk},
            dash = {enter = self.fsm_enter_dash, update = self.fsm_dash, leave = self.fsm_leave_dash},
        },
    }

    self.ghosts = Deque()
end

function Player:update(dt)
    self.sprite:update(dt)
    self.fsm:update(dt)
end

function Player:fsm_enter_idle()
    self.sprite:play "idle"
end

function Player:fsm_idle(dt)
    self.dx = 0
    self.dz = 0
    if keyboard.down "w" then self.dz = self.dz - 1 end
    if keyboard.down "s" then self.dz = self.dz + 1 end
    if keyboard.down "a" then self.dx = self.dx - 1 end
    if keyboard.down "d" then self.dx = self.dx + 1 end

    if self.dx ~= 0 or self.dz ~= 0 then
        self.fsm:transition "walk"
    end
end

function Player:fsm_enter_walk()
    self.sprite:play "walk"
end

function Player:fsm_walk(dt)
    local spd = dt * 5

    self.dx = 0
    self.dz = 0
    if keyboard.down "w" then self.dz = self.dz - 1 end
    if keyboard.down "s" then self.dz = self.dz + 1 end
    if keyboard.down "a" then self.dx = self.dx - 1 end
    if keyboard.down "d" then self.dx = self.dx + 1 end
    if self.dx ~= 0 or self.dz ~= 0 then
        self.dx, self.dz = vec2.normalize(self.dx, self.dz)
    end

    if self.dx == 0 and self.dz == 0 then
        self.fsm:transition "idle"
    elseif keyboard.pressed "space" then
        self.fsm:transition "dash"
    end

    if self.dx > 0 then
        self.facing_right = true
    elseif self.dx < 0 then
        self.facing_right = false
    end

    self.x = self.x + self.dx * spd
    self.z = self.z + self.dz * spd
end

function Player:fsm_enter_dash()
    self.dash_time = self.dash_time_init
end

function Player:fsm_dash(dt)
    if self.dash_time > 0 then
        local spd = dt * 9
        self.x = self.x + self.dx * spd
        self.z = self.z + self.dz * spd
        self.dash_time = self.dash_time - dt
    else
        self.fsm:transition "idle"
    end
end

function Player:fsm_leave_dash()
    self.dash_time = 0
end

function Player:draw()
    local rad = lume.lerp(math.pi * 2, 0, self.dash_time / self.dash_time_init)
    if self.facing_right then
        rad = -rad
    end

    local x1, y1 = vec2.rotate(rad, self.x + 0, 1.2, self.x + 0.5, 0.6)
    local x2, y2 = vec2.rotate(rad, self.x + 0, 0.0, self.x + 0.5, 0.6)
    local x3, y3 = vec2.rotate(rad, self.x + 1, 0.0, self.x + 0.5, 0.6)
    local x4, y4 = vec2.rotate(rad, self.x + 1, 1.2, self.x + 0.5, 0.6)

    local z1 = self.z
    local z2 = self.z

    local uv = self.sprite:uv()
    local u1 = self.facing_right and uv.u2 or uv.u1
    local v1 = uv.v1
    local u2 = self.facing_right and uv.u1 or uv.u2
    local v2 = uv.v2

    local r, g, b, a
    if self.dash_time > 0 then
        r, g, b, a = 230, 230, 255, 255
    else
        r, g, b, a = 255, 255, 255, 255
    end

    gfx.bind_texture(self.sprite.atlas.texture.id)
    gfx.v3_t2_c4_f4(x1, y1, z1, u1, v1, r, g, b, a, 0, 0, 0, 0)
    gfx.v3_t2_c4_f4(x2, y2, z2, u1, v2, r, g, b, a, 0, 0, 0, 0)
    gfx.v3_t2_c4_f4(x3, y3, z2, u2, v2, r, g, b, a, 0, 0, 0, 0)
    gfx.v3_t2_c4_f4(x4, y4, z1, u2, v1, r, g, b, a, 0, 0, 0, 0)
end

return Player
