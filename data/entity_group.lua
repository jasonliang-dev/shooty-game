local class = require "class"

local function noop() end

local EntityGroup = class()

function EntityGroup:new()
    self.next_id = 0
    self.group_by_id = {}
end

function EntityGroup:add(ctor, ...)
    local entity = ctor(...)

    entity.id = self.next_id
    entity.group = self
    entity.dead = false
    entity.on_death = noop

    self.group_by_id[self.next_id] = entity
    self.next_id = self.next_id + 1

    return entity
end

function EntityGroup:get_by_id(id)
    return self.group_by_id[id]
end

function EntityGroup:update(dt)
    for _, entity in pairs(self.group_by_id) do
        entity:update(dt)
    end

    for id, entity in pairs(self.group_by_id) do
        if entity.dead then
            entity:on_death()
            self.group[id] = nil
        end
    end
end

function EntityGroup:draw()
    for _, entity in pairs(self.group_by_id) do
        entity:draw()
    end
end

return EntityGroup
