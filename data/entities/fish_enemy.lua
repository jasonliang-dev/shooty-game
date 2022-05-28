local class = require "class"
local enemy_common = require "enemy_common"
local lume = require "deps.lume"
local Sprite = require "sprite"

local core = _G["core"]

local FishBullet = class()
FishBullet.classname = "FishBullet"

function FishBullet:new(desc)
    self.x = desc.x
    self.y = desc.y
    self.z = desc.z
    self.dx = desc.dx
    self.dy = desc.dy
    self.dz = desc.dz
end

function FishBullet:update(dt)
    local speed = 4
    self.x = self.x + self.dx * speed * dt
    self.y = self.y + self.dy * speed * dt
    self.z = self.z + self.dz * speed * dt

    enemy_common.player_collision(self, 1)

    if self.y < 0 then
        self.dead = true
    end
end

function FishBullet:draw()
    local x1 = self.x - 0.5
    local y1 = self.y + 0.5
    local z1 = self.z
    local x2 = self.x + 0.5
    local y2 = self.y - 0.5
    local z2 = self.z

    local uv = ATL_ENTITIES:uv "bullet_2"

    core.gfx.bind_texture(ATL_ENTITIES.texture.id)
    core.gfx.v3_t2(x1, y1, z1, uv.u1, uv.v1)
    core.gfx.v3_t2(x1, y2, z2, uv.u1, uv.v2)
    core.gfx.v3_t2(x2, y2, z2, uv.u2, uv.v2)
    core.gfx.v3_t2(x2, y1, z1, uv.u2, uv.v1)

    x1 = self.x - 0.2
    z1 = self.z - 0.2
    x2 = self.x + 0.2
    z2 = self.z + 0.2
    uv = ATL_ENTITIES:uv "circle_small"

    core.gfx.v3_t2_c4(x1, 0.01, z1, uv.u1, uv.v1, 240, 240, 255, 32)
    core.gfx.v3_t2_c4(x1, 0.01, z2, uv.u1, uv.v2, 240, 240, 255, 32)
    core.gfx.v3_t2_c4(x2, 0.01, z2, uv.u2, uv.v2, 240, 240, 255, 32)
    core.gfx.v3_t2_c4(x2, 0.01, z1, uv.u2, uv.v1, 240, 240, 255, 32)
end

local FishEnemy = class()
FishEnemy.classname = "FishEnemy"

function FishEnemy:new(desc)
    enemy_common.init(self, desc)

    self.x = desc.x
    self.y = 1 / 0
    self.z = desc.z
    self.dy = 0
    self.time_y = math.random() * math.pi
    self.apex_count = 0

    self.sprite = Sprite {
        atlas = ATL_ENTITIES,
        initial = "default",
        animations = {
            default = {ms_per_frame = 150, frames = {"fish_1", "fish_2"}},
        }
    }
end

function FishEnemy:update(dt)
    self.sprite:update(dt)

    self.time_y = self.time_y + dt
    local y = self.y
    self.y = math.cos(self.time_y * 3)

    enemy_common.update(self, {
        dt = dt,
        collision_distance = 0.8,
    })

    local dy = self.y - y
    if self.dy > dy and lume.sign(dy) ~= lume.sign(self.dy) then
        self.apex_count = self.apex_count + 1
        if self.apex_count == 2 then
            self.apex_count = 0

            local xx = self.group.player.x - self.x
            local yy = self.group.player.y - self.y + 0.5
            local zz = self.group.player.z - self.z
            local norm = math.sqrt(xx * xx + yy * yy + zz * zz)
            self.group:add(FishBullet, {
                x = self.x,
                y = self.y,
                z = self.z,
                dx = xx / norm,
                dy = yy / norm,
                dz = zz / norm,
            })
        end
    end
    self.dy = dy

end

function FishEnemy:on_death()
    enemy_common.on_death(self)
end

function FishEnemy:draw()
    enemy_common.draw(self)
end

return FishEnemy
