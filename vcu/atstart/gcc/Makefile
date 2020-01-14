################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifdef SystemRoot
	SHELL = cmd.exe
	MK_DIR = mkdir
else
	ifeq ($(shell uname), Linux)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), CYGWIN)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW32)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW64)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), DARWIN)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
usb/class/cdc/device \
hpl/uart \
hpl/pmc \
hpl/afec \
hpl/usart \
sd_mmc \
hal/src \
hpl/hsmci \
hpl/mcan \
hpl/usbhs \
hpl/xdmac \
hpl/efc \
hal/utils/src \
same70b/gcc/gcc \
examples \
usb/device \
usb \
same70b/gcc \
hpl/core \
usb/class/msc/device

# List the object files
OBJS +=  \
hal/src/hal_io.o \
hal/src/hal_can_async.o \
hpl/mcan/hpl_mcan.o \
atmel_start.o \
sd_mmc_start.o \
hpl/usart/hpl_usart.o \
hpl/uart/hpl_uart.o \
usb/class/cdc/device/cdcdf_acm.o \
hal/utils/src/utils_syscalls.o \
usb/class/msc/device/mscdf.o \
same70b/gcc/gcc/startup_same70q21b.o \
hpl/efc/hpl_efc.o \
hal/src/hal_delay.o \
hpl/usbhs/hpl_usbhs.o \
hpl/core/hpl_init.o \
hal/utils/src/utils_list.o \
hal/src/hal_usart_os.o \
hal/utils/src/utils_assert.o \
usb_start.o \
hal/src/hal_adc_async.o \
hpl/afec/hpl_afec.o \
usb/usb_protocol.o \
hpl/pmc/hpl_pmc.o \
hal/src/hal_mci_os.o \
hal/src/hal_flash.o \
hal/src/hal_init.o \
hal/src/hal_usb_device.o \
main.o \
usb/device/usbdc.o \
sd_mmc/sd_mmc.o \
examples/driver_examples.o \
driver_init.o \
hal/src/hal_usart_async.o \
same70b/gcc/system_same70q21b.o \
hal/utils/src/utils_ringbuffer.o \
hal/src/hal_gpio.o \
hpl/hsmci/hpl_hsmci.o \
hal/utils/src/utils_event.o \
hal/src/hal_sleep.o \
hpl/pmc/hpl_sleep.o \
hpl/core/hpl_core_m7_base.o \
hal/src/hal_atomic.o \
hpl/xdmac/hpl_xdmac.o

OBJS_AS_ARGS +=  \
"hal/src/hal_io.o" \
"hal/src/hal_can_async.o" \
"hpl/mcan/hpl_mcan.o" \
"atmel_start.o" \
"sd_mmc_start.o" \
"hpl/usart/hpl_usart.o" \
"hpl/uart/hpl_uart.o" \
"usb/class/cdc/device/cdcdf_acm.o" \
"hal/utils/src/utils_syscalls.o" \
"usb/class/msc/device/mscdf.o" \
"same70b/gcc/gcc/startup_same70q21b.o" \
"hpl/efc/hpl_efc.o" \
"hal/src/hal_delay.o" \
"hpl/usbhs/hpl_usbhs.o" \
"hpl/core/hpl_init.o" \
"hal/utils/src/utils_list.o" \
"hal/src/hal_usart_os.o" \
"hal/utils/src/utils_assert.o" \
"usb_start.o" \
"hal/src/hal_adc_async.o" \
"hpl/afec/hpl_afec.o" \
"usb/usb_protocol.o" \
"hpl/pmc/hpl_pmc.o" \
"hal/src/hal_mci_os.o" \
"hal/src/hal_flash.o" \
"hal/src/hal_init.o" \
"hal/src/hal_usb_device.o" \
"main.o" \
"usb/device/usbdc.o" \
"sd_mmc/sd_mmc.o" \
"examples/driver_examples.o" \
"driver_init.o" \
"hal/src/hal_usart_async.o" \
"same70b/gcc/system_same70q21b.o" \
"hal/utils/src/utils_ringbuffer.o" \
"hal/src/hal_gpio.o" \
"hpl/hsmci/hpl_hsmci.o" \
"hal/utils/src/utils_event.o" \
"hal/src/hal_sleep.o" \
"hpl/pmc/hpl_sleep.o" \
"hpl/core/hpl_core_m7_base.o" \
"hal/src/hal_atomic.o" \
"hpl/xdmac/hpl_xdmac.o"

# List the directories containing header files
DIR_INCLUDES +=  \
-I"../" \
-I"../config" \
-I"../examples" \
-I"../hal/include" \
-I"../hal/utils/include" \
-I"../hpl/afec" \
-I"../hpl/core" \
-I"../hpl/hsmci" \
-I"../hpl/mcan" \
-I"../hpl/nvmctrl" \
-I"../hpl/pio" \
-I"../hpl/pmc" \
-I"../hpl/uart" \
-I"../hpl/usart" \
-I"../hpl/usbhs" \
-I"../hpl/xdmac" \
-I"../hri" \
-I"../" \
-I"../config" \
-I"../usb" \
-I"../usb/class/cdc" \
-I"../usb/class/cdc/device" \
-I"../usb/class/msc" \
-I"../usb/class/msc/device" \
-I"../usb/device" \
-I"../" \
-I"../config" \
-I"../sd_mmc" \
-I"../" \
-I"../CMSIS/Core/Include" \
-I"../same70b/include"

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

