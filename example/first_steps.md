# Ixxat SocketCAN First Steps



## Installation

To install the Ixxat kernel module, please refer the README.md provided with the
driver sources. 

If the kernel module is installed, configure the Ixxat CAN network interface with, e.g.:

    $ sudo ip link set canX type can bitrate 500000

CAN network devices are usually named ‘canX’, like can0, can1, can2 …

The CAN framework contains an algorithm to calculate possible bit
timing parameters based on the requested bit rate and the constraints
of the underlying CAN hardware.

Start the Ixxat CAN network interface with:

    $ sudo ip link set canX up

Configuration and start of a CAN interface can be combined into one command:

    $ sudo ip link set canX up type can bitrate 500000

To test if the CAN network interface is available, use:

    $ ifconfig

Or:

    $ ip link show


Stop a CAN network interface:

    $ sudo ip link set canX down




## CAN-utils

CAN-utils is a collection of useful tools using the SocketCAN interface.
It includes applications such as:

- candump : display, filter and log CAN data to files
- canplayer : replay CAN logfiles
- cansend : send a single frame
- cangen : generate (random) CAN traffic
- cansequence : send and check sequence of CAN frames with incrementing payload
- cansniffer : display CAN messages and highlight changes in the data bytes

Install the `can-utils` with:

    $ sudo apt install can-utils

Use the CAN-utils library to test the CAN communication. You need two CAN
devices connected to each other to run the test.

To receive CAN messages on one side, enter:

    $ candump canX

For example to send a message with identifier 0x123 and 
payload [0xDE, 0xAD, 0xBE, 0xEF], enter on the other side:

    $ cansend canX 123#DEADBEEF



## CAN Examples

Sample C code for SocketCAN is publicly available and can be downloaded 
from [here](https://github.com/craigpeacock/CAN-Examples/archive/master.zip). 
This [article](https://www.beyondlogic.org/example-c-socketcan-code/) describes the code in detail.
To use this examples, download and unpack the ZIP archive. 
To test the code with a real CAN interfaces, the code has to be modified.
Change the CAN interface name (approximately in line 29):

```c
strcpy(ifr.ifr_name, "vcan0");    // <-- FROM
strcpy(ifr.ifr_name, "can0");  // <-- TO e.g.
```


Open a terminal and use gcc to build the examples:

    $ gcc cantransmit.c -o cantransmit
    $ gcc canreceive.c -o canreceive
    $ gcc canfilter.c -o canfilter


If compilation errors occur, try:

    $ sudo apt install --reinstall build-essential

Before you execute the examples, check if both interfaces are up with:

    $ ip link show

To receive CAN messages on one side, enter:

    $ ./canreceive

And in a second terminal on the other side, enter:

    $ ./cantransmit



### Transmit/Receive Demo

Part of the delivery is the example C code `candemo.c`. 
It shows how to transmit and receive CAN messages in non-blocking mode.

Build the executable:

    $ gcc candemo.c -o candemo

Start the demo:

    $ ./candemo canX



### CAN-FD Demo

There is  second example `canfddemo.c` with sends and receives 
CAN-FD messages with more than 8 data bytes payload.
To be able to use this demo, re-configure your SocketCAN interface:

Stop the interface:

    $ sudo ip link set canX down

Start the interface again in CAN-FD mode with e.g. 500 KBit/s arbitration rate and 2000 KBit/s data rate:

    $ sudo ip link set canX up type can bitrate 500000 dbitrate 2000000 fd on

Build the executable:

    $ gcc canfddemo.c -o canfddemo

Please consider that you have to re-configure both sides. Start the CAN-FD demo with:

    $ ./canfddemo canX

The `candump` tool can also receive CAN-FD messages. You can start it on the second device:

    $ candump canX




## SocketCAN Documentation

[SocketCAN - Controller Area Network]
(https://www.kernel.org/doc/html/latest/networking/can.html#) is the official
documentation for the SocketCAN interface.



## Main difference to traditional CAN interface APIs

With SocketCAN the CAN interface can be seen like an Ethernet network interface
which is available to several applications at the same time. Therefore:

- Setup of the the CAN adapter is done on system level via network command
  `ip link` and not by individual applications
- You need root access to configure the CAN adapter
- The Berkeley socket API with `socket`, `bind`. `read`, and `write` has to be
  used instead of the functions like `ECI_SendCanMsg`
- Standard tools like `wireshark` can be used to analyze the bus traffic
  instead of dedicated tools provided by the hardware supplier.



## Copyright

(C) 2021 HMS Technology Center Ravensburg GmbH  
Helmut-Vetter-Strasse 2  
88213 Ravensburg  
Germany

This readme is a compilation of several sources:

- [Example C SocketCAN Code](https://www.beyondlogic.org/example-c-socketcan-code/)
- [CAN Interface Modules](https://python-can.readthedocs.io/en/master/interfaces/socketcan.html)
- [SocketCAN - Controller Area Network](https://www.kernel.org/doc/html/latest/networking/can.html)
- [can-utils](https://github.com/linux-can/can-utils)

All copyrights are owned by the original authors.

