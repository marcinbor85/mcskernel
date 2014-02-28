#include "mcsPort.h"

#include <sys/time.h>
#include <signal.h>

mcsU32 tick;

struct itimerval timValue;
struct sigaction newHandler;
struct sigaction oldHandler;

void timHandler(mcsBASE _arg) {
    tick++;
    mcsServiceTickISR(tick);
}

void mcsPortTickIntOff(void) {
    
}
void mcsPortTickIntOn(void) {
    
}

mcsU32 mcsPortGetTick(void) {
    mcsU32 t;
    mcsPortTickIntOff();
    t=tick;
    mcsPortTickIntOn();
    return t;
}

void mcsPortInit(void) {
    mcsPortTickIntOff();
    
    tick=0;
    
    timValue.it_interval.tv_sec = MCS_CONFIG_TICK_PERIOD_US / 1000000;
    timValue.it_interval.tv_usec = MCS_CONFIG_TICK_PERIOD_US % 1000000;
    timValue.it_value.tv_sec = MCS_CONFIG_TICK_PERIOD_US / 1000000;
    timValue.it_value.tv_usec = MCS_CONFIG_TICK_PERIOD_US % 1000000;    
    setitimer(ITIMER_REAL, &timValue, 0);

    newHandler.sa_handler = (void*)&timHandler;
    newHandler.sa_flags = SA_NOMASK;    
    sigaction(SIGALRM, &newHandler, &oldHandler);
    
    mcsPortTickIntOn();
}

void mcsPortDeInit(void) {
    
    mcsPortTickIntOff();
            
    timValue.it_interval.tv_sec = 0;
    timValue.it_interval.tv_usec = 0;
    timValue.it_value.tv_sec = 0;
    timValue.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &timValue, 0);

    sigaction(SIGALRM, &oldHandler, 0);
}
