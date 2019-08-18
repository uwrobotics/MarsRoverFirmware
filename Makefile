# Makefile for Mars Rover 2020 control board firmware applications
###############################################################################
# Can be used to cross-compile compatible apps on different STM32-F446RE boards
# Just run `make` command in same directory to see options.
#
# Example: $ make APP=test_blinky BOARD=nucleo
#          $ make APP=test_blinky BOARD=science
#          $ make APP=arm_lower   BOARD=arm
#
###############################################################################

BUILD_PATH    := build
APPS_PATH	  := apps
APP_OUT_PATH  := ../$(BUILD_PATH)/$(APP)
APP_PATH      := ../$(APPS_PATH)/$(APP)
LIB_PATH      := ../lib
MBED_PATH     := ../mbed-os
CONFIG_PATH   := ../config

###############################################################################
# Boiler-plate

# cross-platform directory manipulation
ifeq ($(shell echo $$OS),$$OS)
    MAKE_DIR = if not exist "$(1)" mkdir "$(1)"
    RM = rmdir /S /Q "$(1)"
    RM_FILE_TYPE = del /S /Q "$(1)" "$(2)"
else
    MAKE_DIR = '$(SHELL)' -c "mkdir -p \"$(1)\""
    RM = '$(SHELL)' -c "rm -rf \"$(1)\""
    RM_FILE_TYPE = '$(SHELL)' -c "find $(1) -name \"$(2)\" -delete -print"
endif

# Move to the build directory
ifeq (,$(filter $(BUILD_PATH),$(notdir $(CURDIR))))
.SUFFIXES:
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
MAKETARGET = '$(MAKE)' --no-print-directory -C $(BUILD_PATH) -f '$(mkfile_path)' \
		'SRCDIR=$(CURDIR)' $(MAKECMDGOALS)

.PHONY: $(BUILD_PATH) clean
all:

ifeq ($(filter $(APP), $(patsubst $(APPS_PATH)/%/,%,$(sort $(dir $(wildcard $(APPS_PATH)/*/))))),)
	$(error APP is not set or is not supported. ${\n}Select an app to build with APP=app_name:${\n}${\n}$(subst ${ },${\n},$(patsubst $(APPS_PATH)/%/,%,$(sort $(dir $(wildcard $(APPS_PATH)/*/)))))${\n}${\n})
endif

ifeq ($(filter $(BOARD)," nucleo arm science safety "),)
	$(error BOARD is not set or is not supported. Set BOARD=board_name:${\n}${\n}safety${\n}arm${\n}science${\n}nucleo${\n}${\n}))
endif

	+@$(call MAKE_DIR,$(BUILD_PATH)/$(APP))
	+@$(MAKETARGET)

$(BUILD_PATH): all

Makefile : ;
% :: $(BUILD_PATH) ; :

makefile : ;
% :: $(BUILD_PATH) ; :

clean :
	$(call RM_DIR,$(BUILD_PATH))
	$(call RM_FILE_TYPE,$(APPS_PATH),*.o)
	$(call RM_FILE_TYPE,$(APPS_PATH),*.d)
	$(call RM_FILE_TYPE,lib,*.d)
	$(call RM_FILE_TYPE,lib,*.o)

clean-mbed: 
	$(call RM_FILE_TYPE,mbed-os,*.d)
	$(call RM_FILE_TYPE,mbed-os,*.o)

else

# Trick rules into thinking we are in the root, when we are in the bulid dir
VPATH = ..

# Boiler-plate
###############################################################################
# Project settings

PROJECT := $(APP)_$(BOARD)

# Project settings
###############################################################################
# Objects and Paths

APP_SRC_C += $(wildcard $(APP_PATH)/*.c)
LIB_SRC_C += $(wildcard $(LIB_PATH)/*/*.c)

APP_SRC_CPP += $(wildcard $(APP_PATH)/*.cpp)
LIB_SRC_CPP += $(wildcard $(LIB_PATH)/*/*.cpp)

