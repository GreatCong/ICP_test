/**
  ******************************************************************************
  * @file   
  * @author  liucongjun
  * @version 
  * @date    
  * @brief ����MicroLibû�еĺ���  
	         lua.c      ---> PC��lua��main����
					 luac.c     ---> lua�Ľ�����main����
					 lint.c     ---> lua��ӵĿ�
					 loslib.c   --> os ���õĿ⣨���ã�
					 liolib.c   --> io�ļ����õĿ�
					 lauxlib.c  ---> 1008�� l_alloc���� �޸�myfree��myrealloc�ڴ���� �����޸������ļ�heap��stack�㹻��
					 Lua print��ʵ����lbaselib.c��luaB_print���� --> �޸���luaB_print������ʹ���ӡ������ʱ�򲻻��һ��'\t'
					 lauxlib.h  ---> �����˴����ض����Ĭ��ֵ,������luaconf.h������޸� lua�����ļ���·��Ҳ��luaconf.h���޸�
******************************************************************************
*/

#include "time.h"

/* ����MicroLibû�еĺ��� */ 
time_t time(time_t * time) {     
  return 0; 
}  

void exit(int status) {      

}  

int system(const char * string) {     
  return 0; 
}
