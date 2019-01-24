FreeRTOSv1011 on AArch64 mode RaspberryPi3Bplus.


HOWTO:

1. A TF card formated in FAT32

2. From the RaspberryPi official release, copy these binaries ( bootcode.bin,  fixup.dat, start.elf) to the TF card.

3. Copy the binary kernel.img which is built from this project to the TF card.

4. Add config.txt file in TF card.


config.txt is showing below.

====================

# ARMv8
arm_control=0x200

# Uart
enable_uart=1

# 1: Load at address 0x0
# 0: Load at address 0x80000
kernel_old=1

# Kernel name
kernel=kernel.img

====================

5. You need to connect the RaspberryPi 3B+ mini UART port to your PC. UART setting is showing below

===============================
set line /dev/ttyUSB0 

set speed 115200 

set carrier-watch off 

set flow-control none 

set prefixing all

set streaming off

set parity none
===============================
