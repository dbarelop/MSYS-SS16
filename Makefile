SRC_DIR := ./xmega
BUILD_DIR := ./target
BLUEOS_PATH := $(SRC_DIR)/BlueOS
BLUEOS_CONFIG_PATH := $(BLUEOS_PATH)/config
BLUEOS_SOURCE_PATH := $(BLUEOS_PATH)/source
AVR_DEVICE := atxmega128a1
F_CPU := 32000000UL
INC_PATHS := $(BLUEOS_CONFIG_PATH) $(BLUEOS_SOURCE_PATH)

CC := avr-gcc
OBJCOPY := avr-objcopy
CC_FLAGS := -Wall -gdwarf-2 -DF_CPU=$(F_CPU) -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -mmcu=$(AVR_DEVICE)
LD_FLAGS := -Wl,--start-group -Wl,-lm -Wl,--end-group -Wl,-L$(BLUEOS_SOURCE_PATH) -Wl,--gc-sections -mmcu=$(AVR_DEVICE)
INC_PARAMS := $(foreach d, $(INC_PATHS), -I$d)

BLUEOS_SRC_FILES := $(wildcard $(BLUEOS_SOURCE_PATH)/*.c)
BLUEOS_OBJ_FILES := $(BLUEOS_SRC_FILES:.c=.o)

main.o: $(SRC_DIR)/main.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CC_FLAGS) $(INC_PARAMS) -c -o $(BUILD_DIR)/$@ $^

motor.o: $(SRC_DIR)/motor.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CC_FLAGS) $(INC_PARAMS) -c -o $(BUILD_DIR)/$@ $^

main.elf: main.o motor.o $(BLUEOS_OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $(BUILD_DIR)/$@ $(addprefix $(BUILD_DIR)/,$^)

main.hex: main.elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $(BUILD_DIR)/$< $(BUILD_DIR)/$@

$(BLUEOS_SOURCE_PATH)/%.o: $(BLUEOS_SOURCE_PATH)/%.c
	mkdir -p $(BUILD_DIR)/$(BLUEOS_SOURCE_PATH)
	$(CC) $(CC_FLAGS) $(INC_PARAMS) -c -o $(BUILD_DIR)/$@ $<

clean:
	rm -rf $(BUILD_DIR)

all: main.hex
