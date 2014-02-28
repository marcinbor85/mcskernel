/* 
 * File:   main.c
 * Author: marcin
 *
 * Created on 27 luty 2014, 21:26
 */

#include <stdio.h>
#include <stdlib.h>

#include "mcs/mcsService.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>


mcsServiceStruct *h1;
mcsServiceStruct *h2;
mcsServiceStruct *h3;
mcsServiceStruct *h4;
mcsServiceStruct *h5;

 
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}
 

void handler1(mcsServiceStruct *_serv, mcsU32 _t) {
    printf("[1] at %08d: TIMEOUT\n",_t);
}

void handler2(mcsServiceStruct *_serv, mcsU32 _t) {
    printf("[2] at %08d: TIMEOUT\n",_t);
}

void oneShot(mcsServiceStruct *_serv, mcsU32 _t) {
    printf("[4] at %08d: SINGLE SHOT\n",_t);
    mcsServiceDel(_serv);
}

void idle(mcsServiceStruct *_serv, mcsU32 _t) {
    printf("[5]");
}

void handler3(mcsServiceStruct *_serv, mcsU32 _t) {
    char c;
    mcsServiceStruct *h;
    if (kbhit()) {
        c=getchar();
        printf("[3] at %08d: key pressed\n",_t);
        if (c=='1') {
            if (mcsServiceIsRun(h1)) {
                mcsServiceStop(h1);
            } else {
                mcsServiceReset(h1);
            }
        }
        if (c=='2') {
            if (mcsServiceIsRun(h2)) {
                mcsServiceStop(h2);
            } else {
                mcsServiceReset(h2);
            }
        }
        if (c=='5') {
            if (mcsServiceIsRun(h5)) {
                mcsServiceStop(h5);
            } else {
                mcsServiceReset(h5);
            }
        }
        if (c=='4') {
            h=mcsServiceAdd(oneShot, 500*MCS_CONFIG_MS, MCS_SERVICE_TYPE_SINGLE,MCS_SERVICE_STATE_RUN);            
            if (h!=mcsNULL) {
                h4=h;
            }
        }
        if (c==27) {
            exit(0);
        }
    }  
}



int main(int argc, char** argv) {

    mcsServiceInit();
    
    h1=mcsServiceAdd((void*)handler1,1000*MCS_CONFIG_MS,MCS_SERVICE_TYPE_MULTI,MCS_SERVICE_STATE_RUN);
    h2=mcsServiceAdd((void*)handler2,300*MCS_CONFIG_MS,MCS_SERVICE_TYPE_MULTI,MCS_SERVICE_STATE_RUN);
    h3=mcsServiceAdd((void*)handler3,10*MCS_CONFIG_MS,MCS_SERVICE_TYPE_MULTI,MCS_SERVICE_STATE_RUN);
    
    h5=mcsServiceAdd((void*)idle,10000,MCS_SERVICE_TYPE_ASYNC,MCS_SERVICE_STATE_STOP);
            
    mcsServiceLoop();
    
    return (EXIT_SUCCESS);
}


