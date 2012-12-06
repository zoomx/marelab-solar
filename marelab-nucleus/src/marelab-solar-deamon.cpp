/*
 ============================================================================
 Name        : marelab-solar-deamon.c
 Author      : 
 Version     :
 Copyright   : Marc Philipp Hammermann (c) 2012
 Description : marelab deamon that is the controller of the system
 ============================================================================
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cwctype>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <time.h>
#include <stdlib.h>
#include <sstream>

#include "mconfig.h"

//#include "marelab/LedString.h"
//#include "marelab/LedTimerListe.h"
#include "marelab/IncomingMsg.h"
#include "json/json.h"
#include "json/writer.h"
#include "json/reader.h"
#include "marelabconf.h"
#include "marelab/ipccom.h"
#include "marelab/PluginRegistry.h"
#include "marelab/MoonPhase.h"

// MODBUS INCLUDES
#include "modbus/modbus.h"
#include "modbus/marelab_modbus.h"
#include "modbus/MBD.h"
#include "modbus/marelab_bus.h"

#include "marelab/ConfigRegister.h"
#include "json/json.h"
#include "marelab/IJsonSerializable.h"
#include "marelab/CJsonSerializer.h"
#include "marelab/mlog.h"






// On Linux, you must compile with the -D_REENTRANT option.  This tells
// the C/C++ libraries that the functions must be thread-safe
#ifndef _REENTRANT
#error ACK! You need to compile with _REENTRANT defined since this uses threads
#endif


using namespace std;

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


/*
 * Registry for all marelab plugins scanned during
 * startup of the deamon
 */
PluginRegistry pluginRegistry;

/*
 * OBJ that are saved to a Config file
 * the ConfigRegister class does the job
 */
Bus *bus = new Bus();				// marelab bus system
//LedTimerListe ledListe;				// List of all LED Strings



ConfigRegister configRegistry;


IncomingMsg   	msgin;				//
time_t 			tim;				// SystemTime

volatile int bStop = false;			// To stop threads

ipccom ipcs;						// Linux IPC Sockets/Queue


static void child_handler(int signum)
{
    switch(signum) {
    case SIGALRM: exit(EXIT_FAILURE); break;
    case SIGUSR1: exit(EXIT_SUCCESS); break;
    case SIGCHLD: exit(EXIT_FAILURE); break;
    }
}

static void daemonize( const char *lockfile )
{
    pid_t pid, sid, parent;
    int lfp = -1;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Create the lock file as the current user */
    if ( lockfile && lockfile[0] ) {
        lfp = open(lockfile,O_RDWR|O_CREAT,0640);
        if ( lfp < 0 ) {
        	string lockf = lockfile;
        	string errortxt =strerror(errno);
        	MLOG::log( "daemonize: unable to create lock file "+lockf+", code=" +i2str(errno)+"("+errortxt+")",__LINE__,__FILE__);
            exit(EXIT_FAILURE);
        }
    }

    /* Drop user if there is one, and we were run as root */
    if ( getuid() == 0 || geteuid() == 0 ) {
        struct passwd *pw = getpwnam(RUN_AS_USER);
        if ( pw ) {
        	string runsuer = RUN_AS_USER;
        	MLOG::log("daemonize: setting user to " + runsuer ,__LINE__,__FILE__);
            setuid( pw->pw_uid );
        }
    }

    /* Trap signals that we expect to recieve */
    signal(SIGCHLD,child_handler);
    signal(SIGUSR1,child_handler);
    signal(SIGALRM,child_handler);

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
    	MLOG::log("daemonize: unable to fork daemon, code=" +i2str(errno)+"("+strerror(errno)+")",__LINE__,__FILE__);
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {

        /* Wait for confirmation from the child via SIGTERM or SIGCHLD, or
           for two seconds to elapse (SIGALRM).  pause() should not return. */
        alarm(2);
        pause();

        exit(EXIT_FAILURE);
    }

    /* At this point we are executing as the child process */
    parent = getppid();

    /* Cancel certain signals */
    signal(SIGCHLD,SIG_DFL); /* A child process dies */
    signal(SIGTSTP,SIG_IGN); /* Various TTY signals */
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGHUP, SIG_IGN); /* Ignore hangup signal */
    signal(SIGTERM,SIG_DFL); /* Die on SIGTERM */

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
    	MLOG::log("daemonize: unable to create a new session, code=" +i2str(errno)+"("+strerror(errno)+")",__LINE__,__FILE__);
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if ((chdir("/")) < 0) {
    	MLOG::log("daemonize: unable to change directory to '/', code=" +i2str(errno)+"("+strerror(errno)+")",__LINE__,__FILE__);
        exit(EXIT_FAILURE);
    }

    /* Redirect standard files to /dev/null */
    freopen( "/dev/null", "r", stdin);
    freopen( "/dev/null", "w", stdout);
    freopen( "/dev/null", "w", stderr);

    /* Tell the parent process that we are A-okay */
    kill( parent, SIGUSR1 );
}

