local translate_key = {
    ["space"] = 32,
    ["'"] = 39,
    [","] = 44,
    ["-"] = 45,
    ["."] = 46,
    ["/"] = 47,
    ["0"] = 48,
    ["1"] = 49,
    ["2"] = 50,
    ["3"] = 51,
    ["4"] = 52,
    ["5"] = 53,
    ["6"] = 54,
    ["7"] = 55,
    ["8"] = 56,
    ["9"] = 57,
    [";"] = 59,
    ["="] = 61,
    ["a"] = 65,
    ["b"] = 66,
    ["c"] = 67,
    ["d"] = 68,
    ["e"] = 69,
    ["f"] = 70,
    ["g"] = 71,
    ["h"] = 72,
    ["i"] = 73,
    ["j"] = 74,
    ["k"] = 75,
    ["l"] = 76,
    ["m"] = 77,
    ["n"] = 78,
    ["o"] = 79,
    ["p"] = 80,
    ["q"] = 81,
    ["r"] = 82,
    ["s"] = 83,
    ["t"] = 84,
    ["u"] = 85,
    ["v"] = 86,
    ["w"] = 87,
    ["x"] = 88,
    ["y"] = 89,
    ["z"] = 90,
    ["["] = 91,
    ["\\"] = 92,
    ["]"] = 93,
    ["`"] = 96,
    ["world_1"] = 161,
    ["world_2"] = 162,
    ["escape"] = 256,
    ["enter"] = 257,
    ["tab"] = 258,
    ["backspace"] = 259,
    ["insert"] = 260,
    ["delete"] = 261,
    ["right"] = 262,
    ["left"] = 263,
    ["down"] = 264,
    ["up"] = 265,
    ["page_up"] = 266,
    ["page_down"] = 267,
    ["home"] = 268,
    ["end"] = 269,
    ["caps_lock"] = 280,
    ["scroll_lock"] = 281,
    ["num_lock"] = 282,
    ["print_screen"] = 283,
    ["pause"] = 284,
    ["f1"] = 290,
    ["f2"] = 291,
    ["f3"] = 292,
    ["f4"] = 293,
    ["f5"] = 294,
    ["f6"] = 295,
    ["f7"] = 296,
    ["f8"] = 297,
    ["f9"] = 298,
    ["f10"] = 299,
    ["f11"] = 300,
    ["f12"] = 301,
    ["f13"] = 302,
    ["f14"] = 303,
    ["f15"] = 304,
    ["f16"] = 305,
    ["f17"] = 306,
    ["f18"] = 307,
    ["f19"] = 308,
    ["f20"] = 309,
    ["f21"] = 310,
    ["f22"] = 311,
    ["f23"] = 312,
    ["f24"] = 313,
    ["f25"] = 314,
    ["kp0"] = 320,
    ["kp1"] = 321,
    ["kp2"] = 322,
    ["kp3"] = 323,
    ["kp4"] = 324,
    ["kp5"] = 325,
    ["kp6"] = 326,
    ["kp7"] = 327,
    ["kp8"] = 328,
    ["kp9"] = 329,
    ["kp."] = 330,
    ["kp/"] = 331,
    ["kp*"] = 332,
    ["kp-"] = 333,
    ["kp+"] = 334,
    ["kp_enter"] = 335,
    ["kp="] = 336,
    ["lshift"] = 340,
    ["lctrl"] = 341,
    ["lalt"] = 342,
    ["lsuper"] = 343,
    ["rshift"] = 344,
    ["rctrl"] = 345,
    ["ralt"] = 346,
    ["rsuper"] = 347,
    ["menu"] = 348,
}

local keyboard = {
    prev = {},
    current = {},
}

function keyboard.on_keydown(keycode)
    keyboard.current[keycode] = true
end

function keyboard.on_keyup(keycode)
    keyboard.current[keycode] = nil
end

function keyboard.down(key)
    return keyboard.current[translate_key[key]]
end

function keyboard.pressed(key)
    local keycode = translate_key[key]
    return not keyboard.prev[keycode] and keyboard.current[keycode]
end

function keyboard.released(key)
    local keycode = translate_key[key]
    return keyboard.prev[keycode] and not keyboard.current[keycode]
end

function keyboard.update()
    for k in pairs(keyboard.prev) do
        keyboard.prev[k] = nil
    end

    for k in pairs(keyboard.current) do
        keyboard.prev[k] = true
    end
end

return keyboard
