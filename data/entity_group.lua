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
    return self.group_by_classname[classname]
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

        for _, group in pairs(self.group_by_classname) do
            for id2 in pairs(group) do
                if id2 == id then
                    group[id2] = nil
                end
            end
        end

        self.group_by_id[id] = nil

        ::continue::
    end
end

function EntityGroup:draw()
    for _, entity in pairs(self.group_by_id) do
        entity:draw()
    end
end

return EntityGroup
