game = {}
local scancodes = neat.define_scancodes()


function game.load(window, width, height)
	game.font = neat.new_font("fonts/PressStart2P-Regular.ttf", 48)
	game.font = neat.new_font("fonts/GamjaFlower-Regular.ttf", 48)
	game.font = neat.new_font("fonts/PlayfairDisplaySC-Italic.ttf", 48)
	game.font = neat.new_font("fonts/MS/times.ttf", 48)
	game.font = neat.new_font("fonts/MS/arial.ttf", 48)


	-- windows size
	-- game.window = window
	game.width, game.height = 500, 500

	-- create some windows, note that we also get the windows ids back
	
	local circle_texture = neat.new_texture("assets/square_32.bmp")	
	game.circle = neat.new_sprite(circle_texture)
	game.circle:move_to(math.floor(width/2)-16, math.floor(height/4 -16))

	local rectangle_texture = neat.new_texture("assets/rectangle2.png")
	game.rectangle = neat.new_sprite(rectangle_texture)
	game.rectangle:set_source(0, 32, 128, 64)
	game.rectangle:move_to(125, 250)
	game.rectangle:set_anchor(0,game.rectangle:get_height())
	--game.rectangle:set_anchor(0,0)

	local rectangle_texture2 = neat.new_texture("assets/rectangle2.png")
	game.rectangle2 = neat.new_sprite(rectangle_texture2)
	game.rectangle2:move_to(250, 250)
	--print(game.rectangle2:get_width())
	--print(game.rectangle2:get_height())
	--print(game.rectangle2:get_dimensions())

	game.resized = false
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

	game.rectangle:add_rotation(1)

	if neat.is_key_pressed(scancodes["U"]) and game.resized == false then
		game.rectangle:set_scale(2, 2)
		game.rectangle2:set_scale(2, 2)
		game.resized = true
	end
end

function  game.on_mouse_motion(x, y)
	
end

function  game.on_window_close()	

end

function  game.on_focus_gained()	

end

function  game.on_focus_lost()	

end

function  game.on_keyup()	

end

function  game.on_keydown()	

end

function  game.render()
	game.circle:render()
	neat.rect_fill(125,250,128,128, neat.color.red)
	game.rectangle:render()
	--game.rectangle2:render()
	
end

return game