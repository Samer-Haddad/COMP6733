#!/usr/bin/kermit +

SET LINE /dev/ttyACM3
IF FAIL EXIT 1 "No USB device found (/dev/ttyACM0) [Connect to VM?]"
set carrier off
set flow none
set speed 115200
connect

SET LINE /dev/ttyACM3
set carrier off
set flow none
set speed 115200
connect

exit
