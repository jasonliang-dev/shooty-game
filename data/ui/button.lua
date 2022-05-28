local class = require "class"
local mouse = require "mouse"
local flux = require "deps.flux"
local Spring = require "spring"

local core = _G["core"]

local Button = class()

local noop = function() end

function Button:new(desc)
    self.font = desc.font
    self.text = desc.text
    self.w = desc.w or self.font:width(self.text)
    self.h = self.font.height
    self.x = desc.x - self.w / 2
    self.y = desc.y - self.h / 2
    self.baseline = self.font.baseline
    self.color = desc.color
    self.on_click = desc.on_click or noop
    self.on_update = desc.on_update or noop
    self.hot = false
    self.pad_x = {val = 15, def = 15}
    self.pad_y = {val = 8}
    self.off_y_spring = Spring()
end

function Button:update(dt)
    self.on_update(self, dt)
    self.off_y_spring:update(dt)

    self.text_x = self.x + (self.w - self.font:width(self.text)) / 2

    local x1 = self.x - self.pad_x.val
    local y1 = self.y - self.pad_y.val
    local x2 = self.x + self.w + self.pad_x.val
    local y2 = self.y + self.h + self.pad_y.val

    local hot = mouse.x > x1 and mouse.x < x2
        and mouse.y > y1 and mouse.y < y2

    if hot and not self.hot then
        flux.to(self.pad_x, 0.6, {val = 25}):ease "elasticout"
        self.off_y_spring:pull(1)
    elseif not hot and self.hot then
        flux.to(self.pad_x, 1, {val = self.pad_x.def}):ease "elasticout"
    end

    self.hot = hot

    if mouse.clicked "left" and hot then
        self.on_click()
    end
end

function Button:draw()
    local x1 = self.x - self.pad_x.val
    local y1 = self.y - self.pad_y.val
    local x2 = self.x + self.w + self.pad_x.val
    local y2 = self.y + self.h + self.pad_y.val

    local shr = 0
    local r, g, b, a = table.unpack(self.color)
    core.gfx.bind_white_texture()
    core.gfx.v3_t2_c4(x1,       y1, 0, 0, 0, r, g, b, a)
    core.gfx.v3_t2_c4(x1 - shr, y2, 0, 0, 0, r, g, b, a)
    core.gfx.v3_t2_c4(x2,       y2, 0, 0, 0, r, g, b, a)
    core.gfx.v3_t2_c4(x2 + shr, y1, 0, 0, 0, r, g, b, a)

    self.font:print(self.text, self.text_x, self.y - self.off_y_spring.x * 2)
end

return Button
