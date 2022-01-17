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

function mat.translate(x, y, z)
    local m = mat.identity()
    m[13] = x
    m[14] = y
    m[15] = z
    return m
end

function mat.rotate(x, y, z, a)
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
