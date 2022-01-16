local rooms = require "rooms"
local mat = require "mat"
local keyboard = require "keyboard"
local Atlas = require "atlas"
local Tilemap = require "tilemap"

function core.init()
    atl_characters = assert(Atlas "data/content/characters.rtpa")
    atl_plants = assert(Atlas "data/content/plants.rtpa")
    map_test = assert(Tilemap "data/content/map.json")
    fnt_normal = assert(gfx.make_font("data/content/times.ttf", 32))
    fnt_small = assert(gfx.make_font("data/content/times.ttf", 20))

    rooms.start "test"
end

function core.event(type, ...)
    if type == "keydown" then
        keyboard.on_keydown(...)
    elseif type == "keyup" then
        keyboard.on_keyup(...)
    end
end

function core.frame(dt)
    rooms.current:update(dt)

    local draws_last_frame = gfx.draw_count()
    gfx.begin_draw()
    rooms.current:draw()

    gfx.bind_mvp(mat.ortho(0, sys.window_width(), sys.window_height(), 0, -1, 1))
    fnt_small:print(string.format("FPS: %.2f", 1 / dt), 10, 20)
    fnt_small:print(string.format("Draws: %d", draws_last_frame), 10, 40)

    gfx.end_draw()

    rooms.transition()
    keyboard.update()
end
