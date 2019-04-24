#ifndef __W5500_CONFIG_H_
#define __W5500_CONFIG_H_
#include "config.h"

#define IP_FROM_DEFINE	        0                          /*ʹ�ó�ʼ�����IP��Ϣ*/
#define IP_FROM_DHCP	          1                          /*ʹ��DHCP��ȡIP��Ϣ*/
#define IP_FROM_EEPROM	        2							             /*ʹ��EEPROM�����IP��Ϣ*/

typedef struct _CONFIG_MSG
{
  uint8 op[4];//header: FIND;SETT;FACT...
  uint8 mac[6];
  uint8 sw_ver[2];
  uint8 lip[4];
  uint8 sub[4];
  uint8 gw[4];
  uint8 dns[4];	
  uint8 dhcp;
  uint8 debug;

  uint16 fw_len;
  uint8 state;
  
}CONFIG_MSG;
typedef struct _EEPROM_MSG	                    
{
	uint8 mac[6];																							/*MAC��ַ*/
  uint8 lip[4];																							/*local IP����IP��ַ*/
  uint8 sub[4];																							/*��������*/
  uint8 gw[4];																							/*����*/
}EEPROM_MSG_STR;
extern uint32 BD_TG_ping_time;                                    /*��BD_TGƽ̨��������Ӧ�����*/
extern CONFIG_MSG  ConfigMsg, RecvMsg;
extern uint8 txsize[];
extern uint8 rxsize[];
extern uint8	ip_from;											               /*ѡ��IP��Ϣ����Դ*/
extern uint8 dhcp_ok;
void dhcp_config_w5500(void);
void set_network(void);
void set_default(void);
void set_w5500_mac(void);
void set_w5500_ip(void);
void dhcp_timer_init(void);
extern uint16 local_port;
#endif


