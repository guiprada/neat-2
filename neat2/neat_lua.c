// guiprada@gmail.com  Guilherme Cunha Prada 2017-2020 : )
// zlib license

#include "neat_lua.h"

int main()
{
	int SDL2_start_status = neat_start();
	if (SDL2_start_status != 0) {
		printf("SDL2 start ERROR, status: %d\n", SDL2_start_status);
		return 0;
	}

	SDL_version compiled;
	SDL_version linked;


	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("Compiled with SDL version %d.%d.%d.\n",
		compiled.major, compiled.minor, compiled.patch);
	printf("Linking with SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);

	// lua variables
	int status, result;
	lua_State* L;

	L = luaL_newstate();


	luaL_openlibs(L);

	lua_open_neat(L);

	/* Load the file containing the script we are going to run */
	printf("Loading script...\n");
	status = luaL_loadfile(L, "main.lua");
	if (status) {
		/* If something went wrong, error message is at the top of */
		/* the stack */
		fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		exit(1);
	}

	/* Ask Lua to run our little script */
	printf("Running script...\n");
	result = lua_pcall(L, 0, LUA_MULTRET, 0);
	if (result) {
		fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
		exit(1);
	}

	double lua_return;
	lua_return = lua_tonumber(L, -1);

	printf("Script returned: %.0f\n", lua_return);

	lua_pop(L, 1);  /* Take the returned value out of the stack */

	lua_close(L);
	neat_stop();
	//system("pause");
	return 0;
}