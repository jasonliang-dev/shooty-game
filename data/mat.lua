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
    local a01 = a[1]
    local a02 = a[2]
    local a03 = a[3]
    local a04 = a[4]
    local a05 = a[5]
    local a06 = a[6]
    local a07 = a[7]
    local a08 = a[8]
    local a09 = a[9]
    local a10 = a[10]
    local a11 = a[11]
    local a12 = a[12]
    local a13 = a[13]
    local a14 = a[14]
    local a15 = a[15]
    local a16 = a[16]

    local b01 = b[1]
    local b02 = b[2]
    local b03 = b[3]
    local b04 = b[4]
    local b05 = b[5]
    local b06 = b[6]
    local b07 = b[7]
    local b08 = b[8]
    local b09 = b[9]
    local b10 = b[10]
    local b11 = b[11]
    local b12 = b[12]
    local b13 = b[13]
    local b14 = b[14]
    local b15 = b[15]
    local b16 = b[16]

    return {
        a01 * b01 + a02 * b05 + a03 * b09 + a04 * b13,
        a01 * b02 + a02 * b06 + a03 * b10 + a04 * b14,
        a01 * b03 + a02 * b07 + a03 * b11 + a04 * b15,
        a01 * b04 + a02 * b08 + a03 * b12 + a04 * b16,
        a05 * b01 + a06 * b05 + a07 * b09 + a08 * b13,
        a05 * b02 + a06 * b06 + a07 * b10 + a08 * b14,
        a05 * b03 + a06 * b07 + a07 * b11 + a08 * b15,
        a05 * b04 + a06 * b08 + a07 * b12 + a08 * b16,
        a09 * b01 + a10 * b05 + a11 * b09 + a12 * b13,
        a09 * b02 + a10 * b06 + a11 * b10 + a12 * b14,
        a09 * b03 + a10 * b07 + a11 * b11 + a12 * b15,
        a09 * b04 + a10 * b08 + a11 * b12 + a12 * b16,
        a13 * b01 + a14 * b05 + a15 * b09 + a16 * b13,
        a13 * b02 + a14 * b06 + a15 * b10 + a16 * b14,
        a13 * b03 + a14 * b07 + a15 * b11 + a16 * b15,
        a13 * b04 + a14 * b08 + a15 * b12 + a16 * b16,
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