APP_INC += -I$(APP_PATH)
LIB_INC += $(addprefix -I,$(wildcard $(LIB_PATH)/*))

SRC_FILES_C   = $(APP_SRC_C)   $(LIB_SRC_C)
SRC_FILES_CPP = $(APP_SRC_CPP) $(LIB_SRC_CPP)

include ${MBED_PATH}/mbed_make_obj_inc

OBJECTS += $(SRC_FILES_C:.c=.o) $(SRC_FILES_CPP:.cpp=.o) ${MBED_OBJ}

INCLUDE_PATHS += -I$(CONFIG_PATH)
INCLUDE_PATHS += -I$(LIB_PATH)
INCLUDE_PATHS += -I$(MBED_PATH)
INCLUDE_PATHS += $(APP_INC) $(LIB_INC) $(MBED_INC)

LIBRARY_PATHS :=
LIBRARIES :=
LINKER_SCRIPT ?= ../mbed-os/targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/device/TOOLCHAIN_GCC_ARM/STM32F446XE.ld

# Objects and Paths
###############################################################################
# Tools and Flags

AS      = arm-none-eabi-gcc
CC      = arm-none-eabi-gcc
CPP     = arm-none-eabi-g++
LD      = arm-none-eabi-gcc
ELF2BIN = arm-none-eabi-objcopy
PREPROC = arm-none-eabi-cpp -E -P -Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r -Wl,--wrap,_realloc_r -Wl,--wrap,_memalign_r -Wl,--wrap,_calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit -Wl,-n -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x8000000 -DMBED_ROM_SIZE=0x80000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x20000 -DMBED_BOOT_STACK_SIZE=1024

ifeq ($(BOARD),nucleo)
	COMMON_FLAGS += -DNUCLEO_PINMAP
else ifeq ($(BOARD),arm)
	COMMON_FLAGS += -DROVERBOARD_ARM_PINMAP
else ifeq ($(BOARD),science)
	COMMON_FLAGS += -DROVERBOARD_SCIENCE_PINMAP
else ifeq ($(BOARD),safety)
	COMMON_FLAGS += -DROVERBOARD_SAFETY_PINMAP
endif

DEFINITIONS += -D__CMSIS_RTOS
DEFINITIONS += -D__CORTEX_M4
DEFINITIONS += -D__FPU_PRESENT=1
DEFINITIONS += -D__MBED__=1
DEFINITIONS += -D__MBED_CMSIS_RTOS_CM
DEFINITIONS += -DARM_MATH_CM4
DEFINITIONS += -DCOMPONENT_NSPE=1
DEFINITIONS += -DCOMPONENT_PSA_SRV_EMUL=1
DEFINITIONS += -DCOMPONENT_PSA_SRV_IMPL=1
DEFINITIONS += -DDEVICE_ANALOGIN=1
DEFINITIONS += -DDEVICE_ANALOGOUT=1
DEFINITIONS += -DDEVICE_CAN=1
DEFINITIONS += -DDEVICE_FLASH=1
DEFINITIONS += -DDEVICE_I2C=1
DEFINITIONS += -DDEVICE_I2C_ASYNCH=1
DEFINITIONS += -DDEVICE_I2CSLAVE=1
DEFINITIONS += -DDEVICE_INTERRUPTIN=1
DEFINITIONS += -DDEVICE_LPTICKER=1
DEFINITIONS += -DDEVICE_MPU=1
DEFINITIONS += -DDEVICE_PORTIN=1
DEFINITIONS += -DDEVICE_PORTINOUT=1
DEFINITIONS += -DDEVICE_PORTOUT=1
DEFINITIONS += -DDEVICE_PWMOUT=1
DEFINITIONS += -DDEVICE_RESET_REASON=1
DEFINITIONS += -DDEVICE_RTC=1
DEFINITIONS += -DDEVICE_SERIAL=1
DEFINITIONS += -DDEVICE_SERIAL_ASYNCH=1
DEFINITIONS += -DDEVICE_SERIAL_FC=1
DEFINITIONS += -DDEVICE_SLEEP=1
DEFINITIONS += -DDEVICE_SPI=1
DEFINITIONS += -DDEVICE_SPI_ASYNCH=1
DEFINITIONS += -DDEVICE_SPISLAVE=1
DEFINITIONS += -DDEVICE_STDIO_MESSAGES=1
DEFINITIONS += -DDEVICE_USTICKER=1
DEFINITIONS += -DDEVICE_WATCHDOG=1
DEFINITIONS += -DMBED_BUILD_TIMESTAMP=1566094560.68
DEFINITIONS += -DMBED_RAM_SIZE=0x20000
DEFINITIONS += -DMBED_RAM_START=0x20000000
DEFINITIONS += -DMBED_ROM_SIZE=0x80000
DEFINITIONS += -DMBED_ROM_START=0x8000000
DEFINITIONS += -DMBED_TRAP_ERRORS_ENABLED=1
DEFINITIONS += -DTARGET_CORTEX
DEFINITIONS += -DTARGET_CORTEX_M
DEFINITIONS += -DTARGET_FAMILY_STM32
DEFINITIONS += -DTARGET_FF_ARDUINO
DEFINITIONS += -DTARGET_FF_MORPHO
DEFINITIONS += -DTARGET_LIKE_CORTEX_M4
DEFINITIONS += -DTARGET_LIKE_MBED
DEFINITIONS += -DTARGET_M4
DEFINITIONS += -DTARGET_NAME=NUCLEO_F446RE
DEFINITIONS += -DTARGET_NUCLEO_F446RE
DEFINITIONS += -DTARGET_RELEASE
DEFINITIONS += -DTARGET_RTOS_M4_M7
DEFINITIONS += -DTARGET_STM
DEFINITIONS += -DTARGET_STM32F4
DEFINITIONS += -DTARGET_STM32F446RE
DEFINITIONS += -DTARGET_STM32F446xE
DEFINITIONS += -DTOOLCHAIN_GCC
DEFINITIONS += -DTOOLCHAIN_GCC_ARM
DEFINITIONS += -DTRANSACTION_QUEUE_SIZE_SPI=2
DEFINITIONS += -DUSB_STM_HAL
DEFINITIONS += -DUSBHOST_OTHER
DEFINITIONS += -DUSE_FULL_LL_DRIVER
DEFINITIONS += -DUSE_HAL_DRIVER

COMMON_FLAGS += -c
COMMON_FLAGS += -fdata-sections
COMMON_FLAGS += -ffunction-sections
COMMON_FLAGS += -fmessage-length=0
COMMON_FLAGS += -fno-delete-null-pointer-checks
COMMON_FLAGS += -fno-exceptions
COMMON_FLAGS += -fomit-frame-pointer
COMMON_FLAGS += -funsigned-char
COMMON_FLAGS += -g1
COMMON_FLAGS += -mcpu=cortex-m4
COMMON_FLAGS += -mfloat-abi=softfp
COMMON_FLAGS += -mfpu=fpv4-sp-d16
COMMON_FLAGS += -MMD
COMMON_FLAGS += -mthumb
COMMON_FLAGS += -Os
COMMON_FLAGS += -Wall
COMMON_FLAGS += -Wextra
COMMON_FLAGS += -Wno-missing-field-initializers
COMMON_FLAGS += -Wno-unused-parameter

C_FLAGS += -std=gnu11
C_FLAGS += -include
C_FLAGS += $(CONFIG_PATH)/mbed_config.h
C_FLAGS += -std=gnu11
C_FLAGS += ${DEFINITIONS}
C_FLAGS += ${COMMON_FLAGS}

CXX_FLAGS += -std=gnu++14
CXX_FLAGS += -fno-rtti
CXX_FLAGS += -Wvla
CXX_FLAGS += -include
CXX_FLAGS += $(CONFIG_PATH)/mbed_config.h
CXX_FLAGS += -std=gnu++14
CXX_FLAGS += -fno-rtti
CXX_FLAGS += -Wvla
CXX_FLAGS += ${DEFINITIONS}
CXX_FLAGS += ${COMMON_FLAGS}

ASM_FLAGS += -x
ASM_FLAGS += assembler-with-cpp

ASM_FLAGS += ${DEFINITIONS}
ASM_FLAGS += ${MBED_INC}
ASM_FLAGS += -include
ASM_FLAGS += $(CONFIG_PATH)/mbed_config.h
ASM_FLAGS += -x
ASM_FLAGS += assembler-with-cpp
ASM_FLAGS += ${COMMON_FLAGS}



LD_FLAGS := -Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r -Wl,--wrap,_realloc_r -Wl,--wrap,_memalign_r -Wl,--wrap,_calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit -Wl,-n -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x8000000 -DMBED_ROM_SIZE=0x80000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x20000 -DMBED_BOOT_STACK_SIZE=1024 
LD_SYS_LIBS := -Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys  -Wl,--end-group

# Tools and Flags
###############################################################################
# Rules

.PHONY: all

all: $(APP_OUT_PATH)/$(PROJECT).bin # $(APP_OUT_PATH)/$(PROJECT).hex size


.s.o:
	+@$(call MAKE_DIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
	@$(AS) -c $(ASM_FLAGS) -o $@ $<

.S.o:
	+@$(call MAKE_DIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
	@$(AS) -c $(ASM_FLAGS) -o $@ $<

.c.o:
	+@$(call MAKE_DIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CC) $(C_FLAGS) $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	+@$(call MAKE_DIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CPP) $(CXX_FLAGS) $(INCLUDE_PATHS) -o $@ $<

$(APP_OUT_PATH)/$(PROJECT).link_script.ld: $(LINKER_SCRIPT)
	@$(PREPROC) $< -o $@

$(APP_OUT_PATH)/$(PROJECT).elf: $(OBJECTS) $(APP_OUT_PATH)/$(PROJECT).link_script.ld 
	+@echo "$(filter %.o, $^)" > .link_options.txt
	+@echo "link: $(notdir $@)"
	@$(LD) $(LD_FLAGS) -T $(filter-out %.o, $^) $(LIBRARY_PATHS) --output $@ @.link_options.txt $(LIBRARIES) $(LD_SYS_LIBS)

$(APP_OUT_PATH)/$(PROJECT).bin: $(APP_OUT_PATH)/$(PROJECT).elf
	$(ELF2BIN) -O binary $< $@
	+@echo "===== bin file ready to flash: $@ ====="

# $(APP_OUT_PATH)/$(PROJECT).hex: $(APP_OUT_PATH)/$(PROJECT).elf
# 	$(ELF2BIN) -O ihex $< $@


# Rules
###############################################################################
# Dependencies

DEPS = $(OBJECTS:.o=.d)
-include $(DEPS)

endif