local json = require "deps.json"
local class = require "class"

local Tilemap = class()

function Tilemap:new(filename)
    self.tiles = {}

    local handle = assert(io.open(filename))
    local export = json.decode(handle:read "*a")

    assert(#export.tilesets == 1)
    local tileset = export.tilesets[1]

    local path = filename:match "(.*/)"
    local image = tileset.image
    self.texture = gfx.make_texture(path .. image)

    for _, layer in ipairs(export.layers) do
        if layer.type ~= "tilelayer" then
            goto next_layer
        end

        for y = 1, export.height do
            for x = 1, export.width do
                local index = layer.data[((y - 1) * export.width) + x]
                if index == 0 then
                    goto next_tile
                end

                -- unset flags?
                local id = index - tileset.firstgid

                local atlas_x = id % tileset.columns
                local atlas_y = id // tileset.columns

                local inset = 0.003

                table.insert(self.tiles, {
                    x = x,
                    y = y,
                    u1 = inset + (tileset.margin + (atlas_x * tileset.spacing) + (atlas_x * export.tilewidth)) / tileset.imagewidth,
                    v1 = inset + (tileset.margin + (atlas_y * tileset.spacing) + (atlas_y * export.tileheight)) / tileset.imageheight,
                    u2 = -inset + (tileset.margin + (atlas_x * tileset.spacing) + ((atlas_x + 1) * export.tilewidth)) / tileset.imagewidth,
                    v2 = -inset + (tileset.margin + (atlas_y * tileset.spacing) + ((atlas_y + 1) * export.tileheight)) / tileset.imageheight,
                })

                ::next_tile::
            end
        end

        ::next_layer::
    end

    self.width = export.width
    self.height = export.height

    handle:close()
end

return Tilemap
