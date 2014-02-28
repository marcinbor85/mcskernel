/* 
 * File:   mcsService.h
 * Author: marcin
 *
 * Created on 27 luty 2014, 21:32
 */

#ifndef MCSSERVICE_H
#define	MCSSERVICE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "mcsTypes.h"
#include "mcsPort.h"
#include "mcsConfig.h"

#define MCS_SERVICE_SIZE_MAX       MCS_CONFIG_SERVICES_MAX
    
typedef enum {
    MCS_SERVICE_TYPE_NOTUSED = 0,
    MCS_SERVICE_TYPE_SINGLE = 1,
    MCS_SERVICE_TYPE_MULTI = 2,
    MCS_SERVICE_TYPE_BUFFERED = 3,
    MCS_SERVICE_TYPE_ASYNC = 4,
} mcsServiceType;

typedef enum {
    MCS_SERVICE_STATE_STOP = 0,
    MCS_SERVICE_STATE_RUN = 1
} mcsServiceState;

typedef struct mcsServiceStruct {
    void (*handle)(struct mcsServiceStruct*,mcsU32);
    mcsU32 period;
    mcsU32 counter;
    mcsU32 runCounter;
    mcsServiceType type;
    mcsServiceState state;    
} mcsServiceStruct;

typedef struct mcsServiceListStruct {
    mcsServiceStruct service[MCS_SERVICE_SIZE_MAX];
    mcsU32 tick;
    mcsServiceState state;    
} mcsServiceListStruct;

void mcsServiceInit(void);
mcsServiceStruct* mcsServiceAdd(void *_handle, mcsU32 _period, mcsServiceType _type, mcsServiceState _state);
mcsBASE mcsServiceIsRun(mcsServiceStruct *_service);
void mcsServiceDel(mcsServiceStruct *_service);
void mcsServiceStart(mcsServiceStruct *_service);
void mcsServiceStop(mcsServiceStruct *_service);
void mcsServiceReset(mcsServiceStruct *_service);
void mcsServiceTickISR(mcsU32 _tick);
void mcsServiceProcess(void);
void mcsServiceLoop(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MCSSERVICE_H */

