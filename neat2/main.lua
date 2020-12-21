-- guiprada@gmail.com  Guilherme Cunha Prada 2017-2020 : )
-- zlib license
neat.color = require "color"
neat.events = require "events"
neat.scancodes = neat.define_scancodes()
neat.utils = require "utils"
neat.utils.run_tests()

-- windows size
local screen_width, screen_height = 500, 500

-- open the windows, note that we also get the windows ids back
local window1 = {}
window1.window, window1.id = neat.new_window("teste neat_lua", 100, 100, screen_width, screen_height)
window1.gamestate = require "game"
window1.gamestate.load(window1.window, screen_width, screen_height)

local is_running = true
local last_time = os.clock()

local events = {}
function on_mouse_motion (arg1, arg2, arg3)
	if arg1 == window1.id then
		window1.gamestate.on_mouse_motion(arg2, arg3)                
    end
	print("x: ", arg2)
end
function on_window_close (arg1, arg2, arg3)
	if arg1 == window1.id then
		window1.gamestate.on_window_close()
		window1.window = nil
	end
end
function on_focus_gained (arg1, arg2, arg3)
	if arg1 == window1.id then
		window1.gamestate.on_focus_gained()
    end
end
function on_focus_lost (arg1, arg2, arg3)
	if arg1 == window1.id then
		window1.gamestate.on_focus_lost()
	end
end
function on_keyup (arg1, arg2, arg3)
    if arg1 == window1.id then
		window1.gamestate.on_keyup()
	end
end
function on_keydown (arg1, arg2, arg3)
    if arg1 == window1.id then
		window1.gamestate.on_keydown()
	end
end

function unhandled(event)
	print("unhandled event:", neat.events[event], event)
end

while is_running do
	neat.dispatch_events()
	
	if window1.window then
        window1.window:clear()
        window1.gamestate.render(window1.window)
        window1.window:present()
    else
		is_running = false
	end

	-- fps and heltatime
	local this_time = os.clock()
	local delta_time = this_time - last_time
	last_time = this_time
	--print(1/delta_time, delta_time)

	window1.gamestate.update(delta_time)
end