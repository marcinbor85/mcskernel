/* 
 * File:   mcsTypes.h
 * Author: marcin
 *
 * Created on 27 luty 2014, 14:26
 */

#ifndef MCSTYPES_H
#define	MCSTYPES_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef unsigned char mcsU8;
typedef signed char mcsS8;
typedef unsigned short mcsU16;
typedef signed short mcsS16;
typedef unsigned long mcsU32;
typedef signed long mcsS32;

#define mcsBASE mcsU32
#define mcsNULL 0

#define mcsFALSE 0
#define mcsTRUE  (!mcsFALSE)

#ifdef	__cplusplus
}
#endif

#endif	/* MCSTYPES_H */

