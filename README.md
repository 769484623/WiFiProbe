# WiFiProbe
## May 25th, 2018 Added ##

Depends Missing Problem Sovled. Thanks for Tianyi.

## Introdution
Openwrt WiFi Probe, with http post to send data to server.
You need to have a OpenwrtSDK to compile the source code. When it is done, you will get a IPK like "Scannerxxxxx.ipk".
Switch your wireless network adapter to "monitor" mode, install this IPK, and tape Scanner in the shell. Then the node is sending data to your server.

When you get the IPK, make sure your Openwrt Router firmware has pcap & pthread installed.(Using muti-thread & pcap)

The data it sends is a json-form. Have fun.

About more details, please read my CSDN.
http://blog.csdn.net/sunhaobo1996/

PS:Using libpcap to open an active Network interface, so you need to download libpcap and install it on your computer, or you will meet some .h header missing problems.[Their website](http://www.tcpdump.org/#latest-releases)
