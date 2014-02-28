/* 
 * File:   mcsConfig.h
 * Author: marcin
 *
 * Created on 27 luty 2014, 22:00
 */

#ifndef MCSCONFIG_H
#define	MCSCONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define MCS_CONFIG_SERVICES_MAX   16

#define MCS_CONFIG_TICK_PERIOD_US 100    
#define MCS_CONFIG_MS             (1000/MCS_CONFIG_TICK_PERIOD_US)

#ifdef	__cplusplus
}
#endif

#endif	/* MCSCONFIG_H */

