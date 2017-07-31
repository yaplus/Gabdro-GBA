.PHONY: clean

#-----------------------------------------------------------------------
# コンパイラの設定
#-----------------------------------------------------------------------
CC      = arm-none-eabi-gcc
LD      = $(CC) -specs=gba_mb.specs
OBJCOPY = arm-none-eabi-objcopy
OPTUSB  = optusb
CFLAGS  = -W -Wall -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math

#-----------------------------------------------------------------------
# ファイルとディレクトリの設定
#-----------------------------------------------------------------------
TARGET  = gabriel
INCLUDE = include
SOURCE  = source
BUILD   = build
CFILES  = $(filter-out $(SOURCE), $(shell find $(SOURCE) -name "*.c"))
OFILES  = $(addprefix $(BUILD)/, $(subst $(SOURCE)/, , $(CFILES:.c=.o)))

#-----------------------------------------------------------------------
# ビルドの設定
#-----------------------------------------------------------------------
default:
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make all

all: $(TARGET).gba

$(TARGET).gba: $(TARGET).elf
	$(OBJCOPY) -O binary $(BUILD)/$^ $@

$(TARGET).elf: $(OFILES)
	$(LD) -o $(BUILD)/$@ $^

$(BUILD)/%.o: $(SOURCE)/%.c
	$(CC) $(CFLAGS) -I $(INCLUDE) -o $@ -c $<

run-gba: $(TARGET).gba
	$(OPTUSB) $^

clean:
	rm -f $(BUILD)/*
	rm -f $(TARGET).gba
