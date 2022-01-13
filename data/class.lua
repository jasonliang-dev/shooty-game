local mt = {
    __call = function(self, ...)
        local inst = setmetatable({}, self)
        inst:new(...)
        return inst
    end
}

local function class()
    obj = {}
    obj.__index = obj
    return setmetatable(obj, mt)
end

return class