void SendMsgBack(string msg){
	if (ipcs.sendSockServer(msg))
			MLOG::log("Sending message back: "+msg,__LINE__,__FILE__);
	else
			MLOG::log("ERROR sending back message :"+msg,__LINE__,__FILE__);
}



bool ParseIncomingMsg(string msg)
{
	//MLOG::log("ParseIncomingMsg: MSG GETS PARSE: \""+ msg+"\"",__LINE__,__FILE__);

	if (CJsonSerializer::Deserialize(&msgin,msg))
	{
		//MLOG::log("ParseIncomingMsg: MSG parsed successfull...",__LINE__,__FILE__);
		return true;
	}
	else
	{
		MLOG::log("ParseIncomingMsg: MSG parsed failed...",__LINE__,__FILE__);
	}
	return false;
}


/*
 * COMMANDS FROM THE SOCKET THREAD
 */
void mcu_BUSSAVE() {
	printf("PORTS & BUS SAVED\n");

	Json::Value para = msgin.getParameter();
	MLOG::log("mcu_BUSSAVE: " + para.toStyledString(),__LINE__,__FILE__);

	string port = para["PORT"].asString();
	int baud = atoi(para["BAUD"].asString().c_str());
	int databits = atoi(para["DATABITS"].asString().c_str());
	int stopbits = atoi(para["STOPBITS"].asString().c_str());
	string parity = para["PARITY"].asString();

	//printf("WEB MSG BUSSAVE :%s,%d,%d,%s,%d",port.c_str(),baud,databits,parity[0],stopbits);
	bus->setBaud(baud);
	bus->setComport(port);
	bus->setDatabits(databits);
	bus->setParity(parity[0]);
	bus->setStopbits(stopbits);

	configRegistry.writeConfig();

	string retmsg = "{\"COMMAND\":\"mcu_BUSSAVE\",\"PARAMETER\":[]}";
	SendMsgBack(retmsg);
}

/*
 * COMMANDS FROM THE SOCKET THREAD
 */
void mcu_BUSRESTORE() {
	string busstring;
	Json::Value busobj;
	bus->Serialize( busobj);
	busstring = busobj.toStyledString();
	MLOG::log("mcu_BUSRESTORE: Return "+busstring,__LINE__,__FILE__);

	string retmsg = "{\"COMMAND\":\"mcu_BUSSAVE\",\"PARAMETER\":"+busstring+"}";

	SendMsgBack(retmsg);
}

void mcu_PORTSCAN(){
	MLOG::log("mcu_PORTSCAN: PORT SCAN ACTIVATED",__LINE__,__FILE__);

	list<string> l = bus->getComList();
	list<string>::iterator it = l.begin();
	string ports;
	if (!l.empty())
		ports = "{\"port\":\""+*it + "\"}";
	while (it != l.end())
	{
		it++;
		if (it != l.end())
		{
			ports = ports + ",{\"port\":\""+*it + "\"}";
			cout << *it << endl;
		}
	}
	string retmsg = "{\"COMMAND\":\"mcu_PORTSCAN\",\"PARAMETER\":["+ports+"]}";
	SendMsgBack(retmsg);
}

void mcu_VERSION(){
	string ver = nucleusversion;
	string retmsg = "{\"COMMAND\":\"GET_mcu_VERSION\",\"PARAMETER\":\""+ver+"\"}";
	SendMsgBack(retmsg);
}

