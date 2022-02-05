local rooms = require "rooms"
local mat = require "mat"
local vec = require "vec"
local keyboard = require "keyboard"
local mouse = require "mouse"
local lume = require "deps.lume"
local Atlas = require "atlas"

local vec3 = vec.vec3

function core.init()
    atl_tiles = assert(Atlas "data/content/tiles.rtpa")
    atl_entities = assert(Atlas "data/content/entities.rtpa")
    fnt_normal = assert(aux.make_font("data/content/times.ttf", 32))
    fnt_small = assert(aux.make_font("data/content/times.ttf", 20))

    tsx_tiles = aux.make_tileset "data/content/tiles.json"
    tmx_lv1 = aux.make_tilemap("data/content/lv1.json", tsx_tiles)

    rooms.start "test"
end

function core.event(type, ...)
    if type == "keydown" then
        keyboard.on_keydown(...)
    elseif type == "keyup" then
        keyboard.on_keyup(...)
    elseif type == "mousedown" then
        mouse.on_mousedown(...)
    elseif type == "mouseup" then
        mouse.on_mouseup(...)
    elseif type == "mousemove" then
        mouse.on_mousemove(...)
    end
end

function core.frame(dt)
    if keyboard.down "escape" then
        sys.quit()
    end

    rooms.current:update(dt)

    local draws_last_frame = gfx.draw_count()
    gfx.begin_draw()

    rooms.current:draw()

    gfx.bind_mvp(mat.ortho(0, sys.window_width(), sys.window_height(), 0, -1, 1))
    fnt_small:print(string.format("FPS: %.2f", 1 / dt), 10, 20)
    fnt_small:print(string.format("Delta Time: %.2fms", dt * 1000), 10, 40)
    fnt_small:print(string.format("Draws: %d", draws_last_frame), 10, 60)

    gfx.end_draw()

    rooms.transition()
    keyboard.update()
    mouse.update()
end
