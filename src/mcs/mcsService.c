#include "mcsService.h"

mcsServiceListStruct mcsServiceList;

void mcsServiceInit(void) {
    mcsU32 i;
    mcsServiceStruct *service;    
    
    for (i=0; i<MCS_SERVICE_SIZE_MAX; i++) {
        service=&mcsServiceList.service[i];

        service->state=MCS_SERVICE_STATE_STOP;
        service->type=MCS_SERVICE_TYPE_NOTUSED;
        service->handle=mcsNULL;
        service->counter=0;
        service->runCounter=0;
        service->period=0;
    }
    mcsServiceList.tick=0;
    mcsServiceList.state=MCS_SERVICE_STATE_STOP;
    
    mcsPortInit();
}

mcsServiceStruct* mcsServiceAdd(void *_handle, mcsU32 _period, mcsServiceType _type, mcsServiceState _state) {
    mcsServiceStruct *service;
    mcsU32 i;
    for (i=0; i<MCS_SERVICE_SIZE_MAX; i++) {
        service=&mcsServiceList.service[i];
        if (service->type==MCS_SERVICE_TYPE_NOTUSED) {
            service->handle=_handle;
            service->period=_period;            
            service->counter=0;
            service->runCounter=0;
            service->type=_type;
            service->state=_state;
            return service;
        }
    }
    return mcsNULL;
}

mcsBASE mcsServiceIsRun(mcsServiceStruct *_service) {
    return (_service->state==MCS_SERVICE_STATE_RUN)?mcsTRUE:mcsFALSE;    
}


void mcsServiceDel(mcsServiceStruct *_service) {
    mcsPortTickIntOff();
    _service->type=MCS_SERVICE_TYPE_NOTUSED;
    mcsPortTickIntOn();
}

void mcsServiceStart(mcsServiceStruct *_service) {
    mcsPortTickIntOff();
    _service->state=MCS_SERVICE_STATE_RUN;
    mcsPortTickIntOn();
}

void mcsServiceStop(mcsServiceStruct *_service) {
    mcsPortTickIntOff();
    _service->state=MCS_SERVICE_STATE_STOP;
    mcsPortTickIntOn();
}
void mcsServiceReset(mcsServiceStruct *_service) {
    mcsPortTickIntOff();
    _service->state=MCS_SERVICE_STATE_STOP;
    _service->counter=0;
    _service->runCounter=0;
    _service->state=MCS_SERVICE_STATE_RUN;
    mcsPortTickIntOn();
}

void mcsServiceTickISR(mcsU32 _tick) {
    mcsU32 i;
    mcsServiceStruct *service;
    if (mcsServiceList.state==MCS_SERVICE_STATE_STOP) return;
    mcsServiceList.tick=_tick;
    for (i=0; i<MCS_SERVICE_SIZE_MAX; i++) {
        service=&mcsServiceList.service[i];        
        if (service->state==MCS_SERVICE_STATE_STOP) continue;
                
        switch (service->type) {
            case MCS_SERVICE_TYPE_SINGLE:
            case MCS_SERVICE_TYPE_MULTI:                
                service->counter++;
                if (service->counter>=service->period) {
                    service->counter=0;                        
                    service->runCounter=1;
                }
                break;
            case MCS_SERVICE_TYPE_BUFFERED:
                service->counter++;
                if (service->counter>=service->period) {
                    service->counter=0;
                    service->runCounter++;
                }
                break;
        }        
    }
}

void mcsServiceProcess(void) {
    mcsU32 i;
    mcsU32 c;
    mcsServiceStruct *service;
    if (mcsServiceList.state!=MCS_SERVICE_STATE_RUN) return;
    for (i=0; i<MCS_SERVICE_SIZE_MAX; i++) {
        service=&mcsServiceList.service[i];
        if (service->state==MCS_SERVICE_STATE_STOP) continue;
        
        mcsPortTickIntOff();
        c=service->runCounter;
        mcsPortTickIntOn();
        
        switch (service->type) {
            case MCS_SERVICE_TYPE_SINGLE:               
                if (c>0) {                    
                    service->handle(service,mcsPortGetTick());
                    mcsPortTickIntOff();            
                    service->state=MCS_SERVICE_STATE_STOP;
                    service->runCounter=0;
                    mcsPortTickIntOn();        
                }
                break;
            case MCS_SERVICE_TYPE_MULTI:
                if (c>0) {                    
                    service->handle(service,mcsPortGetTick());
                    mcsPortTickIntOff();                            
                    service->runCounter=0;
                    mcsPortTickIntOn();        
                }
                break;
            case MCS_SERVICE_TYPE_BUFFERED:                
                while (c>0) {            
                    service->handle(service,mcsPortGetTick());
                    mcsPortTickIntOff();            
                    c=(--service->runCounter);
                    mcsPortTickIntOn();        
                }                
                break;
            case MCS_SERVICE_TYPE_ASYNC:                
                service->counter++;
                if (service->counter>service->period) {
                    service->counter=0;
                    service->handle(service,mcsPortGetTick());
                }
                break;
        
        }        
    }
}

void mcsServiceLoop(void) {
    mcsServiceList.state=MCS_SERVICE_STATE_RUN;
    while (1) mcsServiceProcess();    
}
