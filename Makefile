# ============================================================
#  Makefile — рецепт сборки. Запускаешь "make" — получаешь
#  прошивку firmware.bin. "make flash" — заливает её на плату.
#  ВАЖНО: отступы в правилах ниже — табы, не пробелы.
# ============================================================

# --- Инструменты ---
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# --- Параметры чипа: ядро Cortex-M4, набор Thumb,
#     пока без аппаратной плавающей точки (нам она не нужна) ---
MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft

# --- Флаги компиляции и компоновки ---
CFLAGS  = $(MCU) -Wall -g -O2 -ffreestanding
LDFLAGS = $(MCU) -T linker.ld -nostdlib

# --- Файлы проекта ---
SRCS   = main.c startup.c
OBJS   = $(SRCS:.c=.o)
TARGET = firmware

all: $(TARGET).bin

# Скомпилировать каждый .c в объектный .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Скомпоновать всё в .elf по нашему linker.ld
$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

# Превратить .elf в "сырой" .bin для заливки
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Залить на плату через встроенный ST-Link
flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin

.PHONY: all flash clean
