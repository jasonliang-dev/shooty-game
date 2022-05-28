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
local Progress = require "progress"

local core = _G["core"]

local Player = class()
Player.classname = "Player"

function Player:new(desc)
    self.camera = desc.camera
    self.map = desc.map

    self.spawn_x = desc.x
    self.spawn_z = desc.z

    self.health = desc.health
    self.x = desc.x
    self.y = 0
    self.z = desc.z
    self.dx = 0
    self.dy = 0
    self.dz = 0
    self.facing_right = false
    self.shoot_spring = Spring()
    self.hit_this_frame = false
    self.p_dash = Progress(0.25)
    self.p_invulnerable = Progress(1, true)

    self.sprite = Sprite {
        atlas = ATL_ENTITIES,
        initial = "idle",
        animations = {
            idle = {ms_per_frame = 110, frames = {"char1_1"}},
            walk = {ms_per_frame = 110, frames = {"char1_2", "char1_1"}},
            dash = {ms_per_frame = 110, frames = {"char1_1"}},
            hurt = {ms_per_frame = 110, frames = {"char1_1"}},
            sink = {ms_per_frame = 110, frames = {"char1_1"}},
        },
    }

    self.fsm = StateMachine {
        owner = self,
        initial = "idle",
        states = {
            idle = {enter = self.fsm_enter_idle, update = self.fsm_idle},
            walk = {enter = self.fsm_enter_walk, update = self.fsm_walk},
            dash = {enter = self.fsm_enter_dash, update = self.fsm_dash, leave = self.fsm_leave_dash},
            hurt = {enter = self.fsm_enter_hurt, update = self.fsm_hurt},
            sink = {enter = self.fsm_enter_sink, update = self.fsm_sink, leave = self.fsm_leave_sink},
        },
    }
end

function Player:init()
    self.group.player = self
end

function Player:update(dt)
    self.shoot_spring:update(dt)
    self.p_invulnerable:update(dt)
    self.sprite:update(dt)
    self.fsm:update(dt)
end

function Player:late_update(dt)
    if self.hit_this_frame then
        if self.p_invulnerable:percent() >= 1 then
            self.p_invulnerable.time = 0
            self.dx = self.hit_this_frame.dx
            self.dz = self.hit_this_frame.dz
            self.fsm:transition "hurt"
            self.camera:shake(0.2, 0.8)
        end

        self.hit_this_frame = nil
    end
end

function Player:handle_shoot()
    local cast = 50
    local off_y = 0.4
    local raycast = ray.vs_quad {
        ray = ray.from_screen(mouse.x, mouse.y, self.camera),
        v1 = {x = -cast, y = off_y, z = -cast},
        v2 = {x = -cast, y = off_y, z = cast},
        v3 = {x = cast, y = off_y, z = cast},
        v4 = {x = cast, y = off_y, z = -cast},
    }

    if raycast then
        self.facing_right = raycast.point.x > self.x

        if mouse.clicked "left" then
            local dx = raycast.point.x - self.x
            local dz = raycast.point.z - self.z
            dx, dz = vec2.normalize(dx, dz)
            self.group:add(Bullet, {
                x = self.x,
                z = self.z,
                dx = dx,
                dz = dz,
                rot = lume.angle(self.x, self.z, raycast.point.x, raycast.point.z),
            })
            self.shoot_spring:pull(0.08)
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

    self:handle_shoot()
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

    self.x, self.z = self.map:point_move(self.x, self.z, self.dx * 5 * dt, self.dz * 5 * dt, 4)

    self:handle_shoot()
end

function Player:fsm_enter_dash()
    self.sprite:play "dash"
    core.snd.play "data/content/swish.wav"
end

function Player:fsm_dash(dt)
    local dash_speed = 10
    if self.p_dash:percent() < 1 then
        self.p_dash:update(dt)
        self.x, self.z = self.map:point_move(self.x, self.z, self.dx * dash_speed * dt, self.dz * dash_speed * dt, 4)
    elseif keyboard.any_down {"w", "a", "s", "d"} then
        self.fsm:transition "walk"
    else
        self.fsm:transition "idle"
    end
end

function Player:fsm_leave_dash()
    self.p_dash.time = 0
end

function Player:fsm_enter_hurt()
    self.sprite:play "hurt"
    self.health = self.health - 1
    self.dy = 6
    self.p_invulnerable.time = 0