void mcu_BUSSCAN(){
	string returnval="";
	try{
		MLOG::log("mcu_BUSSCAN: Command:  mcu_BUSSCAN() .. ",__LINE__,__FILE__);
		bus->ScanBus();

		string muuid;
		string name;
		// Generate the List with all BUS members and send it back to the GUI

		int size = bus->getMBdList().size();
		MLOG::log("mcu_BUSSCAN: --Dump MBD LIST------------------------------------",__LINE__,__FILE__);
		for(int i=0; i<size; i++)
		{
			returnval = returnval + "[{\"ADRESS\":\""+ i2str(bus->getMBdList()[i].MBD_getAdress())  + "\"}";
			returnval = returnval + ",{\"MAJOR\":\""+ i2str(bus->getMBdList()[i].MBD_getMajorVersion())  + "\"}";
			returnval = returnval + ",{\"MINJOR\":\""+ i2str(bus->getMBdList()[i].MBD_getMinorVersion())  + "\"}";
			returnval = returnval + ",{\"NAME\":\""+ bus->getMBdList()[i].MBD_getName()  + "\"}";
			if (i== (size-1))
				returnval = returnval + ",{\"UUID\":\""+ bus->getMBdList()[i].MBD_getUUID()  + "\"}]";
			else
				returnval = returnval + ",{\"UUID\":\""+ bus->getMBdList()[i].MBD_getUUID()  + "\"}],";
		}

		string retmsg = "{\"COMMAND\":\"mcu_PORTSCAN\",\"PARAMETER\":["+returnval+"]}";

		SendMsgBack(retmsg);
	}
	catch(string &Exception){
		MLOG::log("mcu_BUSSCAN: "+Exception+"... terminating",__LINE__,__FILE__);
		string retmsg = "{\"COMMAND\":\"mcu_PORTSCAN\",\"PARAMETER\":["+Exception+"]}";
		SendMsgBack(retmsg);
	}

}
/*
 * Scans the plugins and returns a JSON Object of founded Plugins
 */
void  mcu_PLUGINSCAN(){
	string returnval="";
	try{
		MLOG::log("mcu_BUSSCAN: Command:  mcu_BUSSCAN() .. ",__LINE__,__FILE__);
		bus->ScanBus();

		string muuid;
		string name;
		// Generate the List with all BUS members and send it back to the GUI

		int size = bus->getMBdList().size();
		MLOG::log("mcu_BUSSCAN: --Dump MBD LIST------------------------------------",__LINE__,__FILE__);
		for(int i=0; i<size; i++)
		{
			returnval = returnval + "[{\"PLUGINNAME\":\""+ i2str(bus->getMBdList()[i].MBD_getAdress())  + "\"}";
			returnval = returnval + ",{\"PLUGINVERSION\":\""+ i2str(bus->getMBdList()[i].MBD_getMajorVersion())  + "\"}";
			if (i== (size-1))
				returnval = returnval + ",{\"UUID\":\""+ bus->getMBdList()[i].MBD_getUUID()  + "\"}]";
			else
				returnval = returnval + ",{\"UUID\":\""+ bus->getMBdList()[i].MBD_getUUID()  + "\"}],";
		}

		string retmsg = "{\"COMMAND\":\"mcu_BUSSCAN\",\"PARAMETER\":["+returnval+"]}";

		SendMsgBack(retmsg);
	}
	catch(string &Exception){
		MLOG::log("mcu_BUSSCAN: "+Exception+"... terminating",__LINE__,__FILE__);
		string retmsg = "{\"COMMAND\":\"mcu_PORTSCAN\",\"PARAMETER\":["+Exception+"]}";
		SendMsgBack(retmsg);
	}

}

/*
 * Itterates throw the plugin registry and gives back all pluginnames
 */
void PLUGIN_JAVASCRIPTFILES(string command){

	string namelist = pluginRegistry.JSONgetPluginFileNames();
	MLOG::log("PLUGIN_JAVASCRIPTFILES: sending these Plugin Names: "+namelist ,__LINE__,__FILE__);
	string retmsg = "{\"COMMAND\":\"PLUGIN_JAVASCRIPTFILES\",\"PARAMETER\":["+namelist+"]}";
	SendMsgBack(retmsg);
}

void READ_CONFIG(string command, string plugin_name) {
	string output;

	PluginObject* pluginobj = pluginRegistry.GetPluginWithName(plugin_name);
	if (pluginobj!=NULL){
		pluginobj->plugin->GetConfigAsJSON(output);
		//cout << output << endl;
		SendMsgBack(output);
	}
	//
	else{

	}
}

