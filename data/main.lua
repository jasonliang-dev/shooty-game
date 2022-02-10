local rooms = require "rooms"
local mat = require "mat"
local keyboard = require "keyboard"
local mouse = require "mouse"
local flux = require "deps.flux"
local Atlas = require "atlas"

function core.init()
    SYS_PLATFORM = sys.platform()
    math.randomseed(0, 0)

    snd.master_volume(0.2)
    default_pass_clear_color = {0, 0, 0, 255}

    atl_tiles = assert(Atlas "data/content/tiles.rtpa")
    atl_entities = assert(Atlas "data/content/entities.rtpa")
    fnt_normal = assert(aux.make_font("data/content/times.ttf", 32))
    fnt_small = assert(aux.make_font("data/content/times.ttf", 20))
    fnt_pally = assert(aux.make_font("data/content/Pally-Bold.ttf", 40))

    tsx_tiles = aux.make_tileset "data/content/tiles.json"
    tmx_lv1 = aux.make_tilemap("data/content/lv1.json", tsx_tiles)
    tmx_lv2 = aux.make_tilemap("data/content/lv2.json", tsx_tiles)

    rooms.start("island", {
        test = require "rooms.test",
        island = require "rooms.island",
        menu = require "rooms.menu",
    })
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
    flux.update(dt)
    rooms.current:update(dt)

    local draws_last_frame = gfx.draw_count()
    gfx.begin_draw {
        color = default_pass_clear_color,
    }

    rooms.current:draw()

    local r, g, b, a = 0, 0, 0, 255
    gfx.bind_mvp(mat.ortho(0, sys.window_width(), sys.window_height(), 0, -1, 1))
    fnt_small:print(string.format("FPS: %.2f", 1 / dt), 10, 8, r, g, b, a)
    fnt_small:print(string.format("Delta Time: %.2fms", dt * 1000), 10, 28, r, g, b, a)
    fnt_small:print(string.format("Draws: %d", draws_last_frame), 10, 48, r, g, b, a)

    gfx.end_draw()

    rooms.transition()
    keyboard.update()
    mouse.update()
end
