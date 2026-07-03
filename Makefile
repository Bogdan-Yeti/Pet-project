CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft

CFLAGS  = $(MCU) -Wall -g -O2 -ffreestanding
LDFLAGS = $(MCU) -T linker.ld -nostdlib

SRCS   = main.c startup.c
OBJS   = $(SRCS:.c=.o)
TARGET = firmware

all: $(TARGET).bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@
