local class = require "class"

local Deque = class()

function Deque:new()
    self.front = 1
    self.back = 0
    self.data = {}
end

function Deque:push_front(v)
    self.front = self.front - 1
    self.data[self.front] = v
end

function Deque:push_back(v)
    self.back = self.back + 1
    self.data[self.back] = v
end

function Deque:pop_front()
    assert(self.front > self.back, "empty deque")
    local v = self.data[self.front]
    self.data[self.front] = nil
    self.front = self.front + 1
    return v
end

function Deque:pop_back()
    assert(self.front > self.back, "empty deque")
    local v = self.data[self.back]
    self.data[self.back] = nil
    self.back = self.back - 1
    return v
end

function Deque:iter()
    local i = self.front
    return function()
        i = i + 1
        return self.data[i - 1]
    end
end

return Deque
