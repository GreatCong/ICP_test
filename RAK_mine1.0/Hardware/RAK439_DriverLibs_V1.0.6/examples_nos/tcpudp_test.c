#include "rw_app.h"

int platform_init(void)
{
  rw_DriverParams_t     params;
  int                   ret =0;
  char                  libVersion[20]="";
  char                  module_mac[6] ="";
	
  host_platformInit();//bsp.h	��ʼ��ʱ���Լ�����
  DPRINTF("Host platform init...success\r\n");//����printf
  
  //rak module driver init
  wifi_init_params(&params);//rw_lib_platform.c �����ʼ��RAK439��Ӳ������ ����Ӳ���ӿں�����ʱ����������ڴ��������RAK439�ص�������ϵͳ���к���
														//���������� params��ϵͳ�н����˲���
  ret =rw_sysDriverInit(&params);//rw_lib.h lib�е� �μ�API�ĵ� ���ܣ�������ʼ��
  if(ret != RW_OK)
  {
    DPRINTF("RAK module platform init...failed code=%d\r\n", ret);
    while(1); 
  }
  rw_getLibVersion(libVersion);//��ȡ�汾�� 
  DPRINTF("rak wifi LibVersion:%s\r\n", libVersion);
  rw_getMacAddr(module_mac);//��ȡMAC��ַ
  DPRINTF("rak wifi module-MAC:%02X:%02X:%02X:%02X:%02X:%02X\r\n", module_mac[0],module_mac[1],module_mac[2],module_mac[3],module_mac[4],module_mac[5]);
  
  return RW_OK;
}

int main(void)
{
    int  		ret = 0; 
    
    platform_init(); //��ʼ��ϵͳʱ�ӡ���ʼ���ܿڡ���ʼ��Wifi��������ӡWifi��汾����ӡWifiģ��MAC��ַ
    rw_appdemo_context_init();//��ʼ��Wifi���в��� ò�������еĲ��������ʼ��Ϊinvalid

    rw_network_startSTA();//ģ��STAģʽ����·�� �����������Wifi��ssid��password

    while(1) {
			DPRINTF("ok =%d\r\n", 1);
      
        if((ret =rw_sysDriverLoop()) != RW_OK){//������ѭ������ ��OS�����µ���ά��ģ���������� Wifi���ݽ���Ƶ���辭�����ã���
																								//��send������receive������delay������Ҳ�����
            DPRINTF("rw_sysDriverLoop error =%d\r\n", ret);
        }
        
        if (app_demo_ctx.rw_connect_status == STATUS_OK && app_demo_ctx.rw_ipquery_status == STATUS_OK) {//extern RW_APP_CTX  app_demo_ctx;
	     //�����״̬�仯����connect_callback��ip_callback�����ص�������ִ��
					RAK_TcpServer_EventHandle();//������·�ɲ��һ�ȡ��IP��ִ�д���
//          RAK_TcpClient_EventHandle();
//          RAK_UdpServer_EventHandle();
//          RAK_UdpClient_EventHandle();
           
        }else if (app_demo_ctx.rw_connect_status == STATUS_FAIL || app_demo_ctx.rw_ipquery_status == STATUS_FAIL) {
          DPRINTF("reconnect and ipquery...\r\n");//����·��ʧ�ܻ��߻�ȡIPʧ��
          rw_appdemo_context_init();  //��ʼ��Wifi���в��� 
          rw_sysDriverReset();//��λ ��������·��
          rw_network_init(&conn, DHCP_CLIENT, NULL);//extern rw_WlanConnect_t  conn;�����Ӳ��� ������SSID�����������
        }
    }
}
