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
