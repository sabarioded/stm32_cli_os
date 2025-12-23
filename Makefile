TARGET      = stm32_cli_os
LDSCRIPT    = config/stm32l476rg_flash.ld

CC          = arm-none-eabi-gcc
CFLAGS      = -mcpu=cortex-m4 -mthumb -std=gnu11 -g -O0 -Wall -Wextra -ffreestanding \
              -Iinclude -Icore -Idrivers -Iapp -Iconfig
LDFLAGS     = -nostdlib -T $(LDSCRIPT) -Wl,-Map=$(TARGET).map -Wl,--gc-sections

SRCS = \
    app/main.c \
    app/app_tasks.c \
    app/app_commands.c \
    core/utils.c \
    core/scheduler.c \
    core/system_clock.c \
    core/cli.c \
    drivers/led.c \
    drivers/button.c \
    drivers/uart.c \
    drivers/systick.c \
    startup/stm32l476_startup.c

OBJS = $(SRCS:.c=.o)

.PHONY: all clean load

all: $(TARGET).elf

$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).map

load: $(TARGET).elf
	openocd -f board/st_nucleo_l4.cfg \
	 -c "program $(TARGET).elf verify reset exit"
