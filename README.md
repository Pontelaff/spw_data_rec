# spw_package_decode

Software for recording SpaceWire traffic using a STAR-Dundee SpaceWire Link Analyser Mk3.

## Options

`spw_package_decode [-f] [-v] [-a 'TOPIC TEST_ID TEST_VERS IF_ID_IN IF_ID_OUT DB_VERS ASW_VERS'] [-c EN_CHARS] [-p MILLIS] [-r RECV]  [--help] [--usage] SERIAL_NO SECONDS`

| Option | Argument  | Type    | Description                                                                                                                       |
| ------ | --------- | ------- | --------------------------------------------------------------------------------------------------------------------------------- |
| -f     | none      | none    | Flag for using FCTs as the trigger Event instead of Timecodes.                                                                    |
| -v     | none      | none    | Flag for printing readable event based capture logs instead of packet based hexdumps.                                             |
| -a     | "TOPIC TEST_ID TEST_VERS IF_ID_IN IF_ID_OUT DB_VERS ASW_VERS" | string | Enables archiving the captured data to a database using Kafka. The arguments have to be passed as a space-separated string containing at least one character per argument. |
| -c     | EN_CHARS  | integer | Enables SpaceWire characters to be recorded by the LinkAnalyser. The integer input (0-15) is interpreted as a binary value with each bit serving as an enable flag for one type of character. The first bit (LSB) is enabling NChars, the second bit is enabling Timecodes, the third bit is enabling FCTs and the MSB is enabling NULL codes. |
| -p     | MILLIS    | integer | Determines the maximum time period in milliseconds before the trigger, for which recorded packets will be printed to the hexdump. |
| -r     | RECV      | char    | Determines on which of it's receivers the Link Analyser will wait for the trigger event ('A' or 'B').                             |
| none   | SERIAL_NO | integer | The serial number of the Link Analyser recording the data traffic.                                                                |
| none   | SECONDS   | double  | The duration in seconds to be recorded after the Link Analyser has been triggered.                                                |

## Hexdump Format

The hexdump consists of a header containing meta data relevant for the recording and the packet based recording of the data traffic. Each packet is preceded by a timestamp. The first line of data in a packet forms the 12-bit packet header.
Since the packages are only written when complete, they might not appear in strict chronological order. If needed, the packets can be sorted by the Timestamps in Wireshark.

```
# Trigger timestamp:   2022-03-08T20:14:02.598964125
# Software version:    spw_package_decode v0.3.4

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
The packets in the hexdump are marked with an absolute timestamp of the format `"%FT%T.%f"` (e.g. 2022-03-14T13:07:25.598964125), although in some versions of Wireshark, only `"%FT%T."` is accepted to specify this format. The precision of the decimal seconds to be read by Wireshark is variable. The Link Analyser Mk3 is capable of measuring time with 100MHz clock resulting in a 10 nanosecond accuracy.

## Commands

Collection of some helpful commands for using this software.

### Building executable:

`gcc -Iinc -I<star-api include path> -I<spw_la_api include path> -I<librdkafka include path> -I<json-c include path> -L<star-api library path> -L<spw_la_api library path> -g src/*.c -lstar-api -lstar_conf_api_brick_mk2 -lspw_la_api -lrdkafka -luuid -ljson-c -o ./bin/spw_package_decode`

### Recording data to hexdump:

`spw_package_decode [options] <serial number> <seconds> > hexdump.txt`

### Parsing a hexdump:

`text2pcap -D -t "%FT%T." -T 0,59274 hexdump.txt hexdump.pcap`

### Importing a hexdump to Wireshark:

`wireshark -t r hexdump.pcap`
