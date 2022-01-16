local class = require "class"

local StateMachine = class()

function StateMachine:new(desc)
    self.owner = desc.owner
    self.current_state = desc.initial
    self.states = desc.states

    local enter = self.states[self.current_state].enter
    if enter then
        enter(self.owner)
    end
end

function StateMachine:transition(to)
    local leave = self.states[self.current_state].leave
    if leave then
        leave(self.owner)
    end

    self.next_state = to
end

function StateMachine:dispatch(dt)
    if self.next_state then
        self.current_state = self.next_state
        self.next_state = nil
        local enter = self.states[self.current_state].enter
        if enter then
            enter(self.owner)
        end
    end

    self.states[self.current_state].update(self.owner, dt)
end

function StateMachine:state()
    return self.current_state
end

return StateMachine
