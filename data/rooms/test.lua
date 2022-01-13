local mat = require "mat"
local keyboard = require "keyboard"
local class = require "class"
local Player = require "entities.player"

local RoomTest = class()

function RoomTest:new()
    self.player = Player()
end

function RoomTest:update(dt)
    self.player:update(dt)

    if keyboard.down "n" then
        self.rooms.go "test2"
    end
end

function RoomTest:draw()
    local view = mat.mul(mat.translate(-5, -10, -20), mat.rotate(1, 0, 0, math.pi / 4))
    local projection = mat.perspective(math.pi / 4, sys.window_width() / sys.window_height(), 0.05, 1000)
    gfx.bind_mvp(mat.mul(view, projection))

    map_test:draw()
    self.player:draw()

    local plant = {x = 5, z = 4}

    local x1 = plant.x
    local y1 = 1
    local z1 = plant.z - 0.5
    local x2 = plant.x + 1
    local y2 = 0
    local z2 = plant.z

    local uv = atl_plants:uv "tile_0124"
    gfx.bind_texture(atl_plants.texture.id)
    gfx.v3_t2(x1, y1, z1 + 0, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, z2 + 0, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, z2 + 1, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, z1 + 1, uv.u2, uv.v1)

    gfx.v3_t2(x1, y1, z1 + 1, uv.u1, uv.v1)
    gfx.v3_t2(x1, y2, z2 + 1, uv.u1, uv.v2)
    gfx.v3_t2(x2, y2, z2 + 0, uv.u2, uv.v2)
    gfx.v3_t2(x2, y1, z1 + 0, uv.u2, uv.v1)
end

return RoomTest
