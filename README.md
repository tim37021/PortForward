# Port Forward

pf is a software NAT. I developed it for some hacking purpose originally. It can forward data from one terminal to the other. The terminals need not be clients, they can both be servers. cool? This is useful when the target does not have public IP or when firewall blocks any incoming connection. pf can run on multiple platform such as UNIX-like and even very old Windows machine. Hope you like it.

### Development

This project is still active.

### How to Compile
for windows:

    gcc -static -Wall -o pf.exe main.c thread.c network.c -lws2_32

for Linux/FreeBSD:

    make

### Example Usage
Forwarding remote desktop(RDP) need two commands, pf close immediately when one side of connections break. However, RDP actually connect twice. The first is for authentication .
    
    # if -d(on-demand) provided, order matters!!
    pf -c public_ip 1234 -c 127.0.0.1 3389 -d
    pf -c public_ip 1234 -c 127.0.0.1 3389 -d
    

### TODOs
 - Support host
 - multiple session
 - UDP
 - Lua intergation for parsing incoming data
 - Support for Mac OS X
 
License
----

MIT


