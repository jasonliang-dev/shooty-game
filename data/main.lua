local rooms = require "rooms"
local mat = require "mat"
local keyboard = require "keyboard"
local mouse = require "mouse"
local flux = require "deps.flux"
local Atlas = require "atlas"

local core = _G["core"]

function core.init()
    SYS_PLATFORM = core.sys.platform()
    math.randomseed(0, 0)

    core.snd.master_volume(0.2)
    DEFAULT_PASS_CLEAR_COLOR = {0, 0, 0, 255}

    ATL_TILES = assert(Atlas "data/content/tiles.rtpa")
    ATL_ENTITIES = assert(Atlas "data/content/entities.rtpa")
    FNT_NORMAL = assert(core.aux.make_font("data/content/times.ttf", 32))
    FNT_SMALL = assert(core.aux.make_font("data/content/times.ttf", 20))
    FNT_PALLY = assert(core.aux.make_font("data/content/Pally-Bold.ttf", 40))

    TSX_TILES = core.aux.make_tileset "data/content/tiles.json"
    TMX_LV1 = core.aux.make_tilemap("data/content/lv1.json", TSX_TILES)
    TMX_LV2 = core.aux.make_tilemap("data/content/lv2.json", TSX_TILES)

    rooms.start("test", {
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

    local draws_last_frame = core.gfx.draw_count()
    core.gfx.begin_draw {
        color = DEFAULT_PASS_CLEAR_COLOR,
    }

    rooms.current:draw()

    local r, g, b, a = 0, 0, 0, 255
    local w, h = core.sys.window_width(), core.sys.window_height()
    core.gfx.bind_mvp(mat.ortho(0, w, h, 0, -1, 1))
    FNT_SMALL:print(string.format("FPS: %.2f", 1 / dt), 10, h - 28, r, g, b, a)
    FNT_SMALL:print(string.format("Delta Time: %.2fms", dt * 1000), 10, h - 48, r, g, b, a)
    FNT_SMALL:print(string.format("Draws: %d", draws_last_frame), 10, h - 68, r, g, b, a)

    core.gfx.end_draw()

    rooms.transition()
    keyboard.update()
    mouse.update()
end
