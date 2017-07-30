#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "lua_test.h"


/* 测试Lua的字符串 */
const char lua_test[] = { 
    "print(\"Hello,I am lua!\\n--this is newline printf\\r\\n\")\n"
    "function foo()\n"
    "  local i = 0\n"
    "  local sum = 1\n"
    "  while i <= 10 do\n"
    "    sum = sum * 2\n"
    "    i = i + 1\n"
    "  end\n"
    "return sum\n"
    "end\n"
    "print(\"use foo()..,sum =\", foo())\n"
    "print(\" and sum = 2^11 =\", 2 ^ 11)\n"
    "print(\" exp(200) =\", math.exp(200))\n"
};

/* 运行Lua */
void lua_main_test(void)
{
	lua_State *L;
	int ret = 0;
	
	L = luaL_newstate(); /* 建立Lua运行环境 */
	if(L==NULL) {
	  printf("cannot create state: not enough memory\r\n");
		while(1);
	}
	
	luaL_checkversion(L);  /* check that interpreter has correct version */
	luaL_openlibs(L); /* open standard libraries */
	luaopen_base(L);
	
	ret = luaL_dostring(L, lua_test); /* 运行Lua脚本 */
  if(ret !=0 ) {
		printf("something error in luaL_dostring\r\n");
		while(1);
	}
	
	lua_close(L);
}
