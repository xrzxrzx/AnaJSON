typedef union content 
{
	char *value;					//һ��ֵ 
	struct json_domain* object;		//���� 
}JSONContent;

typedef struct json_domain
{
	char* name;							//������ 
	int num;							//����������Ϊ-1ʱ��ʾ�����ֵ��ΪΪ���� 
	JSONContent *domain;						//�����ֵ 
} JSONData; //JSON���ݽṹ 

//NULL from stdio.h
#ifndef NULL
#ifdef __cplusplus
#ifndef _WIN64
#define NULL 0
#else
#define NULL 0LL
#endif  /* W64 */
#else
#define NULL ((void *)0)
#endif
#endif

//�����ַ�����JSON
JSONData* StrtoJSON(const char* string);

//�ͷ�JSONData
void freeJSON(JSONData* data);

/*
��ȡ�����ֵ
@data		JSON���� 
@name		JSON������
@val1		д���ֵ
@val2		д��Ķ��� 
*/
int getJSONVal(JSONData* data, const char* name, char* val1, JSONData* val2);