DEPS_AS_ARGS +=  \
"usb/usb_protocol.d" \
"hal/utils/src/utils_event.d" \
"hal/src/hal_io.d" \
"hal/src/hal_can_async.d" \
"hpl/uart/hpl_uart.d" \
"hpl/usart/hpl_usart.d" \
"hal/utils/src/utils_syscalls.d" \
"usb/class/cdc/device/cdcdf_acm.d" \
"sd_mmc_start.d" \
"hpl/xdmac/hpl_xdmac.d" \
"hpl/efc/hpl_efc.d" \
"same70b/gcc/gcc/startup_same70q21b.d" \
"usb/class/msc/device/mscdf.d" \
"hal/src/hal_usart_os.d" \
"hal/utils/src/utils_list.d" \
"hal/src/hal_adc_async.d" \
"usb_start.d" \
"hal/utils/src/utils_assert.d" \
"hpl/usbhs/hpl_usbhs.d" \
"hal/src/hal_delay.d" \
"hpl/mcan/hpl_mcan.d" \
"hal/src/hal_flash.d" \
"hal/src/hal_mci_os.d" \
"hal/src/hal_usb_device.d" \
"hpl/core/hpl_init.d" \
"hal/src/hal_init.d" \
"hpl/afec/hpl_afec.d" \
"hpl/pmc/hpl_pmc.d" \
"driver_init.d" \
"hal/src/hal_usart_async.d" \
"main.d" \
"examples/driver_examples.d" \
"hpl/hsmci/hpl_hsmci.d" \
"hal/src/hal_sleep.d" \
"hal/utils/src/utils_ringbuffer.d" \
"same70b/gcc/system_same70q21b.d" \
"usb/device/usbdc.d" \
"hal/src/hal_gpio.d" \
"hpl/pmc/hpl_sleep.d" \
"hal/src/hal_atomic.d" \
"sd_mmc/sd_mmc.d" \
"hpl/core/hpl_core_m7_base.d" \
"atmel_start.d"

OUTPUT_FILE_NAME :=AtmelStart
QUOTE := "
OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf

vpath %.c ../
vpath %.s ../
vpath %.S ../

# All Target
all: $(SUB_DIRS) $(OUTPUT_FILE_PATH)

# Linker target

$(OUTPUT_FILE_PATH): $(OBJS)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -o $(OUTPUT_FILE_NAME).elf $(OBJS_AS_ARGS) -Wl,--start-group -lm -Wl,--end-group -mthumb \
-Wl,-Map="$(OUTPUT_FILE_NAME).map" --specs=nano.specs -Wl,--gc-sections -mcpu=cortex-m7 \
 \
-T"../same70b/gcc/gcc/same70q21b_flash.ld" \
-L"../same70b/gcc/gcc"
	@echo Finished building target: $@

	"arm-none-eabi-objcopy" -O binary "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).bin"
	"arm-none-eabi-objcopy" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  \
        "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).hex"
	"arm-none-eabi-objcopy" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma \
        .eeprom=0 --no-change-warnings -O binary "$(OUTPUT_FILE_NAME).elf" \
        "$(OUTPUT_FILE_NAME).eep" || exit 0
	"arm-none-eabi-objdump" -h -S "$(OUTPUT_FILE_NAME).elf" > "$(OUTPUT_FILE_NAME).lss"
	"arm-none-eabi-size" "$(OUTPUT_FILE_NAME).elf"

	

# Compiler targets




%.o: %.c
	@echo Building file: $<
	@echo ARM/GNU C Compiler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAME70Q21B__ -mcpu=cortex-m7 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.s
	@echo Building file: $<
	@echo ARM/GNU Assembler
	$(QUOTE)arm-none-eabi-as$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAME70Q21B__ -mcpu=cortex-m7 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.S
	@echo Building file: $<
	@echo ARM/GNU Preprocessing Assembler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
-D__SAME70Q21B__ -mcpu=cortex-m7 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
$(DIR_INCLUDES) \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean:
	rm -f $(OBJS_AS_ARGS)
	rm -f $(OUTPUT_FILE_PATH)
	rm -f $(DEPS_AS_ARGS)
	rm -f $(OUTPUT_FILE_NAME).a $(OUTPUT_FILE_NAME).hex $(OUTPUT_FILE_NAME).bin \
        $(OUTPUT_FILE_NAME).lss $(OUTPUT_FILE_NAME).eep $(OUTPUT_FILE_NAME).map \
        $(OUTPUT_FILE_NAME).srec