local vec2 = {}
local vec3 = {}

function vec2.normalize(x, y)
    local len = math.sqrt(x * x + y * y)
    if len == 0 then
        return 0, 0
    end

    return x / len, y / len
end

function vec2.rotate(rad, x, y, ox, oy)
    local dx = x - (ox or 0)
    local dy = y - (oy or 0)
    local rx = dx * math.cos(rad) - dy * math.sin(rad)
    local ry = dx * math.sin(rad) + dy * math.cos(rad)
    return ox + rx, oy + ry
end

function vec3.unpack(tab)
    return tab.x, tab.y, tab.z
end

return {
    vec2 = vec2,
    vec3 = vec3,
}
