#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

extern "C" 
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

void set_global_string(lua_State* L, const char* k, const char* v)
{
	lua_pushstring(L, v);
	lua_setglobal(L, k);
}

int call_global_commond(lua_State* L, const char* funcname, const char* cmd)
{
	lua_getglobal(L, funcname);
	lua_pushstring(L, cmd);
	int ret = lua_pcall(L, 1, 1, 0);
	if (ret)
	{
		while (lua_gettop(L))
		{
			printf("%s\n", lua_tostring(L, -1));
			lua_pop(L, -1);
		}
		return 1;//error
	}
	else//succ
	{
		int ret = 0;
		if (lua_tonumber(L, -1) < 0)//invalid command
			ret = 2;
		else//command exec
			ret = 0;
		lua_pop(L, -1);
		return ret;
	}
}

void clear_screen()
{
	system("cls");
}

int main(int argc, char* argv[])
{
	clear_screen();

	lua_State* L = lua_open();

	luaL_openlibs(L);

	int ret = luaL_dofile(L, "script");
	if (ret)
	{
		printf("load script failed, check syntax error please.");
		return EXIT_SUCCESS;
	}
	call_global_commond(L, "LOAD_FILE", argv[1]);

	char cmd = 0;
	char lua_cmd[2]{0, '\0'};
	while ((cmd = getch()) != 'q')
	{
		lua_cmd[0] = cmd;
		int ret = call_global_commond(L, "CHECK_CMD", lua_cmd);
		if (!ret)
		{
			clear_screen();
			call_global_commond(L, "EXEC_CMD", lua_cmd);
		}
	}
	call_global_commond(L, "EXIT", "");

	clear_screen();
	lua_close(L);
	return EXIT_SUCCESS;
}
