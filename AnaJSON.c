#include"AnaJSON.h"
#include<malloc.h>

/*
计算到下个符号的长度
@str 	需操作的字符串
@front  字符串起始索引
@ch		需检测的字符
*/
static int getSymlen(const char* str, int front, char ch);

/*
计算拥有多少个对象
@*/
static int getObjnum(const char* str, int start);

/*
从指定位置复制字符串
@str1	需操作的字符串
@str2	复制后的字符串
@start	起始位置
@size	复制的大小
*/
static int strCpy(const char* str1, char* str2, int start, int size);

/*
解析JSON
@*/
static int AnaJSON(const char* str, int* index, JSONData* data);

//解析字符串到JSON
JSONData* StrtoJSON(const char* string)
{
	int index = 0;
	if(!string || string[0] != '{')		//判断是否是正常JSON开头
		return NULL;

	JSONData* data;

	AnaJSON(string, &index, data);
	return data;
}

//解析JSON
static int AnaJSON(const char* str, int* index, JSONData* data)
{
	int len = -1;
	
	//初始化
	data = (JSONData*)malloc(sizeof(JSONData));
	data->num = -1;
	data->domain = NULL;
	data->name = NULL;
	
	len = getSymlen(str, (*index)+1, ':') - 1;	//"占一位 
	data->name = (char*)malloc(len + 1);
	strCpy(str, data->name, (*index)+1, len);
	*index = (*index)+1+len+2;		//移到“:”后一个字符
	switch(str[*index])
	{
		case '\"':			//如果为字符串类型
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

//计算到下个符号的长度
static int getSymlen(const char* str, int front, char ch)
{
	int len = 0;
	while(1)
	{
		if(str[front] == '\\')		//如果遇到转义符
		{
			front+=2;
			len+=2;
		}
		else if(str[front++] == ch)	//遇到匹配字符w
			break;
		else						//一般情况
			len++;
		if(str[len] == '\0')		//如果到字符串末尾任未检测到字符
			return -1;
	}
	return len;
}

//从指定位置复制字符串
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

//释放JSONData
void freeJSON(JSONData* data)
{
//	free(data->donamin->domain);
//	free(data->donamin->value);		//与上一行 代码作用相同
//	free(data->name);
//	free(data);
}
