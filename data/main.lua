local mat = require "mat"
local rtpa = require "rtpa"
local tilemap = require "tilemap"
local keyboard = require "keyboard"

function core.init()
    player = {x = 3, z = 3}

    characters = assert(rtpa.load "data/content/characters.rtpa")
    plants = assert(rtpa.load "data/content/plants.rtpa")
    map = assert(tilemap.load "data/content/map.json")
    font = assert(gfx.make_font("data/content/times.ttf", 32))
end

function core.event(type, ...)
    if type == "keydown" then
        keyboard.on_keydown(...)
    elseif type == "keyup" then
        keyboard.on_keyup(...)
    end
end

function core.frame(dt)
    local spd = dt * 4
    if keyboard.down "w" then player.z = player.z - spd end
    if keyboard.down "s" then player.z = player.z + spd end
    if keyboard.down "a" then player.x = player.x - spd end
    if keyboard.down "d" then player.x = player.x + spd end

    gfx.bind_mvp(mat.ortho(0, sys.window_width(), sys.window_height(), 0, -1, 1))
    font:print("Hello World", 100, 100)

    local view = mat.mul(mat.translate(-5, -10, -20), mat.rotate(1, 0, 0, math.pi / 4))
    local projection = mat.perspective(math.pi / 4, sys.window_width() / sys.window_height(), 0.05, 1000)
    gfx.bind_mvp(mat.mul(view, projection))

    gfx.bind_texture(map.texture.id)
    for _, t in pairs(map.tiles) do
        local x1 = t.x
        local y1 = t.y
        local x2 = t.x + 1
        local y2 = t.y + 1

        gfx.v3_t2(x1, 0, y1, t.u1, t.v1)
        gfx.v3_t2(x1, 0, y2, t.u1, t.v2)
        gfx.v3_t2(x2, 0, y2, t.u2, t.v2)
        gfx.v3_t2(x2, 0, y1, t.u2, t.v1)
    end

    local x1 = player.x
    local y1 = 1
    local z1 = player.z - 0.5
    local x2 = player.x + 1
    local y2 = 0
    local z2 = player.z

    local uv = characters:uv "character_0000"
    gfx.bind_texture(characters.texture.id)
    gfx.v3_t2(x1, y1, z1, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, z2, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, z2, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, z1, uv.u2, uv.v1)

    local plant = {x = 5, z = 4}

    local x1 = plant.x
    local y1 = 1
    local z1 = plant.z - 0.5
    local x2 = plant.x + 1
    local y2 = 0
    local z2 = plant.z

    local uv = plants:uv "tile_0124"
    gfx.bind_texture(plants.texture.id)
    gfx.v3_t2(x1, y1, z1 + 0, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, z2 + 0, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, z2 + 1, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, z1 + 1, uv.u2, uv.v1)

    gfx.v3_t2(x1, y1, z1 + 1, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, z2 + 1, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, z2 + 0, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, z1 + 0, uv.u2, uv.v1)
end
