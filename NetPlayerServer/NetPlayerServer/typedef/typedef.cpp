#include "typedef.h"

pt_char * __PT_STRCPY(pt_char *dest,const pt_char *Source)
{
	if(Source[0]!='\0')
	return strcpy(dest,Source);

	return "";
}

pt_char * __PT_STRCAT(pt_char *dest,const pt_char *Source)
{
	if(Source[0]!='\0')
	return strcat(dest,Source);

	return "";
}

pt_char * __PT_ITOA(_In_ pt_int _Val, _Pre_notnull_ _Post_z_ pt_char * _DstBuf, _In_ pt_int _Radix)
{
	return itoa(_Val,_DstBuf,_Radix);
}
