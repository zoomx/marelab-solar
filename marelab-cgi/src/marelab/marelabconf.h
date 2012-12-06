/*
 * marelabconf.h
 *
 *  Created on: 28.02.2012
 *      Author: ths
 */

#ifndef MARELABCONF_H_
#define MARELABCONF_H_

/* Change this to whatever your daemon is called */
#define DAEMON_NAME "marelab-nucleus"

/* Change this to whatever your daemon should place his PID lock file */
#define DAEMON_LOCK_PATH "/mnt/arm-marelab-dev/marelab-deepblue/marelab-nucleus/marelab_cfg/"
#define DAEMON_NAME_LOCK "marelab-nucleus-lock"
#define DEAMON_PATH "/mnt/arm-marelab-dev/marelab-deepblue/marelab-nucleus"

/* Change this to whatever your daemon places the ipc*/
//#define DAEMON_PIPE_IN "/home/marelab/msp"
//#define DAEMON_PIPE_OUT "/home/marelab/msp_out"

/* Change this to the user under which to run */
#define RUN_AS_USER "marelab"

#define DAY_SENSE_POINT 48
/* FIFO PIPE BUFFER SIZE */
#define TRANSFER_BUFFER 9192

#define SOCK_PATH "/mnt/arm-marelab-dev/marelab-deepblue/marelab-nucleus/marelab_cfg/marelab_socket"



#define CONFIGFILE "/mnt/arm-marelab-dev/marelab-deepblue/marelab-nucleus/marelab_cfg/marelabsolar.cfg"
//#define CONFIGFILETEST "/mnt/arm-marelab-dev/marelab-deepblue/marelab-nucleus/marelab_cfg/marelabsolartest.cfg"

#define PLUGINDIR "/mnt/arm-marelab-dev/marelab-deepblue/marelab-nucleus/plugins/"

#define nucleusversion "Nucleus 0.1"

#endif /* MARELABCONF_H_ */
