local class = require "class"

local EntityGroup = class()

function EntityGroup:new()
    self.next_id = 0
    self.group_by_id = {}
    self.group_by_classname = {}
end

function EntityGroup:add(ctor, ...)
    local entity = ctor(...)

    entity.id = self.next_id
    entity.group = self
    entity.dead = false

    self.next_id = self.next_id + 1

    self.group_by_id[entity.id] = entity
    if entity.classname then
        if not self.group_by_classname[entity.classname] then
            self.group_by_classname[entity.classname] = {}
        end

        self.group_by_classname[entity.classname][entity.id] = entity
    end

    if entity.init then
        entity:init()
    end

    return entity
end

function EntityGroup:get_by_id(id)
    return self.group_by_id[id]
end

function EntityGroup:get_by_classname(classname)
    return self.group_by_classname[classname] or {}
end

function EntityGroup:nearby_classname(classname, x, y, z, distance)
    local entities = {}

    local by_classname = self.group_by_classname[classname]
    if not by_classname then
        return entities
    end

    for k, entity in pairs(by_classname) do
        local dx = entity.x - x
        local dy = entity.y - y
        local dz = entity.z - z
        local dist = math.sqrt(dx * dx + dy * dy + dz * dz)
        if dist <= distance then
            entities[k] = entity
        end
    end

    return entities
end

function EntityGroup:update(dt)
    for _, entity in pairs(self.group_by_id) do
        entity:update(dt)
    end

    for id, entity in pairs(self.group_by_id) do
        if not entity.dead then
            goto continue
        end

        if entity.on_death then
            entity:on_death()
        end

        if entity.classname then
            self.group_by_classname[entity.classname][id] = nil
        end

        self.group_by_id[id] = nil

        ::continue::
    end

    for _, entity in pairs(self.group_by_id) do
        if entity.late_update then
            entity:late_update(dt)
        end
    end
end

function EntityGroup:draw()
    for _, entity in pairs(self.group_by_id) do
        entity:draw()
    end
end

return EntityGroup
