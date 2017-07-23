#include "rw_app.h"

int platform_init(void)
{
  rw_DriverParams_t     params;
  int                   ret =0;
  char                  libVersion[20]="";
  char                  module_mac[6] ="";
	
  host_platformInit();//bsp.h	初始化时钟以及串口
  DPRINTF("Host platform init...success\r\n");//就死printf
  
  //rak module driver init
  wifi_init_params(&params);//rw_lib_platform.c 负责初始化RAK439的硬件参数 包括硬件接口函数、时间管理函数、内存管理函数、RAK439回调函数和系统运行函数
														//类似于引用 params从系统中接收了参数
  ret =rw_sysDriverInit(&params);//rw_lib.h lib中的 参见API文档 功能：驱动初始化
  if(ret != RW_OK)
  {
    DPRINTF("RAK module platform init...failed code=%d\r\n", ret);
    while(1); 
  }
  rw_getLibVersion(libVersion);//获取版本号 
  DPRINTF("rak wifi LibVersion:%s\r\n", libVersion);
  rw_getMacAddr(module_mac);//获取MAC地址
  DPRINTF("rak wifi module-MAC:%02X:%02X:%02X:%02X:%02X:%02X\r\n", module_mac[0],module_mac[1],module_mac[2],module_mac[3],module_mac[4],module_mac[5]);
  
  return RW_OK;
}

int main(void)
{
    int  		ret = 0; 
    
    platform_init(); //初始化系统时钟、初始化窜口、初始化Wifi驱动、打印Wifi库版本、打印Wifi模块MAC地址
    rw_appdemo_context_init();//初始化Wifi运行参数 貌似是所有的参数句柄初始化为invalid

    rw_network_startSTA();//模块STA模式连接路由 这里可以设置Wifi的ssid和password

    while(1) {
			DPRINTF("ok =%d\r\n", 1);
      
        if((ret =rw_sysDriverLoop()) != RW_OK){//驱动的循环管理 非OS环境下调用维持模块驱动正常 Wifi数据交互频繁需经常调用！！
																								//在send函数，receive函数，delay函数中也会调用
            DPRINTF("rw_sysDriverLoop error =%d\r\n", ret);
        }
        
        if (app_demo_ctx.rw_connect_status == STATUS_OK && app_demo_ctx.rw_ipquery_status == STATUS_OK) {//extern RW_APP_CTX  app_demo_ctx;
	     //上面的状态变化是在connect_callback和ip_callback两个回调函数中执行
					RAK_TcpServer_EventHandle();//连接上路由并且获取到IP，执行处理
//          RAK_TcpClient_EventHandle();
//          RAK_UdpServer_EventHandle();
//          RAK_UdpClient_EventHandle();
           
        }else if (app_demo_ctx.rw_connect_status == STATUS_FAIL || app_demo_ctx.rw_ipquery_status == STATUS_FAIL) {
          DPRINTF("reconnect and ipquery...\r\n");//连接路由失败或者获取IP失败
          rw_appdemo_context_init();  //初始化Wifi运行参数 
          rw_sysDriverReset();//复位 重新连接路由
          rw_network_init(&conn, DHCP_CLIENT, NULL);//extern rw_WlanConnect_t  conn;是连接参数 保存了SSID和密码等数据
        }
    }
}
