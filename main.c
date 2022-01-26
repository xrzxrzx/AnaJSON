#include"AnaJSON.h"
#include<stdio.h>
int main(void)
{
	char a[] = "{\"asd\":\"asd\"}";
	JSONData* data = StrtoJSON(a);
	
	puts(data->name);
	puts(data->domain->value);
	
	return 0;
}
