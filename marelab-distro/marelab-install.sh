#!/bin/bash 
# marelab aqua installation script

VERSION="1.0"
DEFAULT_USER="marelab"
DEFAULT_PASSWORD="password"
MARELAB_REPO="https://code.google.com/p/marelab-solar"
PI_REPO="/marelab-aqua-pi"
MARELAB_BASE_DIR=""
MARELAB_OS=""

function _getinstalldir(){
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  TARGET="$(readlink "$SOURCE")"
  if [[ $SOURCE == /* ]]; then
 #   echo "SOURCE '$SOURCE' is an absolute symlink to '$TARGET'"
    SOURCE="$TARGET"
  else
    DIR="$( dirname "$SOURCE" )"
  #  echo "SOURCE '$SOURCE' is a relative symlink to '$TARGET' (relative to '$DIR')"
    SOURCE="$DIR/$TARGET" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
  fi
done
#echo "SOURCE is '$SOURCE'"
RDIR="$( dirname "$SOURCE" )"
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
#if [ "$DIR" != "$RDIR" ]; then
 # echo "DIR '$RDIR' resolves to '$DIR'"
#fi
#echo "DIR is '$DIR'"
MARELAB_BASE_DIR=$DIR
}

function _checkingPacket(){
	problem=$(dpkg -s $1|grep installed)
 	echo Checking for $1 : $problem
 	if [ "" == "$problem" ]; then
      	echo "No $1 installed. installing it now..."
      	sudo apt-get install $1
 	fi
 }

function _cleanup(){
    echo "removing installation ..."
    echo "bye bye ..."
	rm -rf $MARELAB_BASE_DIR/marelab-aqua
}

function _fail ( ){   
        echo -e $@
        _cleanup                                  
        exit 1                      
}  


function _makedir(){
 	echo "create $1 dir ..."
 	TESTDIR=$1
 	if [ -d "$1" ]; then
        echo " "
        _fail "ERROR: $1 dir couln't created ... \n"
        exit
  	fi
 	mkdir $1
 	return 0
}

function _downloadMarelab(){
 #finding if linux86 or arm like pi beagle mini2440 etc.
 foundos=$(uname -m)
 if [ "$foundos" == "i686" ]; then
    MARELAB_OS="PC"
 fi
 if [ "$foundos" == "armv6l" ]; then
    MARELAB_OS="ARM"
 fi
 if [ "$MARELAB_OS" == "PC" ]; then
    echo "downloading linux pc based version of marelab..."
 fi
 if [ "$MARELAB_OS" == "ARM" ]; then
    echo "downloading linux arm based version of marelab..."
 fi
}




function _testdir(){
#Does this directory exist?
TEST_DIR=$1
if [ -d "$TEST_DIR" ]; then
    echo $TEST_DIR already exists. Exiting!
    exit
fi
}

function _testfile(){
#Does file exist?
    PERL_SOURCE_ZIP_FILE=%1
    if [ ! -f "$PERL_SOURCE_ZIP_FILE" ]; then
        wget http://www.cpan.org/src/5.0/$PERL_SOURCE_ZIP_FILE
    fi
}

function _isvarempty(){
#Is variable empty?
X=abc
echo "$X"
if [ "$X" == "" ]; then
    echo yes
fi
}

function _pre_install(){
 echo "######## marelab Pre installation ################################"
 
 _getinstalldir  	# get the dir the script is in thats the base for all installation & removing 
 
 # check if sudo / root called it
 if [ `/usr/bin/id -u` != "0" ]; then
        echo " "
        _fail "ERROR: Sorry you must be root to run this script exp: ( sudo marelab-install.sh ) \n"
        exit
 fi

 _makedir "marelab-aqua"
  cd marelab-aqua
 _makedir "marelab-web"
 _makedir "marelab-bin"
 _makedir "marelab-conf"
 _makedir "marelab-plug"
 _makedir "temp-install"
 
 #Check the needed packages and install them
 	_checkingPacket "boa"
	_checkingPacket "lua5.2"
	_checkingPacket "git"

echo "downloading marelab from marelab.org git repository ..."
	_downloadMarelab
	
	
#CONFIGURE BOA FOR MARELAB
/etc/init.d/boa stop
#CONFIGURE /etc/boa/boa.conf
echo "configure boa webserver for marelab use ..."
/etc/init.d/boa start

#CONFIGURE LINUX LOGGING DEAMON 
/etc/init.d/rsyslog stop
#ADD NEW MARELAB to rssyslog conf
echo "configure rsyslog deamon for for marelab use ..."
/etc/init.d/rsyslog start 
	
#CREATE MARELAB USER
echo "creating user:  "

#CREATE DEMON
#CREATE MARELAB DEAMON CONF
echo "configure marelab deamon ..."

}

# Detect package type from /etc/issue
function _found_arch() {
  local _ostype="$1"
  shift
  grep -qis "$*" /etc/issue && _OSTYPE="$_ostype"
}



clear

echo "marelab aqua install version: $VERSION "
echo "---------------------------------------------------------------"
_pre_install

