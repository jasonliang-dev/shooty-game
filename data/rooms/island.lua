local class = require "class"
local quat = require "quat"
local mat = require "mat"
local keyboard = require "keyboard"
local Camera = require "camera"
local EntityGroup = require "entity_group"
local Player = require "entities.player"
local Plant = require "entities.plant"
local Crosshair = require "entities.crosshair"

local RoomIsland = class()

function RoomIsland:new()
    sys.show_mouse(false)
    default_pass_clear_color = {128, 128, 128, 255}

    local spawn = tmx_lv2:object_by_type "spawn"
    local tree = tmx_lv2:object_by_type "tree"

    self.camera = Camera(spawn.x, 0, spawn.y, quat.euler(math.pi / 6, 0, 0))

    self.group = EntityGroup()

    self.group:add(Plant, {
        x = tree.x,
        z = tree.y,
        name = "tree",
    })

    self.player = self.group:add(Player, {
        map = tmx_lv2,
        camera = self.camera,
        x = spawn.x,
        z = spawn.y,
        health = 5,
    })

    self.group:add(Crosshair, {
        camera = self.camera,
    })
end

function RoomIsland:update(dt)
    if keyboard.pressed "escape" then
        self.rooms.go "menu"
    end

    self.group:update(dt)

    self.camera.target = {
        x = self.player.x,
        y = 6,
        z = self.player.z + 10
    }
    self.camera:update(dt)
end

function RoomIsland:draw()
    local view_projection = self.camera:view_projection()

    local water_width = 160
    local mvp = mat.chain(
       view_projection,
       mat.translate(-water_width / 2, -0.01, -water_width / 2),
       mat.scale(water_width, 1, water_width),
       mat.axis_rotate(1, 0, 0, math.pi / 2)
    )
    gfx.draw_water(mvp, water_width)

    mvp = mat.mul(mat.axis_rotate(1, 0, 0, math.pi / 2), view_projection)
    tmx_lv2:draw(mvp, tsx_tiles.texture_id)

    gfx.bind_mvp(view_projection)
    self.group:draw()
end

return RoomIsland
