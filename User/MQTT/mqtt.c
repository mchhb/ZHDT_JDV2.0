#include "mqtt.h"
#include "MQTTPacket.h"
#include "transport.h"
#include "Types.h"
#include "socket.h"
#include "w5500.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
extern uint8 w5500_connect;
extern uint8 domain_ip[4];

//MQTT������Ϣ����
int do_mqtt_publish(char *pTopic,char *pMessage)
{
	int32_t len,rc;
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;//���ò��ֿɱ�ͷ����ֵ
	unsigned char buf[100];
	MQTTString topicString = MQTTString_initializer;
	int msglen = strlen(pMessage);//���㷢����Ϣ�ĳ���
	int buflen = sizeof(buf);
	if( w5500_connect == 1)
	{
		w5500_connect++;
		transport_open1();//�򿪷�����Ϣ��SOCKET2016.12.29
		
		
		
		data.clientID.cstring = "me";//�ͻ��˱�ʶ����������ÿ���ͻ���
		data.keepAliveInterval = 60;//�����ʱ���������˷������յ��ͻ�����Ϣ�����ʱ����
		data.cleansession = 1;//�ñ�־��1���������붪��֮ǰ���ֵĿͻ��˵���Ϣ������������Ϊ�������ڡ�
		data.username.cstring = "admin";
		data.password.cstring = "password";
		len = MQTTSerialize_connect(buf, buflen, &data); /*1 �������ӱ���*/
		/*2016��12.29��*/
		rc = transport_sendPacketBuffer1(buf, len);//������������
		if(rc != len)
		{
			printf("publish connect transport_sendPacketBuffer error\n\r");
		}
		
		{
			unsigned char sessionPresent, connack_rc;
			do
			{
				while(MQTTPacket_read(buf, buflen, transport_getdata1) != CONNACK)//�Խ��յ��ı��Ľ��н���
				{
					;
				}
			}
			while(MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0);
			printf("PUBLISH CONNACK is ok %s\r\n",buf);
		}
	}
	memset(buf,0,buflen);
  //strcat(pTopic,data.clientID.cstring);//�ϲ��ַ���Ϊ��������
	topicString.cstring = pTopic;
	len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)pMessage, msglen); /*2 ���췢����Ϣ�ı���*/
	
	rc = transport_sendPacketBuffer1(buf,len);//������Ϣ
	memset(buf,0,buflen);
//  transport_close1();//�ر���ӦSOCKET2016.12.30
	if (rc == len)
		printf("Successfully published\n\r");
	else
		printf("Publish failed\n\r");
	return 0;
}

//MQTT������Ϣ����
int do_mqtt_subscrib(char *pTopic,char *pMessage)
{
	/*����һ��MQTTPacket_connectData���ͽṹ��������ýṹ������ɱ�ͷ������*/
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	int rc = 0;
	unsigned char dup;
	int qos;
	unsigned char retained;
	unsigned short mssageid;
	int payloadlen_in;
	unsigned char* payload_in;
	MQTTString receivedTopic;
	
	unsigned char buf[200];
	int buflen = sizeof(buf);
	int msgid = 1;//��ֵΪ��Ϣ��ʶ��
	/*������һ��MQTTString�ṹ�����ͱ���*/
	MQTTString topicString = MQTTString_initializer;
	int req_qos = 0;
	//int num = 0;
	int len = 0;
	rc = transport_open();//��socket�˿�
	if(rc < 0)
	{
		printf("transport_open error\n\r");
		return rc;
	}
	data.clientID.cstring = "ABCD";//�ͻ��˱�ʶ����������ÿ���ͻ���
	data.keepAliveInterval = 60;//��������������ʱ�䣬������ʱ��󣬷������������Ͽ����ӣ���λΪ��
	data.cleansession = 1;//�ñ�־��1���������붪��֮ǰ���ֵĿͻ��˵���Ϣ������������Ϊ�������ڡ�
	data.username.cstring = "admin";
	data.password.cstring = "password";
	len = MQTTSerialize_connect(buf, buflen, &data);//����Connect Flag�ֽڵĹ����ڸú����н��в�����Ӧֵ���� buf������
	rc = transport_sendPacketBuffer(buf, len);//������������
	if(rc != len)
	{
		printf("connect transport_sendPacketBuffer error\n\r");
	}
	
	/*�ȴ�CONNACK���ݰ��ĵ���connack_rc�д�ŵ���Return Code��ֵ��ֵΪ0ʱ��ʾ���ӽ����ɹ�*/
	{
		unsigned char sessionPresent, connack_rc;
		do
		{
			while(MQTTPacket_read(buf, buflen, transport_getdata) != CONNACK)//�Խ��յ��ı��Ľ��н���
			{
				;
			}
		}
		while(MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0);
		printf("CONNACK is ok %s\r\n",buf);
	}
	
	/* subscribe ���Ͷ�������*/
	topicString.cstring = pTopic;
	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);//���충�����ⱨ��
	rc = transport_sendPacketBuffer(buf, len);
	if(rc != len)
	{
		printf("connect transport_sendPacketBuffer error\n\r");
	}
	/*�ȴ�SUBACK ����ȷ�ϱ��ĵĵ���*/	
	{
		unsigned short submsgid;
		int subcount;
		int granted_qos;
		do
		{
			while(MQTTPacket_read(buf, buflen, transport_getdata) != SUBACK)
			{
				;
			}
			rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
		}
		while (granted_qos != 0);
		printf("SUBACK is ok\r\n");
	}
	
	/* loop getting msgs on subscribed topic */
	topicString.cstring = pTopic;
	memset(buf,0,buflen);
	/*������ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	printf(" ���r�����_ !\r\n");
	/*�������ݻ����������Ƿ������Ͽ����Ӻ�ŷ���*/
	//printf("����Ͽ�");
	//printf("getSn_SR is %d",getSn_SR(SOCK_TCPS));
	return 0;
}
