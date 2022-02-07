local Spring = require "spring"
local Progress = require "progress"
local Corpse = require "entities.corpse"

local enemy_common = {}

function enemy_common.init(entity, desc)
    entity.hit_spring = Spring()
    entity.p_hit = Progress(0.08)
    entity.p_hit.time = 1 / 0
    entity.health = desc.health
end

function enemy_common.update(entity, desc)
    local dt = desc.dt

    entity.hit_spring:update(dt)
    entity.p_hit:update(dt)

    local bullets = entity.group:nearby_classname("Bullet", entity.x, entity.y + 0.5, entity.z, desc.collision_distance)
    if next(bullets) then
        entity.health = entity.health - 1
        entity.hit_spring:pull(0.2)
        entity.p_hit.time = 0
        for _, bullet in pairs(bullets) do
            bullet.dead = true
        end
    end

    if entity.health <= 0 then
        entity.dead = true
    end
end

function enemy_common.on_death(entity)
    snd.play "data/content/death.wav"

    entity.group:add(Corpse, {
        x = entity.x,
        y = entity.y,
        z = entity.z,
        uv = entity.sprite:uv(),
    })
end

function enemy_common.draw(entity)
    local x1 = entity.x - 0.5
    local y1 = entity.y + 1.2
    local z1 = entity.z
    local x2 = entity.x + 0.5
    local y2 = entity.y
    local z2 = entity.z

    x1 = x1 - entity.hit_spring.x
    x2 = x2 + entity.hit_spring.x
    y1 = y1 - entity.hit_spring.x
    y2 = y2 + entity.hit_spring.x

    local uv = entity.sprite:uv()
    local u1 = entity.dx < 0 and uv.u1 or uv.u2
    local v1 = uv.v1
    local u2 = entity.dx < 0 and uv.u2 or uv.u1
    local v2 = uv.v2

    local p = 1 - math.min(1, entity.p_hit:percent())
    p = math.floor(p * 255)

    gfx.bind_texture(entity.sprite.atlas.texture.id)
    gfx.v3_t2_f4(x1, y1, z1, u1, v1, p, p, p, p)
    gfx.v3_t2_f4(x1, y2, z2, u1, v2, p, p, p, p)
    gfx.v3_t2_f4(x2, y2, z2, u2, v2, p, p, p, p)
    gfx.v3_t2_f4(x2, y1, z1, u2, v1, p, p, p, p)
end

return enemy_common
