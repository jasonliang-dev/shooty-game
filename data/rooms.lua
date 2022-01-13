local rooms = {
    list = {
        test = require "rooms.test",
        test2 = require "rooms.test2",
    },
}

function rooms.start(name)
    for _, r in pairs(rooms.list) do
        r.rooms = rooms
    end

    rooms.current = assert(rooms.list[name])()
end

function rooms.go(name)
    rooms.next = rooms.list[name]
end

function rooms.transition()
    if rooms.next ~= nil then
        rooms.current = rooms.next()
        rooms.next = nil
    end
end

return rooms
