local translate_button = {
    ["left"] = 0,
    ["right"] = 1,
    ["middle"] = 2,
}

local mouse = {
    prev = {},
    current = {},
    x = 0,
    y = 0,
    dx = 0,
    dy = 0,
}

function mouse.on_mousedown(button)
    mouse.current[button] = true
end

function mouse.on_mouseup(button)
    mouse.current[button] = nil
end

function mouse.on_mousemove(...)
    mouse.x, mouse.y, mouse.dx, mouse.dy = ...
end

function mouse.down(button)
    return mouse.current[translate_button[button]]
end

function mouse.clicked(button)
    local code = translate_button[button]
    return not mouse.prev[code] and mouse.current[code]
end

function mouse.released(button)
    local code = translate_button[button]
    return mouse.prev[code] and not mouse.current[code]
end

function mouse.position()
    return mouse.x, mouse.y
end

function mouse.delta()
    return mouse.dx, mouse.dy
end

function mouse.update()
    for k in pairs(mouse.prev) do
        mouse.prev[k] = nil
    end

    for k in pairs(mouse.current) do
        mouse.prev[k] = true
    end
end

return mouse