void SAVE_CONFIG(string command, string plugin_name,Json::Value& para) {
	string output;
	PluginObject* pluginobj = pluginRegistry.GetPluginWithName(plugin_name);
	pluginobj->plugin->SetConfigAsJSON(para);
	cout << para.toStyledString()<< endl;
	configRegistry.writeConfig();
	SendMsgBack("{COMMAND=SAVE_CONFIG}");

	/*if (pluginobj!=NULL){
		pluginobj->plugin->SetConfigAsJSON(para);

	}
	else{

	}
	*/
}


// Get Infos of the installed plugins
void GET_PLUGININFO(string command, string plugin_name){
	string output;

	PluginObject* pluginobj = pluginRegistry.GetPluginWithName(plugin_name);
	Json::Value plugins;
	pluginRegistry.SerializeAjax(plugins);
	string plugs = plugins.toStyledString();
	//cout << plugs;
	//string retmsg = "{\"COMMAND\":\"GET_PLUGININFO\",\"PARAMETER\":\""+plugs+"\"}";
	SendMsgBack(plugs);
	//if (plugin_name != ""){
	/*
		if (pluginobj!=NULL){
			output = pluginobj->plugin->getName();
			output = output + " V"+pluginobj->plugin->getVersion() ;
			string retmsg = "{\"COMMAND\":\"GET_PLUGININFO\",\"PARAMETER\":\""+output+"\"}";
			SendMsgBack(retmsg);
		}
		else{

		}
	//}
	 */

}



std::string s2str(string i) {
 std::ostringstream buffer;
 buffer << i;
 return buffer.str();
}


/*
 * sending Log messages back to the client that the deamon has collected up to now
 * remeber that the Client-> Server communication is initiated by the client
 */
void deamon_MLOG()
{
	try
	{
		string logmsg = encodeForJSON(MLOG::getLogMsg());
		logmsg = "\"" + logmsg +"\"";
		string retmsg = "{\"COMMAND\":\"deamon_LOG\",\"PARAMETER\":"+logmsg+"}";
		cout << endl;
		cout << "RETMSG: " << endl;
		cout << retmsg << endl;
		cout << endl;
		//retmsg = "{\"COMMAND\":\"deamon_LOG\",\"PARAMETER\":\"TEST A B C \"}";
		SendMsgBack(retmsg);

	}
	catch(string &Exception){
		MLOG::log("deamon_LOG: ERROR while transmit log : "+Exception+ "...",__LINE__,__FILE__);
	}

}
/*
 * MAIN LOOP FOR COMUNICATION WITH THE MARELAB SYSTEM
 * THE DAEMON HAS A SOKET LISTENER for ipc messages. These ipc Messages controls
 * the action that is send over the marelab bus (USB/RS485) to the mBD Devices.
 * A return is send after the operation back to the sender.
 *
 * Currently a CGI Program converts WEB command task, activated over the web GUI
 * to the deamon. But as long as the sender uses the defined format. Any application
 * able to send ipc messages can control and talk to the marelab system.
 *
 * ALL COMMANDS ARE SEND AS JSON MESSAGE FORMAT
 * COMMUNICATION HANDLING /////////////////////////////////////////////////////////////////
 * ///////////////////////////////////////////////////////////////////////////////////////////
 * //POST: COMMAND='COMMANDNAME' & PARAMETER = '[JSON_ENCODED_PARAMETERARRAY,..]'
 * //JSON_ENCODED_PARAMETERARRAY: [NAME:VALUE,NAMEn:VALUEn]
 */
