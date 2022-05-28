local vec3 = require "vec".vec3

local core = _G["core"]

local ray = {}

function ray.from_screen(x, y, camera)
    local xx = (2 * x) / core.sys.window_width() - 1
    local yy = 1 - (2 * y) / core.sys.window_height()

    local projection = camera:view_projection()
    local vnear = vec3.unproject({x = xx, y = yy, z = 0}, projection)
    local vfar = vec3.unproject({x = xx, y = yy, z = 1}, projection)

    return {
        origin = {x = camera.x, y = camera.y, z = camera.z},
        direction = vec3.normalize(vec3.sub(vfar, vnear))
    }
end

-- moller trumbore intersection algorithm
-- https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
function ray.vs_triangle(desc)
    local origin = desc.ray.origin
    local direction = desc.ray.direction
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

function ray.vs_quad(desc)
    local raycast = ray.vs_triangle {
        ray = desc.ray,
        v1 = desc.v1,
        v2 = desc.v2,
        v3 = desc.v3,
    }

    if not raycast then
        raycast = ray.vs_triangle {
            ray = desc.ray,
            v1 = desc.v1,
            v2 = desc.v3,
            v3 = desc.v4,
        }
    end

    return raycast
end

return ray
