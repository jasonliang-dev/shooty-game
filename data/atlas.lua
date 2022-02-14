local class = require "class"

local Atlas = class()

function Atlas:new(filename)
    self.sub_textures = {}
    local count = 0

    local handle = assert(io.open(filename))

    while true do
        local line = handle:read "*line"
        if not line then
            break
        end

        if line:sub(1, 1) == "a" then
            local path = filename:match "(.*/)"
            local image = line:match "^a ([^%s]+) "
            self.texture = aux.make_texture(path .. image)
        elseif line:sub(1, 1) == "s" then
            local name, _, _, x, y, w, h, pad, trim, _, _, tw, th =
                line:match "^s ([^%s]+) (%d+) (%d+) (%d+) (%d+) (%d+) (%d+) (%d+) (%d+) (%d+) (%d+) (%d+) (%d+)"

            if trim == "1" then
                self.sub_textures[name] = {
                    u1 = x + pad,
                    v1 = y + pad,
                    u2 = x + pad + tw,
                    v2 = y + pad + th,
                }
            else
                self.sub_textures[name] = {
                    u1 = x + pad,
                    v1 = y + pad,
                    u2 = x + pad + w,
                    v2 = y + pad + h,
                }
            end

            count = count + 1
        end
    end

    handle:close()
    print(string.format("Loaded atlas '%s' with %d subtextures", filename, count))
end

function Atlas:uv(name)
    local width = self.texture.width
    local height = self.texture.height
    local sub = self.sub_textures[name]

    return {
        u1 = sub.u1 / width,
        v1 = sub.v1 / height,
        u2 = sub.u2 / width,
        v2 = sub.v2 / height,
    }
end

return Atlas
