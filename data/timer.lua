local timer = {
    next_id = 0,
    afters = {},
}

function timer.after(secs, cb, ...)
    timer.afters[timer.next_id] = {
        elapsed = 0,
        limit = secs,
        callback = cb,
        args = table.pack(...),
    }
    timer.next_id = timer.next_id + 1
end

function timer.update(dt)
    for k, a in pairs(timer.afters) do
        a.elapsed = a.elapsed + dt
        if a.elapsed > a.limit then
            a.callback(table.unpack(a.args))
            timer.afters[k] = nil
        end
    end
end

return timer
