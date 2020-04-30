# Compiler
ARMGNU ?= arm-none-eabi

# Directories
BUILD = build/
SOURCE = source/

# Input and output files
TARGET = kernel.img
MAP = kernel.map
LINKER = kernel.ld
OBJECTS := $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(wildcard $(SOURCE)*.s))

all: $(TARGET) $(LIST)
rebuild: all

$(TARGET) : $(BUILD)output.elf
	$(ARMGNU)-objcopy $(BUILD)output.elf -O binary $(TARGET)

$(BUILD)output.elf : $(OBJECTS) $(LINKER)
	$(ARMGNU)-ld --no-undefined $(OBJECTS) -Map $(MAP) -o $(BUILD)output.elf -T $(LINKER)

$(BUILD)%.o: $(SOURCE)%.s $(BUILD)
	$(ARMGNU)-as -I $(SOURCE) $< -o $@

$(BUILD):
	mkdir $@

clean: 
	-rm -rf $(BUILD)
	-rm -f $(TARGET)
	-rm -f $(LIST)
	-rm -f $(MAP)
