-- guiprada@gmail.com  Guilherme Cunha Prada 2017-2020 : )
-- zlib license

local utils = require "utils"
utils.run_tests()
local scancodes = neat.define_scancodes()

-- windows size
local screen_width, screen_height = 500, 500

-- open the windows, note that we also get the windows ids back
local window1 = {}
window1.window, window1.id = neat.new_window("teste neat_lua", 100, 100, screen_width, screen_height)
window1.functions = require "game"
window1.functions.load(window1.window, screen_width, screen_height)

local is_running = true
local last_time = os.clock()

local events = {}
events["mouse_motion"] = function (arg1, arg2, arg3)
	if arg1 == window1.id then
		window1.functions.on_mouse_motion(arg2, arg3)                
    end
end
events["quit"] = function (arg1, arg2, arg3)
	if arg1 == window1.id then
		window1.functions.on_quit()
		window1.window = nil
	end
end
events["focus_gained"] = function (arg1, arg2, arg3)
	if arg1 == window1.id then
		window1.functions.on_focus_gain()
    end
end
events["focus_lost"] = function (arg1, arg2, arg3)
	if arg1 == window1.id then
		window1.functions.on_focus_lost()
	end
end
events["keyup"] = function (arg1, arg2, arg3)
    if arg1 == window1.id then
		window1.functions.on_keyup()
	end
end
events["keydown"] = function (arg1, arg2, arg3)
    if arg1 == window1.id then
		window1.functions.on_keydown()
	end
end

while is_running do
	repeat event, arg1, arg2, arg3 = neat.get_event()
		local f = events[event]
		if f then
			f(arg1, arg2, arg3)
		end
    until(not event)
	
	if window1.window then
        window1.window:clear()
        window1.functions.render()
        window1.window:present()
    else
		is_running = false
	end

	-- fps and heltatime
	local this_time = os.clock()
	local delta_time = this_time - last_time
	last_time = this_time
	--print(1/delta_time, delta_time)

	window1.functions.update(delta_time)
end
