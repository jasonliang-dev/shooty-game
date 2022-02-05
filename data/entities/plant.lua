local class = require "class"
local lume = require "deps.lume"
local Spring = require "spring"

local Plant = class()

function Plant:new(desc)
    self.x = desc.x
    self.z = desc.z
    self.uv = atl_entities:uv(desc.name)
    self.spring = Spring()
end

local function squish(self, entity)
    local d = lume.distance(entity.x, entity.z, self.x, self.z)
    if d < 0.8 and not self.hot then
        self.spring:pull(0.05)
        self.hot = entity.id
    elseif d > 0.8 and self.hot == entity.id then
        self.hot = nil
    end
end

function Plant:update(dt)
    local player = self.group.player
    local enemies = self.group:get_by_classname "BasicEnemy"

    squish(self, player)
    for _, enemy in pairs(enemies) do
        squish(self, enemy)
    end

    if not self.hot then
        self.spring:update(dt)
    end
end

function Plant:draw()
    local x1 = self.x - 0.5 - self.spring.x
    local y1 = 1 - self.spring.x
    local z1 = self.z - 0.5 - self.spring.x
    local x2 = self.x + 0.5 + self.spring.x
    local y2 = 0
    local z2 = self.z + 0.5 + self.spring.x

    local uv = self.uv
    gfx.bind_texture(atl_entities.texture.id)
    gfx.v3_t2(x1, y1, z1, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, z1, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, z2, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, z2, uv.u2, uv.v1)

    gfx.v3_t2(x1, y1, z2, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, z2, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, z1, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, z1, uv.u2, uv.v1)
end

return Plant
