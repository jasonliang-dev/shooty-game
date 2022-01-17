local mat = require "mat"
local quat = require "quat"

local vec2 = {}
local vec3 = {}

function vec2.normalize(x, y)
    local len = math.sqrt(x * x + y * y)
    return x / len, y / len
end

function vec2.rotate(rad, x, y, ox, oy)
    local dx = x - (ox or 0)
    local dy = y - (oy or 0)
    local rx = dx * math.cos(rad) - dy * math.sin(rad)
    local ry = dx * math.sin(rad) + dy * math.cos(rad)
    return ox + rx, oy + ry
end

function vec3.normalize(v)
    local len = math.sqrt(v.x * v.x + v.y * v.y + v.z * v.z)
    return {
        x = v.x / len,
        y = v.y / len,
        z = v.z / len,
    }
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

function vec3.unproject(v, projection)
    local inv = mat.invert(projection)
    local transform = quat.transform({x = v.x, y = v.y, z = v.z, w = 1}, inv)
    return {
        x = transform.x / transform.w,
        y = transform.y / transform.w,
        z = transform.z / transform.w,
    }
end

return {
    vec2 = vec2,
    vec3 = vec3,
}
