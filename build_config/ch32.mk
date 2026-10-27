# === Directories path =============================================================================
EVT_DIR=/opt/wch/CH32V003EVT/EVT/EXAM/SRC
PLATFORM_DIR=platform/ch32v00x
# ==================================================================================================



# === Toolchain ====================================================================================
OBJCOPY=riscv32-wch-elf-objcopy
OBJDUMP=riscv32-wch-elf-objdump
SIZE=riscv32-wch-elf-size
CC=riscv32-wch-elf-gcc
FLASHER_TOOL=wlink
# ==================================================================================================



# === Compiler options =============================================================================
CC_FLAGS+=-march=rv32ec_zicsr
CC_FLAGS+=-mabi=ilp32e
CC_FLAGS+=-I$(EVT_DIR)/Core
CC_FLAGS+=-I$(EVT_DIR)/Peripheral/inc
CC_FLAGS+=-I./platform/ch32v00x
# ==================================================================================================



# === Linker options ===============================================================================
LDSCRIPT+=./ld/Link.ld
LD_FLAGS+=-T$(LDSCRIPT)
# ==================================================================================================



# Adds WCH SDK to compile flow
STARTUP_SRC=$(PLATFORM_DIR)/startup_ch32v00x.S
PLATFORM_SRCS=$(shell find $(PLATFORM_DIR) -name "*.c")

EVT_SRCS=$(shell find $(EVT_DIR)/Core -name "*.c")
EVT_SRCS+=$(shell find $(EVT_DIR)/Peripheral -name "*.c")

OBJS+=$(patsubst $(EVT_DIR)/%.c,$(OBJ_DIR)/evt/%.o,$(EVT_SRCS))
OBJS+=$(patsubst $(PLATFORM_DIR)/%.c,$(OBJ_DIR)/$(PLATFORM_DIR)/%.o,$(PLATFORM_SRCS))
OBJS+=$(OBJ_DIR)/$(basename $(notdir $(STARTUP_SRC))).o

prog_flash: $(OUT_DIR)/$(TARGET).bin
	$(FLASHER_TOOL) flash $<

prog_test:
	$(FLASHER_TOOL) status


# Compile SDK and init sources
$(OBJ_DIR)/evt/%.o: $(EVT_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(OBJ_DIR)/$(PLATFORM_DIR)/%.o: $(PLATFORM_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(OBJ_DIR)/startup_ch32v00x.o: $(STARTUP_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $@