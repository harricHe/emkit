#ifndef __TYPES_H__
#define __TYPES_H__

#include <stddef.h>

typedef signed char      int8_t;
typedef unsigned char    uint8_t;
typedef signed short     int16_t;
typedef unsigned short   uint16_t;
typedef signed long      int32_t;
typedef unsigned long    uint32_t;
typedef signed long long int64_t;
typedef unsigned long    uint64_t;
typedef float            float32_t;
typedef double           float64_t;
typedef int              bool_t;
typedef int32_t          error_t;
typedef void*            handle_t;

#define TRUE  (1)
#define FALSE (0)
#define OK    (0)
#define NG    (-1)

#define PACK(a, b, c, d) (uint32_t)(((a)<<24) | ((b)<<16) | ((c)<<8) | (d))
#define NULL_SIGNATURE 0x00000000

#endif /* __TYPES_H__ */
