# Simple Makefile for STM32L476RG bare-metal: LED + button + UART

TARGET      = 4_makefile_project
LDSCRIPT    = stm32l476rg_flash.ld

CC          = arm-none-eabi-gcc
CFLAGS      = -mcpu=cortex-m4 -mthumb -std=gnu11 -g -O0 -Wall -Wextra -ffreestanding
LDFLAGS     = -nostdlib -T $(LDSCRIPT) -Wl,-Map=$(TARGET).map -Wl,--gc-sections

OBJS        = main.o stm32l476_startup.o led.o button.o uart.o

.PHONY: all clean load

all: $(TARGET).elf

$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

main.o: main.c led.h button.h uart.h
	$(CC) $(CFLAGS) -c main.c -o main.o

stm32l476_startup.o: stm32l476_startup.c
	$(CC) $(CFLAGS) -c stm32l476_startup.c -o stm32l476_startup.o

led.o: led.c led.h
	$(CC) $(CFLAGS) -c led.c -o led.o

button.o: button.c button.h
	$(CC) $(CFLAGS) -c button.c -o button.o

uart.o: uart.c uart.h
	$(CC) $(CFLAGS) -c uart.c -o uart.o

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).map

load: $(TARGET).elf
	openocd -f board/st_nucleo_l4.cfg \
	 -c "program $(TARGET).elf verify reset exit"
