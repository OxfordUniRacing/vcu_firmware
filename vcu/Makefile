# Must be the same as visual studio project name
OUTPUT_FILE_NAME = vcu

# Flags for debug and release builds
ifndef RELEASE
	CFLAGS += -ggdb3 -Og
	DEFS += -DDEBUG
	BUILD_DIR ?= Debug
else
	CFLAGS += -ggdb3 -O2
	DEFS += -DNDEBUG
	BUILD_DIR ?= Release
endif

# Compile flags
DEFS += -DUSE_SIMPLE_ASSERT=1
CPPFLAGS += -MD -MP
CFLAGS += -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes
CFLAGS += -fno-common -ffunction-sections -fdata-sections -std=gnu99
LDFLAGS += --static -Wl,--gc-sections --specs=nosys.specs

# Target specific flags
DEFS += -D__SAME70Q21B__
ARCH_FLAGS := -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16
LDFLAGS += -Tstartup/same70q21b_flash.ld

subdirs = $(patsubst %/,%,$(filter %/,$(wildcard $(1)/*/)))

# Include directories
INC_DIRS := \
freertos/include \
hal_rtos \
atstart/thirdparty/RTOS \
atstart/thirdparty/RTOS/freertos/FreeRTOSV10.0.0 \
atstart/config \
atstart/CMSIS/Core/Include \
atstart/same70b/include \
atstart/hri \
$(call subdirs,atstart/hpl) \
atstart/hal/utils/include \
atstart/hal/include \
atstart/usb/class/msc/device \
atstart/usb/class/msc \
atstart/usb/class/cdc/device \
atstart/usb/class/cdc \
atstart/usb/device \
atstart/usb \
atstart/sd_mmc \
atstart \
fatfs \
log \
.

# Source directories
SRC_DIRS := \
startup \
freertos \
hal_rtos \
atstart/thirdparty/RTOS/freertos/FreeRTOSV10.0.0 \
$(call subdirs,atstart/hpl) \
atstart/hal/utils/src \
atstart/hal/src \
atstart/usb/class/msc/device \
atstart/usb/class/cdc/device \
atstart/usb/device \
atstart/usb \
atstart/sd_mmc \
atstart/diskio \
atstart \
fatfs \
log \
.

# Sources files excluded from compiling
SRC_EXCLUDES := \
atstart/rtos_start.c \
atstart/usb_start.c \
atstart/main.c

INCS := $(INC_DIRS:%=-I%)
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
SRCS := $(filter-out $(SRC_EXCLUDES),$(SRCS))
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
OUT_DIRS := $(SRC_DIRS:%=$(BUILD_DIR)/%)

PREFIX ?= arm-none-eabi-
CC := $(PREFIX)gcc
CXX := $(PREFIX)g++
LD := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
SIZE := $(PREFIX)size

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
endif

all: $(OUT_DIRS) $(BUILD_DIR)/$(OUTPUT_FILE_NAME).elf

$(OUT_DIRS):
	$(Q)"mkdir" -p $(@)

$(BUILD_DIR)/%.o: %.c
	@printf "  CC      $(<)\n"
	$(Q)$(CC) $(CPPFLAGS) $(DEFS) $(INCS) $(CFLAGS) $(ARCH_FLAGS) -o $(@) -c $(<)

$(BUILD_DIR)/atstart/hpl/usbhs/hpl_usbhs.o: atstart/hpl/usbhs/hpl_usbhs.c
	@printf "  CC      $(<)\n"
	$(Q)$(CC) -include memcpy_usb.h $(CPPFLAGS) $(DEFS) $(INCS) $(CFLAGS) $(ARCH_FLAGS) -o $(@) -c $(<)

$(BUILD_DIR)/%.elf: $(OBJS)
	@printf "  LD      $(@)\n"
	$(Q)$(LD) -o $(@) $(^) -Wl,-Map="$(BUILD_DIR)/$(*).map" $(LDFLAGS) $(ARCH_FLAGS)
	@printf "  OBJCOPY $(*).bin\n"
	$(Q)$(OBJCOPY) -O binary $(@) $(BUILD_DIR)/$(*).bin
	@printf "  OBJCOPY $(*).hex\n"
	$(Q)$(OBJCOPY) -O ihex $(@) $(BUILD_DIR)/$(*).hex
	$(Q)$(CC) --version
	$(Q)$(SIZE) $(@)

clean:
	@printf "  CLEAN\n"
	$(Q)rm -rf $(BUILD_DIR)

.PHONY: all clean
.SECONDARY:

-include $(DEPS)
