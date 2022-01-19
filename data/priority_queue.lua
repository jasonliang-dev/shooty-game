local class = require "class"

local PriorityQueue = class()

function PriorityQueue:new()
    self.values = {}
    self.costs = {}
end

function PriorityQueue:push_min(value, cost)
    table.insert(self.values, value)
    table.insert(self.costs, cost)

    local i = #self.values
    while i // 2 > 0 do
        local j = i // 2
        if self.costs[i] < self.costs[j] then
            self.values[i], self.values[j] = self.values[j], self.values[i]
            self.costs[i], self.costs[j] = self.costs[j], self.costs[i]
        end
        i = i // 2
    end
end

function PriorityQueue:pop_min()
    local value, cost = self.values[1], self.costs[1]

    self.values[1] = self.values[#self.values]
    self.costs[1] = self.costs[#self.costs]
    self.values[#self.values] = nil
    self.costs[#self.costs] = nil

    local i = 1
    while i * 2 <= #self.values do
        local j
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
