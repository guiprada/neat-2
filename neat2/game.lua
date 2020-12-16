game = {}
local scancodes = neat.define_scancodes()
local color = require "color"

function game.load(window, width, height)
	game.font = neat.new_font("fonts/PressStart2P-Regular.ttf", 48)

	-- windows size
	game.width, game.height = 500, 500

	-- create some windows, note that we also get the windows ids back
	
	local circle_texture = neat.new_texture("assets/square_32.png", window)
	
	game.circle = neat.new_sprite(circle_texture)
	game.circle:move_to(math.floor(width/2)-16, math.floor(height/2 -16))	
end

function game.update(dt)
	local circle_speed_x = 0
	local speed = 100
    if (neat.is_key_pressed(scancodes["Left"])) then
        circle_speed_x = -speed*dt
    elseif (neat.is_key_pressed(scancodes["Right"])) then
        circle_speed_x = speed*dt
    end
	game.circle:move(circle_speed_x, 0)
end

function  game.on_mouse_motion(x, y)
	
end

function  game.on_quit()	

end

function  game.on_focus_gain()	

end

function  game.on_focus_lost()	

end

function  game.on_keyup()	

end

function  game.on_keydown()	

end

function  game.render()
	game.circle:render()
end

return game