# spw_package_decode

Software for recording SpaceWire traffic using a STAR-Dundee SpaceWire Link Analyzer Mk3 and decoding the FEE data-packets used in the PLATO project.


## Hexdump Format

The hexdump consists of a header containing meta data relevant for the recording and the packet based recording of the data traffic. Each packet is preceded by a timestamp. The first line of data in a packet forms the 12-bit packet header.

```
# Trigger timestamp: 2022-03-08T20:14:02.59896

### Settings
# Record Duration:	5s
# Trigger:			Timecode
# Enable NULLs:		0
# Enable FCTs:		1
# Enable Timecodes:	1
# Enable NChars:	1

### Link Analyser
# API version:		v5.01
# Device name:		FEE_1
# Serial number:	32190444
# Device version:	v1.01 edit 8
# Firmware version:	v1.05
# Build date:		19-01-10 14:43


2022-03-08T20:14:00.261377
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

2022-03-08T20:14:01.561410
000000 50 F0 00 18 02 82 7F 9A 00 01 00 49      <Header>
00000C 00 00 F0 00 00 00 00 47
000014 A7 56 A3 00 00 00 00 00
00001C 00 00 00 00 00 00 00 18

...
<empty line at end of file>
```

## Timestamp Format
The packets in the hexdump are marked with an absolute timestamp of the format `"%FT%T.%f"` (e.g. 2022-03-14T13:07:25.513545), although in some versions of Wireshark, only `"%FT%T."` is accepted to specify this format. The precision of the decimal seconds is variable.

## Commands

Collection of some usefull helpfull commands for using this software.

### Recording data to hexdump:

`spw_package_decode <serial number> <seconds> > log.txt`

### Parsing a hexdump:

`text2pcap -t "%FT%T." log.txt log.pcap`

### Opening a log in Wireshark:

`wireshark -t r -X lua_script:plato.lua log.pcap`