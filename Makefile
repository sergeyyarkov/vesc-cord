.DEFAULT_GOAL := all
.PHONY: all asms binary clean size test prog_flash prog_test format compiledb


TARGET=firmware
PLATFORM?=ch32
BUILD_CONFIGS_MK=$(basename $(notdir $(shell find ./build_config -name "*.mk")))



# === Directories path =============================================================================
SRC_DIR=./src
OBJ_DIR=./obj/$(PLATFORM)
ASM_DIR=./asm/$(PLATFORM)
OUT_DIR=./build/$(PLATFORM)
# ==================================================================================================



# === Compiler options =============================================================================
CC_FLAGS=-Wall -Wextra
ifeq ($(DEBUG), true)
CC_FLAGS+=-ggdb -Og
CC_FLAGS+=-Wl,-Map=$(OUT_DIR)/$(TARGET).map
CC_FLAGS+=-DDEBUG_BUILD=TRUE
else
CC_FLAGS+=-Os
CC_FLAGS+=-flto
endif
CC_FLAGS+=-std=gnu11
CC_FLAGS+=-MMD -MP
CC_FLAGS+=-I$(SRC_DIR)
CC_FLAGS+=-Wno-unused-parameter
CC_FLAGS+=-ffunction-sections
CC_FLAGS+=-fdata-sections
# CC_FLAGS+=$(shell find libs -type d -printf "-I %p ")
# ==================================================================================================



# === Linker options ===============================================================================
LD_FLAGS+=--specs=nano.specs # Use optimized standard C library
LD_FLAGS+=-nostartfiles
LD_FLAGS+=-Wl,--relax
LD_FLAGS+=-Wl,--gc-sections
# ==================================================================================================



PROJ_SRCS=$(shell find $(SRC_DIR) -name "*.c")
OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(PROJ_SRCS))
ASMS=$(patsubst $(SRC_DIR)/%.c,$(ASM_DIR)/%.S,$(PROJ_SRCS))

ifneq (, $(filter $(PLATFORM), $(BUILD_CONFIGS_MK)))
include build_config/$(PLATFORM).mk
else
$(error Cannot find build config for platform: $(PLATFORM))
endif

DEPS=$(OBJS:.o=.d)



all: binary size
binary: $(OUT_DIR)/$(TARGET).bin
asms: $(ASMS)

$(OUT_DIR)/$(TARGET).bin: $(OUT_DIR)/$(TARGET).elf $(OUT_DIR)/$(TARGET).S
	$(OBJCOPY) -O binary $< $@

$(OUT_DIR)/$(TARGET).elf: $(OBJS) | $(OUT_DIR)
	$(CC) $(CC_FLAGS) $(LD_FLAGS) $^ -o $@

$(OUT_DIR)/$(TARGET).S: $(OUT_DIR)/$(TARGET).elf | $(OUT_DIR)
	$(OBJDUMP) -D $< > $@

## Compile to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $@

## Compile to assembler files (optional)
$(ASM_DIR)/%.S: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -S -fverbose-asm $(CC_FLAGS) -c $< -o $@

$(OUT_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(OUT_DIR)
	rm -rf $(ASM_DIR)

size: $(OUT_DIR)/$(TARGET).elf
	$(SIZE) -B $<

format:
	find . -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format --style=file:.clang-format -i {} +

compiledb:
	@make clean
	@bear -- make all

-include $(DEPS)