#include"AnaJSON.h"
#include<malloc.h>
#include<string.h>

/*
���㵽�¸����ŵĳ���
@str 	��������ַ���
@front  �ַ�����ʼ����
@ch		������ַ�
*/
static int getSymlen(const char* str, int front, char ch);

/*
��ָ��λ�ø����ַ���
@str1		��������ַ���
@str2		���ƺ���ַ���
@start		��ʼλ��
@size		���ƵĴ�С
*/
static int strCpy(const char* str1, char* str2, int start, int size);

/*
����JSON
@str		JSON�ַ���
@index		��ǰ����
@JSONData	JSONData����
@n			��ǰJSONData�Ӷ��������
*/
static int AnaJSON(const char* str, int index, JSONData* data, int n);

//�����ַ�����JSON
JSONData* StrtoJSON(const char* string)
{
	int n = 0;
	int index = 1;
	if(!string || string[0] != '{')		//�ж��Ƿ�������JSON��ͷ
		return NULL;

	JSONData* data = (JSONData*)malloc(sizeof(JSONData));
	data->domain = (JSONContent*)malloc(sizeof(JSONContent));
	data->domain->object = NULL;

	//���ø��ڵ������
	data->name = (char*)malloc(5);
	strCpy("root", data->name, 0, 4);

	AnaJSON(string, index, data, n);
	return data;
}

//����JSON
static int AnaJSON(const char* str, int index, JSONData* data, int n)
{
	int len;
	int ns = 0;
	//��ʼ��
	if(n == 0)
	{
		data->domain->object = (JSONData*)malloc(sizeof(JSONData));
		data->num = 0;
		n = 1;
	}
	else
	{
		data->domain->object = (JSONData*)realloc(data->domain->object, sizeof(JSONData)*n);
		data->num = n;
	}
	data->domain->object[n-1].num = 0;
	data->domain->object[n-1].domain = NULL;
	data->domain->object[n-1].name = NULL;

	len = getSymlen(str, index+1, ':') - 1;	//"ռһλ
	data->domain->object[n-1].name = (char*)malloc(len + 1);
	strCpy(str, data->domain->object[n-1].name, index+1, len);
	index = index+1+len+2;		//�Ƶ���:����һ���ַ�
	switch(str[index])
	{
		case '\"':			//���Ϊ�ַ�������
			len = getSymlen(str, index+1, '\"');
			data->domain->object[n-1].domain = (JSONContent*)malloc(sizeof(JSONContent));
			data->domain->object[n-1].domain->value = (char*)malloc(len + 1);
			strCpy(str, data->domain->object[n-1].domain->value, index+1, len);
			index = index + 1 + len + 1;
			break;
		case '{':			//�������� 
			index++;
			data->domain->object[n-1].domain = (JSONContent*)malloc(sizeof(JSONContent));
			index = AnaJSON(str, index, &data->domain->object[n-1], ns);
			data->domain->object[n-1].num++;
			break;
		case '\0':			//�����ַ�����β 
			free(data->domain->object[n-1].name);
			return -1;
//		case '[':
//			break;
		default:			//�������ͻ򲼶����� 
			len = getSymlen(str, index, ',');
			if(str[index+len-1] == '}')
				len = getSymlen(str, index, '}');
			if(len == -1)
			{
				len = getSymlen(str, index, '}');
				data->domain->object[n-1].domain = (JSONContent*)malloc(sizeof(JSONContent));
				data->domain->object[n-1].domain->value = (char*)malloc(len+1);
				strCpy(str, data->domain->object[n-1].domain->value, index, len);
				index = index + len;
				break;
			}
			data->domain->object[n-1].domain = (JSONContent*)malloc(sizeof(JSONContent));
			data->domain->object[n-1].domain->value = (char*)malloc(len+1);
			strCpy(str, data->domain->object[n-1].domain->value, index, len);
			index = index + len;
			break;
	}
//	index++;
	switch(str[index])
	{
		case '}':
		case '\0':
			index++;
			break;
		case ',':
			index = AnaJSON(str, ++index, data, ++n);
			break;
		default:
			return -1;
	}
	
	return index;
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
	str2[i] = '\0';
}

//�ͷ�JSONData
void freeJSON(JSONData* data)
{
	int i;
	JSONData *con;

	con = data;
	free(con->name);
	if(con->num == 0)
		free(con->domain->value);
	else
	{
		for(i = 0; i < con->num; i++)
		{
			freeJSON(&con->domain->object[i]);
		}
	}
	free(con);
}

//��ȡ�����ֵ
int getJSONVal(JSONData* data, const char* name, char* val1, JSONData* val2)
{
	int i;
	if(data->num == 0)			//����ö����ֵΪ�ַ��� 
	{
		strcpy(val1, data->domain->value);
		puts("1");
		return 1;
	}
	else						//����ö����ֵΪ���� 
	{
		for(i = 0; i < data->num; i++)		//�Ƚ��Ӷ��� 
		{
			puts("3");
			if(!strcmp(name, data->domain->object[i].name))
			{
				if(data->domain->object[i].num != 0)	//����ö����ֵΪ���� 
				{
					printf("num: %d\n", data->domain->object[i].num);
					puts("2");
					if(val2 == NULL)
						return -1;
					val2 = &data->domain->object[i];
					return 2;
				}
				else									//����ö����ֵΪ�ַ��� 
				{
					strcpy(val1, data->domain->object[i].domain->value);
					return 1;
				}
			}
		}
	}
	return 0;
}
