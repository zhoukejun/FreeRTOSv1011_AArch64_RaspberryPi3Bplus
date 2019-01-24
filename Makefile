ARMGNU ?= aarch64-elf
INCLUDEPATH1 ?= Source/include 
INCLUDEPATH2 ?= Source/portable/GCC/ARM_CA53_64_BIT_NO_GIC
INCLUDEPATH3 ?= Demo/CORTEX_A53_64-bit_BCM2837B0
INCLUDEPATH4 ?= Demo/CORTEX_A53_64-bit_BCM2837B0/Drivers


COPS = -Wall -O2 -lm -lgcc  -ffreestanding -nostartfiles  -std=gnu99  -mcpu=cortex-a53 -I $(INCLUDEPATH1) -I $(INCLUDEPATH2) -I $(INCLUDEPATH3) -I $(INCLUDEPATH4)
#COPS = -O2 -lm -lgcc -lstdc++ -ffreestanding  -std=gnu99 -I $(INCLUDEPATH1) -I $(INCLUDEPATH2) -I $(INCLUDEPATH3) -I $(INCLUDEPATH4)
#COPS = -Wall -O2 -mcpu=cortex-amrv8 -I $(INCLUDEPATH1) -I $(INCLUDEPATH2) -I $(INCLUDEPATH3) -I $(INCLUDEPATH4)


OBJS += build/boot.o
#OBJS += build/crt0.o
OBJS += build/gpio.o
OBJS += build/irq.o
OBJS += build/arm_timer.o
OBJS += build/led.o
OBJS += build/uart.o
OBJS += build/FreeRTOS_asm_vectors.o
OBJS += build/FreeRTOS_tick_config.o
OBJS += build/main.o
OBJS += build/mem_man.o
OBJS += build/vectors.o
OBJS += build/exception.o
OBJS += build/croutine.o
OBJS += build/event_groups.o
OBJS += build/list.o
OBJS += build/portASM.o
OBJS += build/port.o
#OBJS += build/heap_1.o
#OBJS += build/heap_2.o
#OBJS += build/heap_3.o
OBJS += build/heap_4.o
#OBJS += build/heap_5.o
OBJS += build/queue.o
OBJS += build/stream_buffer.o
OBJS += build/tasks.o
OBJS += build/timers.o

build/%.o : Demo/CORTEX_A53_64-bit_BCM2837B0/%.S
	$(ARMGNU)-gcc $(COPS) -D__ASSEMBLY__ -c -o $@ $<
	
build/%.o : Demo/CORTEX_A53_64-bit_BCM2837B0/Drivers/%.c
	$(ARMGNU)-gcc $(COPS)  -c -o $@ $<
		
build/%.o : Demo/CORTEX_A53_64-bit_BCM2837B0/%.c
	$(ARMGNU)-gcc $(COPS)  -c -o $@ $<
	
build/%.o : Source/%.c
	$(ARMGNU)-gcc $(COPS)  -c -o $@ $<
	
build/%.o : Source/portable/GCC/ARM_CA53_64_BIT_NO_GIC/%.S
	$(ARMGNU)-gcc $(COPS) -D__ASSEMBLY__ -c -o $@ $<

build/%.o : Source/portable/GCC/ARM_CA53_64_BIT_NO_GIC/%.c
	$(ARMGNU)-gcc $(COPS) -c -o $@ $<
           
build/%.o : Source/portable/MemMang/%.c
	$(ARMGNU)-gcc $(COPS) -c -o $@ $<

build/%.o : Source/%.c
	$(ARMGNU)-gcc $(COPS) -c -o $@ $<



all : kernel.img

kernel.img : Demo/CORTEX_A53_64-bit_BCM2837B0/raspberrypi.ld $(OBJS)
	$(ARMGNU)-ld $(OBJS) -o freertos_bcm2837.elf -T Demo/CORTEX_A53_64-bit_BCM2837B0/raspberrypi.ld
	$(ARMGNU)-objdump -D freertos_bcm2837.elf > freertos_bcm2837.list
	$(ARMGNU)-objcopy freertos_bcm2837.elf -O ihex freertos_bcm2837.hex
	$(ARMGNU)-objcopy freertos_bcm2837.elf -O binary freertos_bcm2837.bin
	$(ARMGNU)-objcopy freertos_bcm2837.elf -O binary kernel.img

clean :
	rm -f build/*.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f build/*.bc

