local class = require "class"

local PriorityQueue = class()

function PriorityQueue:new()
    self.values = {}
    self.costs = {}
end

function PriorityQueue:count()
    return #self.values
end

function PriorityQueue:push_min(value, cost)
    table.insert(self.values, value)
    table.insert(self.costs, cost)

    local i = #self.values
    local j = i // 2
    while i > 1 and self.costs[i] < self.costs[j] do
        self.values[i], self.values[j] = self.values[j], self.values[i]
        self.costs[i], self.costs[j] = self.costs[j], self.costs[i]
        i = i // 2
        j = i // 2
    end
end

function PriorityQueue:pop_min()
    if #self.values == 0 then
        return
    end

    local value, cost = self.values[1], self.costs[1]

    self.values[1] = self.values[#self.values]
    self.costs[1] = self.costs[#self.costs]
    self.values[#self.values] = nil
    self.costs[#self.costs] = nil

    local i = 1
    local j
    while i * 2 <= #self.values do
        if i * 2 + 1 > #self.values then
            j = i * 2
        elseif self.costs[i * 2] < self.costs[i * 2 + 1] then
            j = i * 2
        else
            j = i * 2 + 1
        end

        if self.costs[i] > self.costs[j] then
            self.values[i], self.values[j] = self.values[j], self.values[i]
            self.costs[i], self.costs[j] = self.costs[j], self.costs[i]
        end

        i = j
    end

    return value, cost
end

return PriorityQueue
