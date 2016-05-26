# mt7688routerBox

## Usage
### Goto mt7688 Terminal by ssh or serial 
* Modify configure file to change mt7688Duo to router mode **vi /etc/config/network**
<pre>
config interface 'lan'
      option proto 'static'
	  option netmask '255.255.255.0'
	  option ipaddr '192.168.100.1'
<br>
config interface 'wan'
	  option ifname 'eth0'
	  option proto 'dhcp'
</pre>
* Press the WiFi Reset Button for at least 5 seconds and release, then connect to Linkit 7688 AP, now your board can be used as a router 
* First you need to enable yunbridge 
<pre>
> uci set yunbridge.config.disabled='0'
> uci commit
</pre>
* Then setting startup script 
<pre>
> chmod +x script/netDetermineBox
> cp script/netDetermineBox /etc/init.d/
> /etc/init.d/netDetermineBox enable
> /etc/init.d/netDetermineBox start &
> reboot
</pre>


## Using Arduino IDE to upload Arduino sketch
* Download this repository to your computer and copy the Arduino library to your IDE's library path, open routerBox_Arduino.ino with Arduino IDE compile and upload sketch.
