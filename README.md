# spw_package_decode

Software for recording SpaceWire traffic using a STAR-Dundee SpaceWire Link Analyzer Mk3 and decoding the FEE data-packets used in the PLATO project.

## Options

`spw_package_decode [-f?] [-c EN_CHARS] [-p MILLIS] [-r RECV] SERIAL_NO SECONDS`

| Option | Argument  | Type    | Description                                                                                                                       |
| ------ | --------- | ------- | --------------------------------------------------------------------------------------------------------------------------------- |
| -f     | none      | none    | Flag for using FCTs as the trigger Event instead of Timecodes.                                                                    |
| -c     | EN_CHARS  | integer | Enables SpaceWire characters to be recorded by the LinkAnalyser. The integer input (0-15) is interpreted as a binary value with each bit serving as an enable flag for one type of character. The first bit (LSB) is enabling NChars, the second bit is enabling Timecodes, the third bit is enabling FCTs and the MSB is enabling NULL codes. |
| -p     | MILLIS    | integer | Determines the maximum time period in milliseconds before the trigger, for which recorded packets will be printed to the hexdump. |
| -r     | RECV      | char    | Determines on which of it's receivers the Link Analyser will wait for the trigger event ('A' or 'B').                             |
| none   | SERIAL_NO | integer | The serial number of the Link Analyser recording the data traffic.                                                                |
| none   | SECONDS   | double  | The duration in seconds to be recorded after the Link Analyser has been triggered.                                                |

## Hexdump Format

The hexdump consists of a header containing meta data relevant for the recording and the packet based recording of the data traffic. Each packet is preceded by a timestamp. The first line of data in a packet forms the 12-bit packet header.

```
# Trigger timestamp:   2022-03-08T20:14:02.59896
# Software version:    spw_package_decode v0.2.0

### Configuration
# Record duration:     5s
# PreTrig duration:    3000ms
# Trigger event:       Timecode
# Enable NULLs:        0
# Enable FCTs:         1
# Enable Timecodes:    1
# Enable NChars:       1

### Link Analyser
# API version:         v5.01
# Device name:         FEE_1
# Serial number:       32190444
# Device version:      v1.01 edit 8
# Firmware version:    v1.05
# Build date:          19-01-10 14:43


I 2022-03-08T20:14:00.261377                    <Incoming Packet>
000000 50 F0 00 80 02 83 7F 9A 00 00 00 2E      <Header>
00000C 00 00 00 00 00 00 00 00
000014 00 00 00 00 07 7F 9B 00
00001C 00 00 00 00 00 00 00 00
000024 00 00 00 00 00 00 00 00
00002C 00 00 00 00 00 00 00 00
000034 00 00 00 00 00 00 00 00
00003C 00 00 00 00 00 00 00 00
000044 00 00 00 00 00 00 00 00
00004C 00 00 00 00 00 00 00 00
000054 00 00 00 00 00 00 00 00
00005C 00 00 00 00 00 00 00 00
000064 00 00 00 00 00 00 00 00
00006C 00 00 00 00 00 00 00 00
000074 00 00 00 00 00 00 00 00
00007C 00 00 00 00 00 00 00 00
000084 00 00 00 00 00 00 00 3B

O 2022-03-08T20:14:01.561410                    <Outgoing Packet>
000000 50 F0 00 18 02 82 7F 9A 00 01 00 49      <Header>
00000C 00 00 F0 00 00 00 00 47
000014 A7 56 A3 00 00 00 00 00
00001C 00 00 00 00 00 00 00 18

...
<empty line at end of file>
```

## Timestamp Format
The packets in the hexdump are marked with an absolute timestamp of the format `"%FT%T.%f"` (e.g. 2022-03-14T13:07:25.513545), although in some versions of Wireshark, only `"%FT%T."` is accepted to specify this format. The precision of the decimal seconds to be read by Wireshark is variable. The Link Analyser Mk3 is capable of measureing time with 100MHz clock resulting in a 10 nanosecond acuracy.

## Commands

Collection of some usefull helpfull commands for using this software.

### Recording data to hexdump:

`spw_package_decode <serial number> <seconds> > hexdump.txt`

### Parsing a hexdump:

`text2pcap -D -t "%FT%T." hexdump.txt hexdump.pcap`

### Importing a hexdump to Wireshark:

`wireshark -t r -X lua_script:plato.lua hexdump.pcap`