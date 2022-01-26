#include"AnaJSON.h"
#include<malloc.h>

/*
���㵽�¸����ŵĳ���
@str 	��������ַ���
@front  �ַ�����ʼ����
@ch		������ַ�
*/
static int getSymlen(const char* str, int front, char ch);

/*
����ӵ�ж��ٸ�����
@*/
static int getObjnum(const char* str, int start);

/*
��ָ��λ�ø����ַ���
@str1	��������ַ���
@str2	���ƺ���ַ���
@start	��ʼλ��
@size	���ƵĴ�С
*/
static int strCpy(const char* str1, char* str2, int start, int size);

/*
����JSON
@*/
static int AnaJSON(const char* str, int* index, JSONData* data);

//�����ַ�����JSON
JSONData* StrtoJSON(const char* string)
{
	int index = 0;
	if(!string || string[0] != '{')		//�ж��Ƿ�������JSON��ͷ
		return NULL;

	JSONData* data;

	AnaJSON(string, &index, data);
	return data;
}

//����JSON
static int AnaJSON(const char* str, int* index, JSONData* data)
{
	int len = -1;
	
	//��ʼ��
	data = (JSONData*)malloc(sizeof(JSONData));
	data->num = -1;
	data->domain = NULL;
	data->name = NULL;
	
	len = getSymlen(str, (*index)+1, ':') - 1;	//"ռһλ 
	data->name = (char*)malloc(len + 1);
	strCpy(str, data->name, (*index)+1, len);
	*index = (*index)+1+len+2;		//�Ƶ���:����һ���ַ�
	switch(str[*index])
	{
		case '\"':			//���Ϊ�ַ�������
			len = getSymlen(str, (*index)+1, '\"');
			data->domain->value = (char*)malloc(len+1);
			strCpy(str, data->domain->value, (*index)+1, len);
			*index = (*index) + 1 + len + 2;
			break;
		case '{':
			index++;
			data->num = 0;
			//data->domain->object = (JSONData*)malloc(sizeof(JSONData));
			while(str[*index] != '}')
			{
				data->domain->object = (JSONData*)realloc(data->domain->object, sizeof(JSONData) * (data->num + 1));
				AnaJSON(str, index, &data->domain->object[data->num]);
				data->num++;
			}
			break;
		case '\0':
			free(data->name);
			return -1;
//			case '[':
//				break;
		default:
			len = getSymlen(str, *index, ',');
			data->domain->value = (char*)malloc(len+1);
			strCpy(str, data->domain->value, (*index)+1, len);
			*index = (*index) + len + 1;
			break;
	}
	return 0;
}

//���㵽�¸����ŵĳ���
static int getSymlen(const char* str, int front, char ch)
{
	int len = 0;
	while(1)
	{
		if(str[front] == '\\')		//�������ת���
		{
			front+=2;
			len+=2;
		}
		else if(str[front++] == ch)	//����ƥ���ַ�w
			break;
		else						//һ�����
			len++;
		if(str[len] == '\0')		//������ַ���ĩβ��δ��⵽�ַ�
			return -1;
	}
	return len;
}

//��ָ��λ�ø����ַ���
static int strCpy(const char* str1, char* str2, int start, int size)
{
	int i = 0;
	if(size <= 0)
	{
		str2[0] = '\0';
		return 1; 
	} 
	
	while(size > 0)
	{
		if(str1[start] == '\\')
		{
			size -= 2;
			start += 2;
			str2[i++] = str1[start - 1];
		}
		else if(str1[start] == '\0')
		{
			str2[i] = '\0';
			if(size > 0)
				return -1;
			else
				return 0;
		}
		else
		{
			str2[i++] = str1[start++];
			size--;
		}
	}
}

//�ͷ�JSONData
void freeJSON(JSONData* data)
{
//	free(data->donamin->domain);
//	free(data->donamin->value);		//����һ�� ����������ͬ
//	free(data->name);
//	free(data);
}
