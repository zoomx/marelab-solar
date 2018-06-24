# marelab-solar
Automatically exported from code.google.com/p/marelab-solar



ARM Linux based controll for Aquarium & tanks High Power LED, PH sensor, pump control, fresh water and much more...
Soft & Hardware for embbeded Linux aquarium control

marelab-solar is a soft & hardware project for developing a free and state of the art tank / aquarium control. Everybody is welcomed to help and extend the project, just drop a short mail. Please use the wiki tab for detailed information. The hole development takes place on a Linux PC. You can test the hole package even without Hardware on a Linux PC.
CHECK OUT THE PROJECT WEBSIDE FOR DETAILED INFORMATION
http://www.marelab.org

The hole system depends on a very flexible bus infrastructure with one marelab system control up to 254 tanks or terrariums with one embedded linux board!!!!
Software Base

    Linux OS as solid rock ground ( running form small embedded Hardware to full blown systems)
    Universal core to control any aspect of a reef tank
    Flexible to customize and to enhance with new functions & Hardware over plugins!
    Flexible to use any kind of programming language (C, C++, PHP, JavaScript, PERL, Lua, C#)
    Stable and flexible architecture
    GUI for Touchscreens & Web
    Usable over Tablett PC & Smartphones
    A complete Vmware Image is available for download to get a ready configured development environment in miniutes.

Hardware Base

    Hardware in depend and easy to use other Linux Platforms like rasphery pi, beagle boards & mini2440 as the reference design
    Easy to build Hardware AddOns for reef tank controls & terrariums
    All schematics and pcb design free to use for private projects
    Flexible to enhance or adapt existing Hardware based on arduino

Referenz Design

    Linux ARM mini2440 Device with LCD touchscreen, Network , USB Ports, Buzzer, SD-CARD
    Can be used with USB, RS485 I2C BUS
    Dimmable High Power LED controller that can be extended to unlimmited LED channels to drive unlimited amount of LED strings
    I/O Ports for switching 8 channels per modul (up to 128 Moduls per device)
    Galvanic seperation of Controlling Device & AddOn Hardware

The base ground for the hole development is a free linux OS compiled for a arm based board. The software is total device independent and can be run from a smal embedded Linux system up to a usal linux PC to control all aspects of salt & fresh water tanks. The project is based on the software and hardware part to get a running and absolute unique control system for aquarium fans. As interface an normal WebBrowser or the touch Screen display can be used. The project started because I needed a real customizable and extensible controll that is able to handle large grids of High Power LEDs. The system is a real modular and easy to extend system so over time I will implement all needed controls for all activities controlling a large aquarium system. Please look at the wiki to get a better understanding what and why is done right yet and how the road map is.There you find also all hardware and device plans to build your own system.
A ARM based embedded board with Linux

http://marelab-solar.googlecode.com/svn/wiki/ARM-MARELAB.jpg
The interactive webgui served by the ARM board

http://marelab-solar.googlecode.com/svn/wiki/webgui_marelab1.jpg
Adding unlimited LED strings to easy customize your needs

http://marelab-solar.googlecode.com/svn/wiki/webgui-marelab2.jpg
Our Marelab Linux Distribution on a Tablet

Just control and eddit all your reef tank parameters from your sofa. And yes all this is delivered by our 100US$ bucks linux embedded aqua control!!! http://marelab-solar.googlecode.com/svn/wiki/images/TABLET1.jpg http://marelab-solar.googlecode.com/svn/wiki/images/TABLET2.jpg http://marelab-solar.googlecode.com/svn/wiki/images/TABLET3.jpg
Our Marelab Linux Distribution on a Smartphone

We are getting even more crazy stuff working, you got a smartphone? What about controling your DIY marelab reef control from the smartphone ... thats fun and I need smal fingers :) And yes all this is delivered by our 100US$ bucks linux embedded aqua control!!! We even don't need to change anything to get the smartphone rock the control... I would say thats the power of Linux... http://marelab-solar.googlecode.com/svn/wiki/images/smart1.jpg http://marelab-solar.googlecode.com/svn/wiki/images/smart2.jpg http://marelab-solar.googlecode.com/svn/wiki/images/smart3.jpg
Dimm & Timers for n LED channels to control large grids of led panels for aqua tanks
Integrated temperatur sensing for safe operation
integrated power meter
integrated moon simulation
shematic for controller & LED Dimm circuts
integrated webserver to control the hole system over a ajax interface
SD CARD support
Initial Version based on Embedded Linux for ARM
Plattform lowcost board mini2440
Multi function control for all devices like pumps, ph sensor etc.
Extra confortable web interface to design your LED stips to control
Touch Screen Interface for fast overview & setup
