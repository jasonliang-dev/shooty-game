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

function vec3.add(a, b)
    return {
        x = a.x + b.x,
        y = a.y + b.y,
        z = a.z + b.z,
    }
end

function vec3.sub(a, b)
    return {
        x = a.x - b.x,
        y = a.y - b.y,
        z = a.z - b.z,
    }
end

function vec3.scale(v, n)
    return {
        x = v.x * n,
        y = v.y * n,
        z = v.z * n,
    }
end

function vec3.cross(a, b)
    return {
        x = a.y * b.z - a.z * b.y,
        y = a.z * b.x - a.x * b.z,
        z = a.x * b.y - a.y * b.x,
    }
end

function vec3.dot(a, b)
    return a.x * b.x + a.y * b.y + a.z * b.z
end

return {
    vec2 = vec2,
    vec3 = vec3,
}
