local vec2 = {}

function vec2.normalize(x, y)
    local len = math.sqrt(x * x + y * y)
    if len == 0 then
        return 0, 0
    end

    return x / len, y / len
end

return {vec2 = vec2}
