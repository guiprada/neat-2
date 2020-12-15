game = {}
local scancodes = neat.define_scancodes()
local color = require "colors"

function game.load()
	game.font = neat.new_font("fonts/PressStart2P-Regular.ttf", 48)

	-- windows size
	game.width, game.height = 500, 500

	-- create some windows, note that we also get the windows ids back
	game.w1 = {}
	game.w1.window, game.w1.id = neat.new_window("teste neat_lua 1", 100, 100, screen_width, screen_height)
	game.w2 = {}
	game.w2.window, game.w2.id = neat.new_window("teste neat_lua 2", 600, 100, screen_width, screen_height)

	game.circle_texture1 = neat.new_texture(game.w1.window, "assets/square_32.png")
	game.circle_texture2 = neat.new_texture(game.w2.window, "assets/square_32.png")

	game.circle1 = neat.new_sprite(circle_texture1)
	game.circle1:move_to(math.floor(screen_width/2)-16, math.floor(screen_height/2 -16))
	game.circle2 = neat.new_sprite(circle_texture2)
	game.circle2:move_to(math.floor(screen_width/2 -16), math.floor(screen_height/2 -16))

end

function game.update(dt)
	 if (neat.is_key_pressed(scancodes["U"])) then
        print("u")
    end

    if (neat.is_key_pressed(scancodes["Left"])) then
        circle1_speed=-0.3*dt
    elseif (neat.is_key_pressed(scancodes["Right"])) then
        circle1_speed=0.3*dt
    else
        circle1_speed = 0
    end
	circle1:move(circle1_speed,0)
end

function  game.draw()

end

return game