void *marelab_Socket_thread(void *)
{
	while (1) /*Endlosschleife*/
	{
	// Incoming Msg per socket
	if (ipcs.recvSock()) {
		//MLOG::log("marelab_Socket_thread: input = ["+ipcs.getMsg()+"]",__LINE__,__FILE__);
		if (ParseIncomingMsg(ipcs.getMsg()) == true) {
			string command = msgin.getCommand().asString();

			if (command.compare("EXIT") == 0) {
				MLOG::log("marelab_Socket_thread: marelab deamon exit command ...",__LINE__,__FILE__);

			}

			else if (command.compare("mcu_VERSION") == 0) {
				string ver = nucleusversion;
				MLOG::log("marelab_Socket_thread: mcu_Version:"+ ver +" ...",__LINE__,__FILE__);
				mcu_VERSION();

			}

			/* Scans the bus for mBD Devices and returns them to the GUI */
			else if (command.compare("mcu_BUSSCAN") == 0) {
				MLOG::log("marelab_Socket_thread: mcu_BUSSCAN() ...",__LINE__,__FILE__);
				mcu_BUSSCAN();

			}

			/* Scans the bus for mBD Devices and returns them to the GUI */
			else if (command.compare("mcu_PLUGINSCAN") == 0) {
				MLOG::log("marelab_Socket_thread: mcu_BUSSCAN() ...",__LINE__,__FILE__);
				mcu_PLUGINSCAN();

			}

			/* Get all Ports on the mCU */
			else if (command.compare("mcu_PORTSCAN") == 0) {
				MLOG::log("marelab_Socket_thread: mcu_PORTSCAN() ...",__LINE__,__FILE__);
				mcu_PORTSCAN();

			}

			/* TEST FOR THE NEW CONFIG PERSISTENT LAYER */
			else if (command.compare("mcu_BUSSAVE") == 0) {
				MLOG::log("marelab_Socket_thread: mcu_BUSSAVE() ...",__LINE__,__FILE__);
				mcu_BUSSAVE();

			}
			else if (command.compare("mcu_BUSRESTORE") == 0) {
				MLOG::log("marelab_Socket_thread: mcu_BUSRESTORE() ...",__LINE__,__FILE__);
				mcu_BUSRESTORE();
			}

			/* Send Log Messages to the client */
			else if (command.compare("deamon_LOG") == 0) {
				// Remove log Info b
				//MLOG::log("marelab_Socket_thread: deamon_MLOG() ...",__LINE__,__FILE__);
				deamon_MLOG();

			}



			/*
			 * Plugin Section these functions are imported into the daemon by
			 * dynamic loading of plugins.
			 */
			/* Gets a list of all JavaScript files needed by the installed Plugins */
			else if (command.compare("PLUGIN_JAVASCRIPTFILES") == 0) {
				MLOG::log("marelab_Socket_thread: GET THE PLUGIN_JAVASCRIPTFILES...",__LINE__,__FILE__);
				PLUGIN_JAVASCRIPTFILES(command);
			}
			/* Plugin Info Name & Version */
			else if (command.compare("GET_PLUGININFO") == 0) {
				MLOG::log("marelab_Socket_thread: GET THE PLUGIN INFO ...",__LINE__,__FILE__);
				GET_PLUGININFO(command,msgin.getPlugin());
			}
			/* Read Plugin Config */
			else if (command.compare("READ_CONFIG") == 0) {
				MLOG::log("marelab_Socket_thread: PLUGIN wants to READ_CONFIG ...",__LINE__,__FILE__);
				READ_CONFIG(command, msgin.getPlugin());
			}
			/* Save Plugin Config */
			else if (command.compare("SAVE_CONFIG") == 0) {
				MLOG::log("marelab_Socket_thread: SAVELEDROW(command) ...",__LINE__,__FILE__);
				Json::Value para = msgin.getParameter();
				SAVE_CONFIG(command,msgin.getPlugin(),para);
			}
			else {
				/**
				string output;
				if (CJsonSerializer::SerializeAjax(&ledListe, output)) {
					MLOG::log("marelab_Socket_thread: COMMAND ["+command+"] activated  ...",__LINE__,__FILE__);
					// Sending Back Result for the incoming msg
					if (ipcs.sendSockServer(output))
						MLOG::log("marelab_Socket_thread: Send msg to client..",__LINE__,__FILE__);
				}
				*/
			}/* else {
			 syslog(MLOG_ERR, "COMMAND [%s] unknown  ...",command.c_str());
			 // Sending Back Result for the incoming msg
			 if (ipcs.sendSockServer("RETURN FORM DAEMON"))
			 syslog( MLOG_ERR, "Send msg to client..");
			 }*/
		}
	}
	// Incomig MSG can't be parsed
	else{
		MLOG::log("marelab_Socket_thread: MSG ["+ipcs.getMsg()+"] unknowen  ...",__LINE__,__FILE__);
		// Sending Back Result for the incoming msg
		string retmsg = "Received MSG is unknown by the deamon";
		SendMsgBack(retmsg);
	}
	}
}