end

function Player:fsm_hurt(dt)
    self.dy = self.dy - 30 * dt
    self.y = self.y + self.dy * dt

    self.x = self.x + self.dx * 4 * dt
    self.z = self.z + self.dz * 4 * dt

    if self.y <= 0 then
        if self.map:point_collision(self.x, self.z) then
            self.fsm:transition "sink"
        else
            self.y = 0
            self.fsm:transition "idle"
        end
    end
end

function Player:fsm_enter_sink()
    self.sprite:play "sink"
end

function Player:fsm_sink(dt)
    self.y = self.y - 2 * dt
    if self.y < -1 then
        self.fsm:transition "idle"
    end
end

function Player:fsm_leave_sink()
    self.y = 0
    self.x = self.spawn_x
    self.z = self.spawn_z
    self.p_invulnerable.time = 0
end

function Player:draw()
    if self.p_invulnerable:percent() <= 1 then
        local blink_range = math.fmod(self.p_invulnerable.time, 0.1)
        if blink_range < 0.05 then
            return
        end
    end

    local rad = lume.lerp(0, math.pi * 2, self.p_dash:percent())

    local x1, y1, z1, x2, y2, z2, x3, y3, x4, y4
    if self.dx ~= 0 then
        if self.dx > 0 then
            rad = -rad
        end

        x1, y1 = vec2.rotate(rad, self.x - 0.5, self.y + 1.2, self.x, self.y + 0.6)
        x2, y2 = vec2.rotate(rad, self.x - 0.5, self.y + 0.0, self.x, self.y + 0.6)
        x3, y3 = vec2.rotate(rad, self.x + 0.5, self.y + 0.0, self.x, self.y + 0.6)
        x4, y4 = vec2.rotate(rad, self.x + 0.5, self.y + 1.2, self.x, self.y + 0.6)
        z1, z2 = self.z, self.z
    else
        x1, x2 = self.x - 0.5, self.x - 0.5
        x3, x4 = self.x + 0.5, self.x + 0.5

        y1, z1 = vec2.rotate(rad, self.y + 1.2, self.z, self.y + 0.6, self.z)
        y2, z2 = vec2.rotate(rad, self.y + 0.0, self.z, self.y + 0.6, self.z)
        y3 = y2
        y4 = y1
    end

    x1 = x1 - self.shoot_spring.x
    x2 = x2 - self.shoot_spring.x
    x3 = x3 + self.shoot_spring.x
    x4 = x4 + self.shoot_spring.x
    y1 = y1 - self.shoot_spring.x
    y2 = y2 + self.shoot_spring.x
    y3 = y3 + self.shoot_spring.x
    y4 = y4 - self.shoot_spring.x

    local uv = self.sprite:uv()
    local u1 = self.facing_right and uv.u2 or uv.u1
    local v1 = uv.v1
    local u2 = self.facing_right and uv.u1 or uv.u2
    local v2 = uv.v2

    local r, g, b, a
    if self.p_dash.time ~= 0 then
        r, g, b, a = 230, 230, 255, 255
    else
        r, g, b, a = 255, 255, 255, 255
    end

    core.gfx.bind_texture(self.sprite.atlas.texture.id)
    core.gfx.v3_t2_c4(x1, y1, z1, u1, v1, r, g, b, a)
    core.gfx.v3_t2_c4(x2, y2, z2, u1, v2, r, g, b, a)
    core.gfx.v3_t2_c4(x3, y3, z2, u2, v2, r, g, b, a)
    core.gfx.v3_t2_c4(x4, y4, z1, u2, v1, r, g, b, a)
end

function Player:draw_hp_bar(x, y)
    for i = 1, self.health do
        local x1 = x + (i - 1) * 48
        local y1 = y
        local x2 = x1 + 32
        local y2 = y1 + 32

        local uv = self.sprite.atlas:uv "heart"

        core.gfx.bind_texture(self.sprite.atlas.texture.id)
        core.gfx.v3_t2(x1, y1, 0, uv.u1, uv.v1)
        core.gfx.v3_t2(x1, y2, 0, uv.u1, uv.v2)
        core.gfx.v3_t2(x2, y2, 0, uv.u2, uv.v2)
        core.gfx.v3_t2(x2, y1, 0, uv.u2, uv.v1)
    end
end

return Player
