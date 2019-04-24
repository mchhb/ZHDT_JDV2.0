#include "stm32f10x.h"

#include "BD_TG.h"
#include "MqttKit.h"
#include "socket.h"
#include "w5500_config.h"
#include "w5500.h"
#include "tcp_demo.h"

#include "delay.h"
#include <string.h>
#include <stdio.h>


#define PROID		"utfuett/device_001"							// �豸ȫ��//֮ǰ�û����ǣ�96ppxbd/s001

#define AUTH_INFO	"j9NiGuEaIuKPVexp/MUwWAlZLDox1YQ5+eqX6DuuP/k="	// API��Կ  //֮ǰ������c8opPYlA7ejO7RxAmMjynNBBf015UaGf04UxAIowQIU=

#define DEVID		"DeviceClient005"																								// ��ҫ����豸ID 002��
//uint8 Disconnect_Request[2] = {0xe0,0x00};
unsigned char w5500_buf[128];   //֮ǰ��128
unsigned char publish_buf[64]; 
extern unsigned char Restore_factory[10];
extern unsigned char  flag;
extern unsigned char  BD_TGflag;
//==========================================================
//	�������ƣ�	BD_TG_DevLink

//	�������ܣ�	��ٶ��칤��������

//	��ڲ�����	��

//	���ز�����	1-�ɹ�	0-ʧ��

//	˵����		��ٶ��칤ƽ̨��������
//==========================================================
//const char *topics1[] = {"stateup"};

void BD_TG_DevLink(void)
{
	
	int len;
	static MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���

	unsigned char *dataPtr;
//	unsigned char mqtt=0;

	printf( "�ٶ��칤_DevLink\r\nPROID: %s,	AUIF: %s,	DEVID:%s\r\n",PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 60, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
			
		
		send(SOCK_TCPC,mqttPacket._data,mqttPacket._len);	
		delay_ms(2000);  //�ȴ�ƽ̨��Ӧ
      //send(SOCK_TCPC,test,sizeof(test));/////////���Դ���//////////
			//BD_TG_Publish(*topics1, "123"); //������Ϣ
//		while( w5500_buf[0] != 0x20)
		while( w5500_buf[0] != 0x20)
		{	
			len=getSn_RX_RSR(SOCK_TCPC);		
			recv(SOCK_TCPC,w5500_buf,len); 
			//send(SOCK_TCPC,Disconnect_Request, 2);	
			//send(SOCK_TCPC,mqttPacket._data,mqttPacket._len);
            delay_ms(1);
					  break;	
 		}
		if(w5500_buf[0] == 0x20)
	  {
		  flag=1;
		}	
//		else
//		{	
//			len=getSn_RX_RSR(SOCK_TCPC);		
//			recv(SOCK_TCPC,w5500_buf,len); 
//			//send(SOCK_TCPC,Disconnect_Request, 2);	
//			//send(SOCK_TCPC,mqttPacket._data,mqttPacket._len);
////            delay_ms(100);
//			
//		//BD_TGflag=1;	
// 		}
	
		dataPtr = w5500_buf;	
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:printf( "Tips:	���ӳɹ�\r\n");break;			
					case 1:printf("WARN:	����ʧ�ܣ�Э�����\r\n");break;
					case 2:printf("WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");break;
					case 3:printf("WARN:	����ʧ�ܣ�������ʧ��\r\n");break;
					case 4:printf("WARN:	����ʧ�ܣ��û������������\r\n");break;
					case 5:printf("WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");break;		
					default:printf("ERR:	����ʧ�ܣ�δ֪����\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//ɾ��
	}
	else
	printf( "WARN:	MQTT_PacketConnect Failed\r\n");
	

}


void MQTT_UnSubscribe(const char *topics[], uint8 topics_cnt)
{

	int len;
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//Э���

	if(MQTT_PacketUnSubscribe(10, topics,topics_cnt, &mqttPacket) == 0)
	{
	   send(SOCK_TCPC,mqttPacket._data,mqttPacket._len);
	   delay_ms(100);  //�ȴ�ƽ̨��Ӧ
		 len=getSn_RX_RSR(SOCK_TCPC);
     recv(SOCK_TCPC,w5500_buf,len);
		 if ((w5500_buf[0] >> 4) == MQTT_PKT_UNSUBACK)
		    printf("�ɹ�ȡ������\r\n");					
	}	

	

}
//==========================================================
//	�������ƣ�	BD_TG_Subscribe

//	�������ܣ�	����

//	��ڲ�����	topics�����ĵ�topic
//				topic_cnt��topic����

//	���ز�����	SEND_TYPE_OK-�ɹ�	SEND_TYPE_SUBSCRIBE-��Ҫ�ط�

//	˵����		
//==========================================================
void BD_TG_Subscribe(const char *topics[], unsigned char topic_cnt)
{
	
	unsigned char i = 0;
	uint16  len;
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//Э���
	
	for(; i < topic_cnt; i++)
		printf( "Subscribe Topic: %s\r\n", topics[i]);
	
	if(MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL0, topics, topic_cnt, &mqttPacket) == 0)
	{
		send(SOCK_TCPC,mqttPacket._data, mqttPacket._len);					//��ƽ̨���Ͷ�������
		
		MQTT_DeleteBuffer(&mqttPacket);											//ɾ��
	}
	
	delay_ms(100);  //�ȴ�ƽ̨��Ӧ
	len=getSn_RX_RSR(SOCK_TCPC);
	recv(SOCK_TCPC,w5500_buf,len); 
	switch(w5500_buf[4])
	{
			case 0x00:
				printf("����ȷ�ϣ� QoS�ȼ�0\r\n");
			  break;
			case 0x01:
				printf("����ȷ�ϣ� QoS�ȼ�1\r\n");
			  break;
		  case 0x02:
				printf("����ȷ�ϣ� QoS�ȼ�2 \r\n");
			  break;
			case 0x80:
				printf("����ʧ�� \r\n");
			  break;
		}
	 

}

