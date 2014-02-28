/* 
 * File:   mcsPort.h
 * Author: marcin
 *
 * Created on 27 luty 2014, 21:42
 */

#ifndef MCSPORT_H
#define	MCSPORT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mcsService.h"
#include "mcsTypes.h"
#include "mcsConfig.h"

void mcsPortInit(void);
void mcsPortDeInit(void);
mcsU32 mcsPortGetTick(void);
void mcsPortTickIntOff(void);
void mcsPortTickIntOn(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MCSPORT_H */

