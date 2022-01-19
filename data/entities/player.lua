local class = require "class"
local keyboard = require "keyboard"
local mouse = require "mouse"
local vec2 = require "vec".vec2
local lume = require "deps.lume"
local ray = require "ray"
local Sprite = require "sprite"
local Bullet = require "entities.bullet"
local StateMachine = require "state_machine"
local Spring = require "spring"

local Player = class()

function Player:new(desc)
    self.camera = desc.camera
    self.map = desc.map

    self.x = desc.x
    self.z = desc.z
    self.dx = 0
    self.dz = 0
    self.dash_time = 0
    self.dash_time_init = 0.24
    self.facing_right = false
    self.shoot_spring = Spring(400, 28, 0)

    self.sprite = Sprite {
        atlas = atl_entities,
        initial = "idle",
        animations = {
            idle = {ms_per_frame = 110, frames = {"char1_1"}},
            walk = {ms_per_frame = 110, frames = {"char1_2", "char1_1"}},
            dash = {ms_per_frame = 40, frames = {"char1_1"}},
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
end

function Player:update(dt)
    self.shoot_spring:update(dt)
    self.sprite:update(dt)
    self.fsm:update(dt)

    local state = self.fsm.current_state
    if state == "idle" or state == "walk" then
        local raycast = ray.vs_quad {
            ray = ray.from_screen(mouse.x, mouse.y, self.camera),
            v1 = {x = -50, y = 0, z = -50},
            v2 = {x = -50, y = 0, z = 50},
            v3 = {x = 50, y = 0, z = 50},
            v4 = {x = 50, y = 0, z = -50},
        }

        if raycast then
            self.facing_right = raycast.point.x > self.x

            if mouse.clicked "left" then
                local dx = raycast.point.x - self.x
                local dz = raycast.point.z - self.z
                dx, dz = vec2.normalize(dx, dz)
                self.group:add(Bullet, self.x, self.z, dx, dz)
                self.shoot_spring:pull(0.12)
            end
        end
    end
end

function Player:move(spd)
    local sub_steps = 4
    local dx, dz = self.dx / sub_steps, self.dz / sub_steps
    local x, z

    for i = 1, sub_steps do
        x, z = self.x, self.z
        self.x = x + dx * spd
        if self.map:point_collision(self.x, self.z) then
            self.x, self.z = x, z
        end

        x, z = self.x, self.z
        self.z = z + dz * spd
        if self.map:point_collision(self.x, self.z) then
            self.x, self.z = x, z
        end
    end
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

    self:move(5 * dt)
end

function Player:fsm_enter_dash()
    self.dash_time = self.dash_time_init
end

function Player:fsm_dash(dt)
    if self.dash_time > 0 then
        self:move(9 * dt)
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
    if self.dx > 0 then
        rad = -rad
    end

    local x1, y1, z1, x2, y2, z2, x3, y3, x4, y4
    if self.dx ~= 0 then
        x1, y1 = vec2.rotate(rad, self.x - 0.5, 1.2, self.x, 0.6)
        x2, y2 = vec2.rotate(rad, self.x - 0.5, 0.0, self.x, 0.6)
        x3, y3 = vec2.rotate(rad, self.x + 0.5, 0.0, self.x, 0.6)
        x4, y4 = vec2.rotate(rad, self.x + 0.5, 1.2, self.x, 0.6)
        z1, z2 = self.z, self.z
    else
        x1, x2 = self.x - 0.5, self.x - 0.5
        x3, x4 = self.x + 0.5, self.x + 0.5

        y1, z1 = vec2.rotate(rad, 1.2, self.z, 0.6, self.z)
        y2, z2 = vec2.rotate(rad, 0.0, self.z, 0.6, self.z)
        y3 = y2
        y4 = y1
    end

    x1 = x1 - self.shoot_spring.x
    x2 = x2 - self.shoot_spring.x
    x3 = x3 + self.shoot_spring.x
    x4 = x4 + self.shoot_spring.x

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
    gfx.v3_t2_c4(x1, y1, z1, u1, v1, r, g, b, a)
    gfx.v3_t2_c4(x2, y2, z2, u1, v2, r, g, b, a)
    gfx.v3_t2_c4(x3, y3, z2, u2, v2, r, g, b, a)
    gfx.v3_t2_c4(x4, y4, z1, u2, v1, r, g, b, a)
end

return Player
