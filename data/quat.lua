local quat = {}

function quat.identity()
    return {x = 0, y = 0, z = 0, w = 1}
end

function quat.normalize(q)
    local len = math.sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w)
    return {
        x = q.x / len,
        y = q.y / len,
        z = q.z / len,
        w = q.w / len,
    }
end

function quat.euler(pitch, yaw, roll)
    local cx = math.cos(pitch / 2)
    local sx = math.sin(pitch / 2)
    local cy = math.cos(yaw / 2)
    local sy = math.sin(yaw / 2)
    local cz = math.cos(roll / 2)
    local sz = math.sin(roll / 2)

    return {
        x = sx * cy * cz - cx * sy * sz,
        y = cx * sy * cz + sx * cy * sz,
        z = cx * cy * sz - sx * sy * cz,
        w = cx * cy * cz + sx * sy * sz,
    }
end

function quat.lerp(a, b, t)
    return {
        x = a.x + (b.x - a.x) * t,
        y = a.y + (b.y - a.y) * t,
        z = a.z + (b.z - a.z) * t,
        w = a.w + (b.w - a.w) * t,
    }
end

function quat.mul(a, b)
    return {
        x = a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y,
        y = a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z,
        z = a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x,
        w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
    }
end

function quat.transform(q, m)
    return {
        x = m[01] * q.x + m[05] * q.y + m[09] * q.z + m[13] * q.w,
        y = m[02] * q.x + m[06] * q.y + m[10] * q.z + m[14] * q.w,
        z = m[03] * q.x + m[07] * q.y + m[11] * q.z + m[15] * q.w,
        w = m[04] * q.x + m[08] * q.y + m[12] * q.z + m[16] * q.w,
    }
end

return quat
