/**
  ******************************************************************************
  * @file   
  * @author  liucongjun
  * @version 
  * @date    
  * @brief 定义MicroLib没有的函数  
	         lua.c      ---> PC中lua的main函数
					 luac.c     ---> lua的解释器main函数
					 lint.c     ---> lua添加的库
					 loslib.c   --> os 调用的库（不用）
					 liolib.c   --> io文件调用的库
					 lauxlib.c  ---> 1008行 l_alloc函数 修改myfree和myrealloc内存管理 否则修改启动文件heap和stack足够大
					 Lua print的实现在lbaselib.c的luaB_print函数 --> 修改了luaB_print函数，使其打印参数的时候不会多一个'\t'
					 lauxlib.h  ---> 包含了串口重定向的默认值,可以在luaconf.h中添加修改
******************************************************************************
*/

#include "time.h"

/* 定义MicroLib没有的函数 */ 
time_t time(time_t * time) {     
  return 0; 
}  

void exit(int status) {      

}  

int system(const char * string) {     
  return 0; 
}
