local class = require "class"
local mat = require "mat"
local keyboard = require "keyboard"
local EntityGroup = require "entity_group"
local Button = require "ui.button"

local RoomMenu = class()

function RoomMenu:new()
    sys.show_mouse(true)
    default_pass_clear_color = {128, 128, 128, 255}

    self.group = EntityGroup()

    local font = fnt_pally
    self.group:add(Button, {
        x = sys.window_width() / 2,
        y = 100,
        w = 200,
        font = font,
        text = "Play",
        color = {0, 200, 0, 180},
        on_click = function() self.rooms.go "test" end,
        on_update = function(self, dt)
            self.x = (sys.window_width() - self.w) / 2
        end,
    })

    self.group:add(Button, {
        x = sys.window_width() / 2,
        y = 200,
        w = 200,
        font = font,
        text = "Options",
        color = {210, 0, 200, 180},
        on_click = function() end,
        on_update = function(self, dt)
            self.x = (sys.window_width() - self.w) / 2
        end,
    })

    if SYS_PLATFORM ~= "html5" then
        self.group:add(Button, {
            x = sys.window_width() / 2,
            y = 300,
            w = 200,
            font = font,
            text = "Quit",
            color = {200, 0, 0, 180},
            on_click = function() sys.quit() end,
            on_update = function(self, dt)
                self.x = (sys.window_width() - self.w) / 2
            end,
        })
    end
end

function RoomMenu:update(dt)
    if SYS_PLATFORM ~= "html5" and keyboard.pressed "escape" then
        sys.quit()
    end

    self.group:update(dt)
end

function RoomMenu:draw()
    gfx.bind_mvp(mat.ortho(0, sys.window_width(), sys.window_height(), 0, -1, 1))
    self.group:draw()
end

return RoomMenu