//==========================================================
//	�������ƣ�	BD_TG_Publish
//
//	�������ܣ�	������Ϣ
//
//	��ڲ�����	topic������������
//				msg����Ϣ����
//
//	���ز�����	SEND_TYPE_OK-�ɹ�	SEND_TYPE_PUBLISH-��Ҫ����
//
//	˵����		
//==========================================================
char test1[200]={1,2,3,4,5,0,6,7,8,9,0,12};
unsigned char ttt[100]={1,2,3,4,5,0,6,7,8,9,0,12};;
extern MQTT_PACKET_STRUCTURE mqttPacket;							//Э���
    
void BD_TG_Publish(const char *topic,char *msg,unsigned short Length)  //����ԭ��void BD_TG_Publish(const char *topic, const char *msg)
{
//	char i;
     
	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, Length, MQTT_QOS_LEVEL0, 0, 1, &mqttPacket) == 0)
	{
		send(SOCK_TCPC,mqttPacket._data, mqttPacket._len);					////mqttPacket._len��ƽ̨���Ͷ������
	  printf("���ͳɹ�\r\n"); 
		MQTT_DeleteBuffer(&mqttPacket);
		delay_ms(100);
	}

}

//==========================================================
//	�������ƣ�	BD_TG_RevPro
//
//	�������ܣ�	ƽ̨�������ݼ��
//
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void BD_TG_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//Э���
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	unsigned char type = 0;
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
		case MQTT_PKT_CMD:															//�����·�
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload);				//���topic����Ϣ��
			if(result == 0)
			{
				printf("cmdid: %s, req: %s\r\n", cmdid_topic, req_payload);
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//����ظ����
				{
				 printf( "Tips:	Send CmdResp\r\n");
					
					send(SOCK_TCPC,mqttPacket._data, mqttPacket._len);			//�ظ�����
					MQTT_DeleteBuffer(&mqttPacket);									//ɾ��
				}
			}
		
		break;
			
		case MQTT_PKT_PUBLISH:														//���յ�Publish��Ϣ
		
			result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &req_payload, &qos, &pkt_id);
			if(result == 0)
			{
				printf("topic: %s\r\npayload: %s\r\n", cmdid_topic, req_payload);
				memcpy(publish_buf,req_payload,strlen(req_payload));
				switch(qos)
				{
					case 1:													//�յ�publish��qosΪ1���豸��Ҫ�ظ�Ack
					
						if(MQTT_PacketPublishAck(pkt_id, &mqttPacket) == 0)
						{
							printf( "Tips:	Send PublishAck\r\n");
							send(SOCK_TCPC,mqttPacket._data, mqttPacket._len);
							MQTT_DeleteBuffer(&mqttPacket);
						}
					
					break;
					
					case 2:													//�յ�publish��qosΪ2���豸�Ȼظ�Rec
																					//ƽ̨�ظ�Rel���豸�ٻظ�Comp
						if(MQTT_PacketPublishRec(pkt_id, &mqttPacket) == 0)
						{
							printf("Tips:	Send PublishRec\r\n");
							send(SOCK_TCPC,mqttPacket._data, mqttPacket._len);
							MQTT_DeleteBuffer(&mqttPacket);
						}
					
					break;
					
					default:
						break;
				}
			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//����Publish��Ϣ��ƽ̨�ظ���Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				printf("Tips:	MQTT Publish Send OK\r\n");
			
		break;
			
		case MQTT_PKT_PUBREC:														//����Publish��Ϣ��ƽ̨�ظ���Rec���豸��ظ�Rel��Ϣ
		
			if(MQTT_UnPacketPublishRec(cmd) == 0)
			{
				printf("Tips:	Rev PublishRec\r\n");
				if(MQTT_PacketPublishRel(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					 printf ("Tips:	Send PublishRel\r\n");
					send(SOCK_TCPC,mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
		
		break;
			
		case MQTT_PKT_PUBREL:														//�յ�Publish��Ϣ���豸�ظ�Rec��ƽ̨�ظ���Rel���豸���ٻظ�Comp
			
			if(MQTT_UnPacketPublishRel(cmd, pkt_id) == 0)
			{
				printf("Tips:	Rev PublishRel\r\n");
				if(MQTT_PacketPublishComp(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					printf( "Tips:	Send PublishComp\r\n");
					send(SOCK_TCPC,mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
		
		break;
		
		case MQTT_PKT_PUBCOMP:														//����Publish��Ϣ��ƽ̨����Rec���豸�ظ�Rel��ƽ̨�ٷ��ص�Comp
		
			if(MQTT_UnPacketPublishComp(cmd) == 0)
			{
			 printf("Tips:	Rev PublishComp\r\n");
			}
		
		break;
			
		case MQTT_PKT_SUBACK:														//����Subscribe��Ϣ��Ack
		
			if(MQTT_UnPacketSubscribe(cmd) == 0)
				printf("Tips:	MQTT Subscribe OK\r\n");
			else
				printf("Tips:	MQTT Subscribe Err\r\n");
		
		break;
			
		case MQTT_PKT_UNSUBACK:														//����UnSubscribe��Ϣ��Ack
		
			if(MQTT_UnPacketUnSubscribe(cmd) == 0)
				printf( "Tips:	MQTT UnSubscribe OK\r\n");
			else
				printf("Tips:	MQTT UnSubscribe Err\r\n");
		
		break;
		
		default:
			result = -1;
		break;
	}
	
	//ESP8266_Clear();									//��ջ���
	
	if(result == -1)
		return;
	
	dataPtr = strchr(req_payload, '}');					//����'}'

	if(dataPtr != NULL && result != -1)					//����ҵ���
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
		{
			numBuf[num++] = *dataPtr++;
		}
		
		num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
		
	}

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}
}
