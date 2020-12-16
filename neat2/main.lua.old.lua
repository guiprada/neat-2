-- guiprada@gmail.com  Guilherme Cunha Prada 2017-2020 : )
-- zlib license

local utils = require "utils"
utils.run_tests()

local scancodes = neat.define_scancodes()

-- load up some fonts
local font = neat.new_font("fonts/PressStart2P-Regular.ttf", 48)

-- windows size
local screen_width, screen_height = 201, 201

-- open the windows, note that we also get the windows ids back
window1, id_window1 = neat.new_window("teste neat_lua 1", 100, 100, screen_width, screen_height)
window2, id_window2 = neat.new_window("teste neat_lua 2", 500, 100, screen_width, screen_height)

red = neat.new_color( 255, 0, 0, 255)
blue = neat.new_color( 0, 0, 255, 255)
circle_texture1 = neat.new_texture("assets/square_32.png")
circle_texture2 = neat.new_texture("assets/square_32.png", window2)

circle1 = neat.new_sprite(circle_texture1)
circle1:move_to(math.floor(screen_width/2)-16, math.floor(screen_height/2 -16))
circle2 = neat.new_sprite(circle_texture2)
circle2:move_to(math.floor(screen_width/2 -16), math.floor(screen_height/2 -16))

texto_texture1 = neat.new_text_texture("Ola Mundo", font1,red, window1)
texto1 = neat.new_sprite(texto_texture1)
texto1:move_to(20,20)
texto1:set_rotation(20)
texto1:set_anchor(20,20)

texto_texture2 = neat.new_text_texture("focus : ", font1, red, window2)
texto2 = neat.new_sprite(texto_texture2)
texto2:move_to(0,0)

circle1_speed = 0
is_running = true
local last_time = os.clock()
while is_running do
	local this_time = os.clock()
	local delta_time = this_time - last_time
	last_time = this_time
	--print(1/delta_time, delta_time)
    if (neat.is_key_pressed(scancodes["U"])) then
        print("u")
    end

    if (neat.is_key_pressed(scancodes["Left"])) then
        circle1_speed=-2
    elseif (neat.is_key_pressed(scancodes["Right"])) then
        circle1_speed=2
    else
        circle1_speed = 0
    end

	repeat event, arg1, arg2, arg3 = neat.get_event()
        if(event == "mouse_motion")then
            if(arg1 == id_window2)then
                circle2:move_to(arg2, arg3)
            end
            --print(arg1 .. " " ..arg2)
        elseif(event == "quit")then
            if window1 and arg1 == id_window1 then
                window1:hide()
				--circle_texture1 = nil
				--texto_texture1 = nil
                window1 = nil
            end
            if window2 and arg1 == id_window2 then
                window2:hide()
				--circle_texture2 = nil
				--texto_texture2 = nil
                window2 = nil
            end
            collectgarbage('collect')

        elseif(window2 and event == "focus")then
            texto_texture2 = neat.new_text_texture("focus : " .. tostring(arg1), font1, red, window2)
            texto2 = neat.new_sprite(texto_texture2)

        elseif(event == "keyup")then
            if(arg2== "Escape" and arg1 == id_window1) then is_running = false end

        elseif(event == "keydown")then
            -- print(arg2)
            -- print(arg1)
			if window2 then
				texto_texture2 = neat.new_text_texture("focus : " .. tostring(arg1), font1, red, window2)
				texto2 = neat.new_sprite(texto_texture2)
			end
        end
        --event, arg1, arg2 = neat.get_event()
    until(not event)

	if window1 and window1:is_shown()  then
        window1:clear()
        circle1:render()
        texto1:render()
        circle1:move(circle1_speed,0)
        neat.rect_fill(100, 100, 50, 50, red, window1)
        window1:present()
    end

    if window2 and window2:is_shown() then
        window2:clear()
        circle2:render()
        texto2:render()
        window2:present()
    end

    if (window1 == nil and window2 == nil) then is_running = false end
end
