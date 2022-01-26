typedef struct json_domain
{
	char* name;							//对象名 
	int num;							//对象数量（为-1时表示对象的值不为为对象） 
	union content 
	{
		char *value;					//一般值 
		struct json_domain* object;		//对象 
	}*domain;			//对象的值 
} JSONData; //JSON数据结构 

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

//解析字符串到JSON
JSONData* StrtoJSON(const char* string);

//释放JSONData
void freeJSON(JSONData* data); 
