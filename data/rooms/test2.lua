local mat = require "mat"
local keyboard = require "keyboard"
local class = require "class"

local RoomTest2 = class()

function RoomTest2:new()
end

function RoomTest2:update(dt)
    if keyboard.down "p" then
        self.rooms.go "test"
    end
end

function RoomTest2:draw()
    local view = mat.mul(mat.translate(-5, -10, -20), mat.rotate(1, 0, 0, math.pi / 4))
    local projection = mat.perspective(math.pi / 4, sys.window_width() / sys.window_height(), 0.05, 1000)
    gfx.bind_mvp(mat.mul(view, projection))

    map_test:draw()
end

return RoomTest2
