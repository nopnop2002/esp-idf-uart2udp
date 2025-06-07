# esp-idf-uart2udp
UART to UDP bridge for ESP-IDF.   

![Image](https://github.com/user-attachments/assets/a81ce56e-623d-4b42-88eb-07e5c5cb055a)

# Software requirements
ESP-IDF V5.0 or later.   
ESP-IDF V4.4 release branch reached EOL in July 2024.   


# Installation

```
git clone https://github.com/nopnop2002/esp-idf-uart2udp
cd esp-idf-uart2udp/
idf.py menuconfig
idf.py flash
```

# Configuration
![Image](https://github.com/user-attachments/assets/c72c8e65-54ba-4222-a8fa-0bbfd769b6f4)
![Image](https://github.com/user-attachments/assets/6e4fdd49-a535-4304-a2e9-a347c3f70e1f)

## WiFi Setting
Set the information of your access point.   
![Image](https://github.com/user-attachments/assets/6f00ec25-023f-4d48-b864-69a3261bdd0b)

## UART Setting
Set the information of UART Connection.   
![Image](https://github.com/user-attachments/assets/2903cf03-06d3-449f-a032-21d33652ff4d)

## UDP Setting
Set the information of UDP Connection.   
![Image](https://github.com/user-attachments/assets/10be168a-0604-4177-b0c8-4a7967beaccd)

There are the following four methods for specifying the UDP Address.
- Limited broadcast address   
 The address represented by 255.255.255.255, or \<broadcast\>, cannot cross the router.   
 Both the sender and receiver must specify a Limited broadcast address.   

- Directed broadcast address   
 It is possible to cross the router with an address that represents only the last octet as 255, such as 192.168.10.255.   
 Both the sender and receiver must specify the Directed broadcast address.   
 __Note that it is possible to pass through the router.__   

- Multicast address   
 Data is sent to all PCs belonging to a specific group using a special address (224.0.0.0 to 239.255.255.255) called a multicast address.   
 I've never used it, so I don't know anything more.

- Unicast address   
 It is possible to cross the router with an address that specifies all octets, such as 192.168.10.41.   
 Both the sender and receiver must specify the Unicast address.


# Write this sketch on Arduino Uno.   
You can use any AtMega microcontroller.   

```
unsigned long lastMillis = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  while (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    Serial.println(command);
  }

  if(lastMillis + 1000 <= millis()){
    Serial.print("Hello World ");
    Serial.println(millis());
    lastMillis += 1000;
  }

  delay(1);
}
```

Strings from Arduino to ESP32 are terminated with CR(0x0d)+LF(0x0a).   
```
I (1189799) UART-RX: 0x3ffc8458   48 65 6c 6c 6f 20 57 6f  72 6c 64 20 31 30 30 31  |Hello World 1001|
I (1189799) UART-RX: 0x3ffc8468   0d 0a
```

The Arduino sketch inputs data with LF as the terminator.   
So strings from the ESP32 to the Arduino must be terminated with LF (0x0a).   
If the string output from the ESP32 to the Arduino is not terminated with LF (0x0a), the Arduino sketch will complete the input with a timeout.   
The default input timeout for Arduino sketches is 1000 milliseconds.   
If the string received from UDP does not have a LF at the end, this project will add a LF to the end and send it to Arduino.   
The Arduino sketch will echo back the string it reads.   
```
I (1285439) UART-TX: 0x3ffc72f8   61 62 63 64 65 66 67 0a                           |abcdefg.|
I (1285459) UART-RX: 0x3ffc8458   61 62 63 64 65 66 67 0d  0a                       |abcdefg..|
```


# Wireing   
Connect ESP32 and AtMega328 using wire cable   

|AtMega328||ESP32|ESP32S2/S3|ESP32C2/C3/C6|
|:-:|:-:|:-:|:-:|:-:|
|TX|--|GPIO16|GPIO2|GPIO1|
|RX|--|GPIO17|GPIO1|GPIO0|
|GND|--|GND|GND|GND|

__You can change it to any pin using menuconfig.__   


# Windows Application   
I used [this](https://sourceforge.net/projects/sockettest/) app.   
This application runs both a UDP-Listener (UDP-Server) and a UDP-Client simultaneously.   

- Listen from ESP32   
	Specify the port number and press the Start Listening button.   
	This application works as a UDP-Listener (UDP-Server) and communicates with the UDP-Client of ESP32.   
	![Image](https://github.com/user-attachments/assets/30bb42e3-6828-4926-bbe5-7f068ef4a052)   
	![Image](https://github.com/user-attachments/assets/c0ef903a-d0a8-4b68-b653-349fc01ac183)   

- Send to ESP32   
	Specify the IP Address of ESP32 and port number and press the Send button.   
	This application works as a UDP-Client and communicates with the UDP-Server of ESP32.   
	![Image](https://github.com/user-attachments/assets/f1d0002b-4f97-4ef4-8bf9-77eec030551e)   
	Instead of an IP address, you can use an mDNS hostname.   
	![Image](https://github.com/user-attachments/assets/5aafff5a-3bab-481d-b65f-98e3dc6fd25e)   


We can use [this](https://www.hw-group.com/software/hercules-setup-utility) app.   
This application runs both a UDP-Listener (UDP-Server) and a UDP-Client simultaneously.   
![Image](https://github.com/user-attachments/assets/62eb14da-fae5-4aa1-90e7-8d4cb81d500b)   


# Python script   
udp-client.py is UDP-Client.   
udp-server.py is UDP-Listener (UDP-Server).   


# Linux Application
I searched for a GUI tool that can be used with ubuntu or debian, but I couldn't find one.   

# Using linux rsyslogd as logger   
We can forward the UART input to rsyslogd on your Linux machine.
```
+-------------+          +-------------+         +-------------+ 
| Arduino Uno |--(UART)->|    ESP32    |--(UDP)->|  rsyslogd   |
+-------------+          +-------------+         +-------------+ 
```

Configure with port number = 514.   
![Image](https://github.com/user-attachments/assets/bf4cadb5-40d8-49ea-a1b9-dee23fa70c63)

The rsyslog server on linux can receive logs from outside.   
Execute the following command on the Linux machine that will receive the logging data.   
Please note that port 22 will be closed when you enable ufw.   
I used Ubuntu 22.04.   

```
$ cd /etc/rsyslog.d/

$ sudo vi 99-remote.conf
module(load="imudp")
input(type="imudp" port="514")

if $fromhost-ip != '127.0.0.1' and $fromhost-ip != 'localhost' then {
    action(type="omfile" file="/var/log/remote")
    stop
}

$ sudo ufw enable
Firewall is active and enabled on system startup

$ sudo ufw allow 514/udp
Rule added
Rule added (v6)

$ sudo ufw allow 22/tcp
Rule added
Rule added (v6)

$ sudo systemctl restart rsyslog

$ ss -nulp | grep 514
UNCONN 0      0            0.0.0.0:514        0.0.0.0:*
UNCONN 0      0               [::]:514           [::]:*

$ sudo ufw status
Status: active

To                         Action      From
--                         ------      ----
514/udp                    ALLOW       Anywhere
22/tcp                     ALLOW       Anywhere
514/udp (v6)               ALLOW       Anywhere (v6)
22/tcp (v6)                ALLOW       Anywhere (v6)
```

UART input goes to /var/log/remote.   
```
$ tail -f /var/log/remote
Jun  6 11:05:04 Hello World 6721000
Jun  6 11:05:05 Hello World 6722000
Jun  6 11:05:06 Hello World 6723000
Jun  6 11:05:07 Hello World 6724000
Jun  6 11:05:08 Hello World 6725000
Jun  6 11:05:09 Hello World 6726000
Jun  6 11:05:10 Hello World 6727000
Jun  6 11:05:11 Hello World 6728000
Jun  6 11:05:12 Hello World 6729000
Jun  6 11:05:13 Hello World 6730000
Jun  6 11:05:14 Hello World 6731000
Jun  6 11:05:15 Hello World 6732000
```

One advantage of using rsyslogd is that you can take advantage of log file rotation.   
Rotating log files prevents the log files from growing forever.   
The easiest way to rotate logs is to add /var/log/remote to /etc/logrotate.d/rsyslog.   
There are many resources available on the Internet about rotating log files.   
```
$ ls -l /var/log/remote*
-rw-r--r-- 1 syslog syslog    6264 Jun  6 10:35 /var/log/remote
-rw-r--r-- 1 syslog syslog 1219823 Jun  6 10:33 /var/log/remote.1
```


# References

https://github.com/nopnop2002/esp-idf-uart2web

https://github.com/nopnop2002/esp-idf-uart2bt

https://github.com/nopnop2002/esp-idf-uart2mqtt
