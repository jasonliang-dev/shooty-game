local mat = {}

function mat.identity()
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    }
end

function mat.mul(a, b)
    return {
        a[01] * b[01] + a[02] * b[05] + a[03] * b[09] + a[04] * b[13],
        a[01] * b[02] + a[02] * b[06] + a[03] * b[10] + a[04] * b[14],
        a[01] * b[03] + a[02] * b[07] + a[03] * b[11] + a[04] * b[15],
        a[01] * b[04] + a[02] * b[08] + a[03] * b[12] + a[04] * b[16],
        a[05] * b[01] + a[06] * b[05] + a[07] * b[09] + a[08] * b[13],
        a[05] * b[02] + a[06] * b[06] + a[07] * b[10] + a[08] * b[14],
        a[05] * b[03] + a[06] * b[07] + a[07] * b[11] + a[08] * b[15],
        a[05] * b[04] + a[06] * b[08] + a[07] * b[12] + a[08] * b[16],
        a[09] * b[01] + a[10] * b[05] + a[11] * b[09] + a[12] * b[13],
        a[09] * b[02] + a[10] * b[06] + a[11] * b[10] + a[12] * b[14],
        a[09] * b[03] + a[10] * b[07] + a[11] * b[11] + a[12] * b[15],
        a[09] * b[04] + a[10] * b[08] + a[11] * b[12] + a[12] * b[16],
        a[13] * b[01] + a[14] * b[05] + a[15] * b[09] + a[16] * b[13],
        a[13] * b[02] + a[14] * b[06] + a[15] * b[10] + a[16] * b[14],
        a[13] * b[03] + a[14] * b[07] + a[15] * b[11] + a[16] * b[15],
        a[13] * b[04] + a[14] * b[08] + a[15] * b[12] + a[16] * b[16],
    }
end

function mat.invert(a)
    local b01 = a[01] * a[06] - a[02] * a[05]
    local b02 = a[01] * a[07] - a[03] * a[05]
    local b03 = a[01] * a[08] - a[04] * a[05]
    local b04 = a[02] * a[07] - a[03] * a[06]
    local b05 = a[02] * a[08] - a[04] * a[06]
    local b06 = a[03] * a[08] - a[04] * a[07]
    local b07 = a[09] * a[14] - a[10] * a[13]
    local b08 = a[09] * a[15] - a[11] * a[13]
    local b09 = a[09] * a[16] - a[12] * a[13]
    local b10 = a[10] * a[15] - a[11] * a[14]
    local b11 = a[10] * a[16] - a[12] * a[14]
    local b12 = a[11] * a[16] - a[12] * a[15]

    local idet = 1 / (b01 * b12 - b02 * b11 + b03 * b10 + b04 * b09 - b05 * b08 + b06 * b07)

    local m = mat.identity()
    m[01] = (a[06] * b12 - a[07] * b11 + a[08] * b10) * idet
    m[02] = (-a[02] * b12 + a[03] * b11 - a[04] * b10) * idet
    m[03] = (a[14] * b06 - a[15] * b05 + a[16] * b04) * idet
    m[04] = (-a[10] * b06 + a[11] * b05 - a[12] * b04) * idet
    m[05] = (-a[05] * b12 + a[07] * b09 - a[08] * b08) * idet
    m[06] = (a[01] * b12 - a[03] * b09 + a[04] * b08) * idet
    m[07] = (-a[13] * b06 + a[15] * b03 - a[16] * b02) * idet
    m[08] = (a[09] * b06 - a[11] * b03 + a[12] * b02) * idet
    m[09] = (a[05] * b11 - a[06] * b09 + a[08] * b07) * idet
    m[10] = (-a[01] * b11 + a[02] * b09 - a[04] * b07) * idet
    m[11] = (a[13] * b05 - a[14] * b03 + a[16] * b01) * idet
    m[12] = (-a[09] * b05 + a[10] * b03 - a[12] * b01) * idet
    m[13] = (-a[05] * b10 + a[06] * b08 - a[07] * b07) * idet
    m[14] = (a[01] * b10 - a[02] * b08 + a[03] * b07) * idet
    m[15] = (-a[13] * b04 + a[14] * b02 - a[15] * b01) * idet
    m[16] = (a[09] * b04 - a[10] * b02 + a[11] * b01) * idet
    return m
end

function mat.translate(x, y, z)
    local m = mat.identity()
    m[13] = x
    m[14] = y
    m[15] = z
    return m
end

function mat.axis_rotate(x, y, z, a)
    local m = mat.identity()

    local sin = math.sin(a)
    local cos = math.cos(a)
    local t = 1.0 - cos

    m[1] = x * x * t + cos
    m[2] = y * x * t + z * sin
    m[3] = z * x * t - y * sin
    m[4] = 0

    m[5] = x * y * t - z * sin
    m[6] = y * y * t + cos
    m[7] = z * y * t + x * sin
    m[8] = 0

    m[9] = x * z * t + y * sin
    m[10] = y * z * t - x * sin
    m[11] = z * z * t + cos
    m[12] = 0

    m[13] = 0
    m[14] = 0
    m[15] = 0
    m[16] = 1

    return m
end

function mat.rotate(q)
    local m = mat.identity()
    m[1] = 1 - 2 * (q.y * q.y + q.z * q.z)
    m[2] = 2 * (q.x * q.y + q.w * q.z)
    m[3] = 2 * (q.x * q.z - q.w * q.y)
    m[5] = 2 * (q.x * q.y - q.w * q.z)
    m[6] = 1 - 2 * (q.x * q.x + q.z * q.z)
    m[7] = 2 * (q.y * q.z + q.w * q.x)
    m[9] = 2 * (q.x * q.z + q.w * q.y)
    m[10] = 2 * (q.y * q.z - q.w * q.x)
    m[11] = 1 - 2 * (q.x * q.x + q.y * q.y)
    m[16] = 1
    return m
end

function mat.ortho(left, right, bottom, top, znear, zfar)
    local m = mat.identity()

    m[1] = 2 / (right - left)
    m[6] = 2 / (top - bottom)
    m[11] = -2 / (zfar - znear)
    m[13] = -(right + left) / (right - left)
    m[14] = -(top + bottom) / (top - bottom)
    m[15] = -(zfar + znear) / (zfar - znear)
    m[16] = 1

    return m
end

function mat.perspective(fov, aspect, znear, zfar)
    local m = mat.identity()

    local cot = 1 / math.tan(fov * 0.5)
    m[1] = cot / aspect
    m[6] = cot
    m[11] = (zfar + znear) / (znear - zfar)
    m[12] = -1
    m[15] = 2 * zfar * znear / (znear - zfar)
    m[16] = 1

    return m
end

return mat
