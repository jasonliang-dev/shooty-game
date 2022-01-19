local rooms = require "rooms"
local mat = require "mat"
local vec = require "vec"
local keyboard = require "keyboard"
local mouse = require "mouse"
local Atlas = require "atlas"
local Tilemap = require "tilemap"
local PriorityQueue = require "priority_queue"

local vec3 = vec.vec3

function core.init()
    atl_tiles = assert(Atlas "data/content/tiles.rtpa")
    atl_entities = assert(Atlas "data/content/entities.rtpa")
    map_test = assert(Tilemap "data/content/map.json")
    fnt_normal = assert(gfx.make_font("data/content/times.ttf", 32))
    fnt_small = assert(gfx.make_font("data/content/times.ttf", 20))

    rooms.start "test"

    local pq = PriorityQueue()
    pq:push_min(0, 0)
    pq:push_min(1, 1)
    pq:push_min(5, 5)
    pq:push_min(9, 9)
    pq:push_min(3, 3)
    pq:push_min(2, 2)
    pq:push_min(4, 4)

    local v
    repeat
        v = pq:pop_min()
        print(v)
    until v == nil
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
    mouse.update()
end
