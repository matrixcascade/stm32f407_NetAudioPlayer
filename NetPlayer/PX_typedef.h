#ifndef __PX_TYPEDEF_H
#define __PX_TYPEDEF_H

#define     _IN
#define     _OUT
#define     PX_FALSE			0
#define     PX_TRUE				1
#define		PX_NULL				0
#define     PX_PI				3.14159265359f

typedef char * _px_va_list;
#define __PX_INTSIZEOF(n) ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define _px_va_start(ap,v) ( ap = (_px_va_list)&v + __PX_INTSIZEOF(v) )
#define _px_va_arg(ap,t) ( *(t *)((ap += __PX_INTSIZEOF(t)) - __PX_INTSIZEOF(t)) )
#define _px_va_end(ap) ( ap = (_px_va_list)0 )

typedef		void				px_void;
typedef		int					px_bool;
typedef		unsigned int		px_dword;
typedef     short               px_short;
typedef     unsigned short		px_word;
typedef		unsigned short		px_ushort;
typedef     unsigned int		px_uint;
typedef     int					px_int;
typedef		char				px_char;
typedef		char				px_byte;
typedef		unsigned char		px_uchar;
typedef		unsigned long		px_ulong;
typedef		long				px_long;
typedef     float				px_float;
typedef     double				px_double;
typedef     long long           px_qword;

typedef     unsigned long long  px_u64;
typedef     unsigned int        px_u32;
typedef     unsigned short      px_u16;
typedef     unsigned char       px_u8;


#endif
