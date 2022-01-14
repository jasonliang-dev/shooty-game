local class = require "class"
local keyboard = require "keyboard"
local vec2 = require "vec".vec2
local Sprite = require "sprite"

local Player = class()

function Player:new()
    self.x = 3
    self.z = 3
    self.dx = 0
    self.dz = 0
    self.dashing = false

    self.facing_right = false
    self.sprite = Sprite {
        atlas = atl_characters,
        current_animation = "idle",
        animations = {
            idle = {
                ms_per_frame = 120,
                frames = {"character_0000"},
            },
            walk = {
                ms_per_frame = 120,
                frames = {"character_0001", "character_0000"},
            },
            dash = {
                ms_per_frame = 40,
                frames = {"character_0001", "character_0000"},
            }
        },
    }

    -- self.routine = coroutine.create(self.co_update)
end

function Player:update(dt)
    local key_down = keyboard.down
    self.dt = dt

    self.sprite:update(self.dt)

    local spd = self.dt * 4

    self.dx = 0
    self.dz = 0
    if key_down "w" then self.dz = self.dz - 1 end
    if key_down "s" then self.dz = self.dz + 1 end
    if key_down "a" then self.dx = self.dx - 1 end
    if key_down "d" then self.dx = self.dx + 1 end
    self.dx, self.dz = vec2.normalize(self.dx, self.dz)

    if self.dx > 0 then
        self.facing_right = true
    elseif self.dx < 0 then
        self.facing_right = false
    end

    self.x = self.x + self.dx * spd
    self.z = self.z + self.dz * spd

    -- local ok, err = coroutine.resume(self.routine, self)
    -- assert(ok, err)
end

function Player:co_update()
    local key_down = keyboard.down

    ::before_idle::
    self.sprite:play "idle"
    ::idle:: do
        self.dx = 0
        self.dz = 0
        if key_down "w" then self.dz = self.dz - 1 end
        if key_down "s" then self.dz = self.dz + 1 end
        if key_down "a" then self.dx = self.dx - 1 end
        if key_down "d" then self.dx = self.dx + 1 end

        if self.dx ~= 0 or self.dz ~= 0 then
            coroutine.yield()
            goto before_walk
        end
        coroutine.yield()
        goto idle
    end

    ::before_walk::
    self.sprite:play "walk"
    ::walk:: do
        local spd = self.dt * 4

        self.dx = 0
        self.dz = 0
        if key_down "w" then self.dz = self.dz - 1 end
        if key_down "s" then self.dz = self.dz + 1 end
        if key_down "a" then self.dx = self.dx - 1 end
        if key_down "d" then self.dx = self.dx + 1 end
        self.dx, self.dz = vec2.normalize(self.dx, self.dz)

        if self.dx == 0 and self.dz == 0 then
            coroutine.yield()
            goto before_idle
        elseif keyboard.pressed "lshift" then
            coroutine.yield()
            goto before_dash
        end

        if self.dx > 0 then
            self.facing_right = true
        elseif self.dx < 0 then
            self.facing_right = false
        end

        self.x = self.x + self.dx * spd
        self.z = self.z + self.dz * spd

        coroutine.yield()
        goto walk
    end

    ::before_dash::
    self.sprite:play "dash"
    ::dash:: do
        self.dashing = true
        local dash_time = 0.2
        while dash_time > 0 do
            local spd = self.dt * 12
            self.x = self.x + self.dx * spd
            self.z = self.z + self.dz * spd
            dash_time = dash_time - self.dt
            coroutine.yield()
        end
        self.dashing = false
        goto before_idle
    end

    error "player update function died"
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

    local r, g, b, a
    if self.dashing then
        r, g, b, a = 200, 200, 255, 255
    else
        r, g, b, a = 255, 255, 255, 255
    end

    gfx.bind_texture(self.sprite.atlas.texture.id)
    gfx.v3_t2_c4_f4(x1, y1, z1, u1, v1, r, g, b, a, 0, 0, 0, 0)
    gfx.v3_t2_c4_f4(x1, y2, z2, u1, v2, r, g, b, a, 0, 0, 0, 0)
    gfx.v3_t2_c4_f4(x2, y2, z2, u2, v2, r, g, b, a, 0, 0, 0, 0)
    gfx.v3_t2_c4_f4(x2, y1, z1, u2, v1, r, g, b, a, 0, 0, 0, 0)
end

return Player
