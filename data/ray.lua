local vec3 = require "vec".vec3

local ray = {}

function ray.from_screen(x, y, camera)

end

-- moller trumbore intersection algorithm
-- https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
function ray.vs_triangle(desc)
    local origin = desc.origin
    local direction = desc.direction
    local v1 = desc.v1
    local v2 = desc.v2
    local v3 = desc.v3

    local edge1 = vec3.sub(v2, v1)
    local edge2 = vec3.sub(v3, v1)

    local h = vec3.cross(direction, edge2)
    local a = vec3.dot(edge1, h)

    local epsilon = 0.0000001
    if a > -epsilon and a < epsilon then
        return -- culling
    end

    local f = 1 / a
    local s = vec3.sub(origin, v1)
    local u = f * vec3.dot(s, h)
    if u < 0 or u > 1 then
        return
    end

    local q = vec3.cross(s, edge1)
    local v = f * vec3.dot(direction, q)
    if v < 0 or u + v > 1 then
        return
    end

    local t = f * vec3.dot(edge2, q)
    if t <= epsilon then
        return
    end

    return {
        distance = t,
        point = vec3.add(origin, vec3.scale(direction, t))
    }
end

return ray
