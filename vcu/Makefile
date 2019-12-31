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
CPPFLAGS += -MD -MP
CFLAGS += -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes
CFLAGS += -Werror=implicit-function-declaration
CFLAGS += -fno-common -ffunction-sections -fdata-sections -std=gnu99
LDFLAGS += --static -Wl,--gc-sections --specs=nosys.specs

# Target specific flags
DEFS += -D__SAME70Q21B__
ARCH_FLAGS := -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16
LDFLAGS += -Tthirdparty/startup/same70q21b_flash.ld

subdirs = $(patsubst %/,%,$(filter %/,$(wildcard $(1)/*/)))

# Include directories
INC_DIRS := \
thirdparty/freertos \
thirdparty/freertos/include \
thirdparty/hal_rtos \
thirdparty/fatfs \
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
driver/include \
.


# Source directories
SRC_DIRS := \
thirdparty/startup \
thirdparty/freertos \
thirdparty/hal_rtos \
thirdparty/fatfs \
$(call subdirs,atstart/hpl) \
atstart/hal/utils/src \
atstart/hal/src \
atstart/usb/class/msc/device \
atstart/usb/class/cdc/device \
atstart/usb/device \
atstart/usb \
atstart/sd_mmc \
atstart \
driver \
$(call subdirs,driver) \
framework \
$(call subdirs,framework) \
app \
$(call subdirs,app)

# Sources files excluded from compiling
SRC_EXCLUDES := \
atstart/rtos_start.c \
atstart/usb_start.c \
atstart/main.c \
atstart/hal/utils/src/utils_assert.c \
app/main.c

INCS := $(INC_DIRS:%=-I%)
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
SRCS := $(filter-out $(SRC_EXCLUDES),$(SRCS))
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
TESTS := $(wildcard test/*.c)
OUT_DIRS := $(SRC_DIRS:%=$(BUILD_DIR)/%) $(BUILD_DIR)/test

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

all: patch $(OUT_DIRS) $(BUILD_DIR)/$(OUTPUT_FILE_NAME).elf

test: patch $(OUT_DIRS) $(TESTS:%.c=$(BUILD_DIR)/%.elf)

patch:
	$(Q)rm -f atstart/hal/utils/include/utils_assert.h

$(OUT_DIRS):
	$(Q)"mkdir" -p $(@)

$(BUILD_DIR)/%.o: %.c
	@printf "  CC      $(<)\n"
	$(Q)$(CC) $(CPPFLAGS) $(DEFS) $(INCS) $(CFLAGS) $(ARCH_FLAGS) -o $(@) -c $(<)

$(BUILD_DIR)/atstart/hpl/usbhs/hpl_usbhs.o: atstart/hpl/usbhs/hpl_usbhs.c
	@printf "  CC      $(<)\n"
	$(Q)$(CC) -include memcpy_usb.h $(CPPFLAGS) $(DEFS) $(INCS) $(CFLAGS) $(ARCH_FLAGS) -o $(@) -c $(<)

$(BUILD_DIR)/$(OUTPUT_FILE_NAME).elf: $(OBJS) $(BUILD_DIR)/app/main.o
	@printf "  LD      $(@)\n"
	$(Q)$(LD) -o $(@) $(^) -Wl,-Map="$(@:.elf=.map)" $(LDFLAGS) $(ARCH_FLAGS)
	@printf "  OBJCOPY $(@:.elf=.bin)\n"
	$(Q)$(OBJCOPY) -O binary $(@) $(@:.elf=.bin)
	@printf "  OBJCOPY $(@:.elf=.hex)\n"
	$(Q)$(OBJCOPY) -O ihex $(@) $(@:.elf=.hex)
	$(Q)$(CC) --version
	$(Q)$(SIZE) $(@)

$(BUILD_DIR)/test/%.elf: $(OBJS) $(BUILD_DIR)/test/%.o
	@printf "  LD      $(@)\n"
	$(Q)$(LD) -o $(@) $(^) $(LDFLAGS) $(ARCH_FLAGS)

clean:
	@printf "  CLEAN\n"
	$(Q)rm -rf $(BUILD_DIR)

.PHONY: all test clean patch
.SECONDARY:

-include $(DEPS)