void marelab_daemon_entry_point()
{
	MLOG::log( "marelab_daemon_entry_poin: marelab deepblue deamon starting now...",__LINE__,__FILE__ );

	try{
		ipcs.openServer();
	}
	catch(string &Exception){
		MLOG::log( "marelab_daemon_entry_poin: ERROR="+Exception+ " ... terminating deamon...",__LINE__,__FILE__ );

	}


	/* COMMAND RECV THREAD */
	pthread_t mythread;
	int code = pthread_create(&mythread,NULL, marelab_Socket_thread,NULL);

	/* ENDLESS LOOP FOR PROCESSING */
	//int tempp=0;
	MoonPhase moon;


	/////////////////////////////////////////////////////////////////////////
	// This is the Daemon Loop                                             //
	/////////////////////////////////////////////////////////////////////////
	while (1)
	{
		tim=time(NULL);
		tm *timeNow = localtime(&tim);						// Get the time now
		//syslog(MLOG_ERR, "COMMAND [%s] unknown  ...",command.c_str());
		/* DO MARELAB ACTIONS CONTROLLING HARDWARE DEVICES CONECTED TO MARELAB          */
		/* Iterate over the LedChannels and change the dimm values if needed            */

		/******************************************************************
		 * CALLS ALL INSTALLED PLUGINS -> work FUNCTION
		 */


		//for (int counter=0; counter <  ledListe.CountLed(); counter++)
		//	tempp = ledListe.GetPowerValue(counter,timeNow);
		usleep(2000);										// lets sleep a few ticks
	}
}



int main( int argc, char *argv[] ) {
    /* Initialize the logging interface */
	openlog( DAEMON_NAME, LOG_PID, LOG_LOCAL5 );

    MLOG::log("Starting marelab-deepblue-deamon...",__LINE__,__FILE__);


    bool daemon = true;
    if(0 == strcmp(argv[argc-1],"-?"))
    {
    	cout << endl;
    	cout << "Help     -?" << endl;
    	cout << "NoDaemon -ND" << endl;
    	cout << endl;
    	daemon = false;
    	closelog();
    	return 0;
    }
    else if(0 == strcmp(argv[argc-1],"-ND"))
    {
    	MLOG::log("Starting without getting Daemon",__LINE__,__FILE__ );
    	daemon = false;
    }


    /*
     * The Registry for marelab plugins
     */
    pluginRegistry.ScanForPlugins();

    /*
     * Adding all objects that needs config values from the Config File
     */
    configRegistry.addObj(bus);
    pluginRegistry.PluginsAddToConfig(&configRegistry);
    //configRegistry.addObj(&ledListe);

    configRegistry.readConfig();



    if (bus->ConnectBus())
    	bus->ScanBus();


    /*
     *
     * Getting all device plugins
     *



    cout << "Loading symbol sensor devices...\n";

    void* tempsensor = dlopen("/mnt/arm-marelab-dev/marelab-deepblue/marelab-devices/Debug/libmarelab-devices.so", RTLD_LAZY);

    if (!tempsensor) {
       cout << "Cannot open library: " << dlerror() << '\n';
       return 1;
    }


    // load the symbols
  //create_t* create_triangle = (create_t*) dlsym(triangle, "create");
    create_t* create_sensor = (create_t*) dlsym(tempsensor, "create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
    	 cout << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }

    destroy_t* destroy_sensor = (destroy_t*) dlsym(tempsensor, "destroy");
    dlsym_error = dlerror();
    if (dlsym_error) {
    	 cout << "Cannot load symbol destroy: " << dlsym_error << '\n';
        return 1;
    }

    //polygon* poly = create_triangle();
    // create an instance of the class
    sensor* sens = create_sensor();

    // use the class
    string name = sens->getName();
    cout << "Name of loaded pugin: " << name << '\n';

    // destroy the class
    destroy_sensor(sens);

    // unload the triangle library
    dlclose(tempsensor);
     */


    /* One may wish to process command line arguments here */


    /* Daemonize */
    if(daemon)
    	//daemonize( DAEMON_LOCK_PATH DAEMON_NAME_LOCK );

    /* Now we are a daemon -- do the work for which we were paid */
    marelab_daemon_entry_point();

    /* Finish up */
    MLOG::log("marelab deamon exit ...." ,__LINE__,__FILE__);
    closelog();
    return 0;
}
