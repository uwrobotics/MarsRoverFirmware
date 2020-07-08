# Makefile for Mars Rover 2020 control board firmware applications
###############################################################################
# Can be used to cross-compile compatible apps on different STM32-F446RE boards
# Just run `make` command in same directory to see options.
#
# Example: $ make APP=test_blinky TARGET=nucleo
#          $ make APP=test_blinky TARGET=science
#          $ make APP=arm_lower   TARGET=arm
#
###############################################################################

BUILD_FOLDER   := build
BIN_FOLDER     := bin
TARGETS_FOLDER := targets
APPS_FOLDER    := apps
OBJS_FOLDER    := obj
BIN_OUT_PATH   := ../$(BUILD_FOLDER)/$(APPS_FOLDER)/$(BIN_FOLDER)/$(APP)
OBJ_PATH       := ../$(BUILD_FOLDER)/$(OBJS_FOLDER)
TARGET_PATH    := ../$(TARGETS_FOLDER)/$(TARGET)
APP_PATH       := ../$(APPS_FOLDER)/$(APP)
LIB_PATH       := ../lib
MBED_PATH      := ../mbed-os
CONFIG_PATH    := ../config

LAST_BOARD_TARGET := $(shell cat $(BUILD_FOLDER)/LAST_BOARD_TARGET 2>/dev/null)# ignores non-existant file error

# Utility
###############################################################################

# Whitespace definitions
EMPTY :=
SPACE := ${EMPTY} ${EMPTY}
${SPACE} := ${SPACE}

define \n


endef

# Cross-platform commands
ifeq ($(shell echo $$OS),$$OS)
    MAKE_DIR = if not exist "$(1)" mkdir "$(1)"
    RM_DIR = rmdir /S /Q "$(1)"
    RM_FILE_TYPE = del /S /Q "$(1)" "$(2)"
else
    MAKE_DIR = '$(SHELL)' -c "mkdir -p \"$(1)\""
    RM_DIR = '$(SHELL)' -c "rm -rf \"$(1)\""
    RM_FILE_TYPE = '$(SHELL)' -c "find $(1) -name \"$(2)\" -delete -print"
endif
                        
# Other Convenience Commands
REPLACE_SPACES = $(subst $(SPACE),?,$(1))
RESTORE_SPACES = $(subst ?,$(SPACE),$(1))
NOTDIR_PATH_WITH_SPACES = $(call RESTORE_SPACES,$(notdir $(call REPLACE_SPACES,$(1))))

# If not in build directory, move to the build directory
ifneq ($(BUILD_FOLDER),$(call NOTDIR_PATH_WITH_SPACES,$(CURDIR)))

.PHONY: $(BUILD_FOLDER) clean
all:
	+@$(call MAKE_DIR,$(BUILD_FOLDER))
ifeq (,$(findstring $(APP), $(patsubst $(APPS_FOLDER)/%/,%,$(sort $(dir $(wildcard $(APPS_FOLDER)/*/))))))
	$(error APP is not set or is not supported. ${\n}Select an app to build with APP=app_name:${\n}${\n}$(subst ${ },${\n},$(patsubst $(APPS_FOLDER)/%/,%,$(sort $(dir $(wildcard $(APPS_FOLDER)/*/)))))${\n}${\n})
endif
ifeq (,$(findstring $(TARGET), $(patsubst $(TARGETS_FOLDER)/%/,%,$(sort $(dir $(wildcard $(TARGETS_FOLDER)/*/))))))
	$(error TARGET is not set or is not supported. ${\n}Select a target with TARGET=board_name:${\n}${\n}$(subst ${ },${\n},$(patsubst $(TARGETS_FOLDER)/%/,%,$(sort $(dir $(wildcard $(TARGETS_FOLDER)/*/)))))${\n}${\n})
endif
ifneq ($(TARGET),$(LAST_BOARD_TARGET))
	$(info New TARGET detected. Recompiling all files.)
	+@$(call RM_DIR,$(BUILD_FOLDER)/$(OBJS_FOLDER))
	+@echo $(TARGET) > $(BUILD_FOLDER)/LAST_BOARD_TARGET
endif
	+@$(MAKE) --directory='$(BUILD_FOLDER)' --file='$(abspath $(lastword $(MAKEFILE_LIST)))' $(MAKECMDGOALS)

$(BUILD_FOLDER): all

clean :
	$(call RM_DIR,$(BUILD_FOLDER))
	$(call RM_FILE_TYPE,$(APPS_FOLDER),*.d)
	$(call RM_FILE_TYPE,$(APPS_FOLDER),*.o)
	$(call RM_FILE_TYPE,lib,*.d)
	$(call RM_FILE_TYPE,lib,*.o)

clean-mbed : 
	$(call RM_DIR,$(BUILD_FOLDER)/obj/mbed-os)


clean-all :
	$(call RM_DIR,$(BUILD_FOLDER))
	$(call RM_FILE_TYPE,.,*.d)
	$(call RM_FILE_TYPE,.,*.o)

else

# Trick rules into thinking we are in the root, when we are in the build dir
VPATH = ..

# Project settings
###############################################################################

PROJECT := $(APP).$(TARGET)

# Objects and Paths
###############################################################################

APP_INC += -I$(APP_PATH)
LIB_INC += $(addprefix -I,$(wildcard $(LIB_PATH)/*))

APP_SRC_C += $(wildcard $(APP_PATH)/*.c)
LIB_SRC_C += $(wildcard $(LIB_PATH)/*/*.c)

APP_SRC_CPP += $(wildcard $(APP_PATH)/*.cpp)
LIB_SRC_CPP += $(wildcard $(LIB_PATH)/*/*.cpp)

UWRT_SRC_C   = $(APP_SRC_C)   $(LIB_SRC_C)
UWRT_SRC_CPP = $(APP_SRC_CPP) $(LIB_SRC_CPP)

TARGET_SRC += ${TARGET_PATH}/PeripheralPins.c
TARGET_SRC += ${TARGET_PATH}/system_clock.c

include ${MBED_PATH}/mbedfile

UWRT_C_OBJ := $(subst ../,${OBJ_PATH}/,$(UWRT_SRC_C:.c=.o))
UWRT_CPP_OBJECTS := $(subst ../,${OBJ_PATH}/,$(UWRT_SRC_CPP:.cpp=.o))
TARGET_OBJ := $(subst ../,${OBJ_PATH}/,$(TARGET_SRC:.c=.o))
MBED_OBJ := $(subst ../,${OBJ_PATH}/,$(MBED_OBJ))

OBJECTS += ${UWRT_C_OBJ}
OBJECTS += ${UWRT_CPP_OBJECTS}
OBJECTS += ${TARGET_OBJ}
OBJECTS += ${MBED_OBJ}


INCLUDE_PATHS += -I$(CONFIG_PATH)
INCLUDE_PATHS += -I$(LIB_PATH)
INCLUDE_PATHS += -isystem$(MBED_PATH)
INCLUDE_PATHS += -isystem$(MBED_INC)
INCLUDE_PATHS += -isystem${TARGET_PATH}
INCLUDE_PATHS += $(APP_INC) $(LIB_INC)

LINKER_SCRIPT ?= ${MBED_PATH}/targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/device/TOOLCHAIN_GCC_ARM/STM32F446XE.ld

# Tools and Flags
###############################################################################

MAKEFLAGS += -j$(nproc)

AS      = arm-none-eabi-gcc
CC      = arm-none-eabi-gcc
CPP     = arm-none-eabi-g++
LD      = arm-none-eabi-gcc
ELF2BIN = arm-none-eabi-objcopy

MEM_DEFINITIONS := -DMBED_ROM_START=0x8000000 -DMBED_ROM_SIZE=0x80000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x20000 -DMBED_BOOT_STACK_SIZE=1024

LD_FLAGS := -Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r -Wl,--wrap,_realloc_r -Wl,--wrap,_memalign_r -Wl,--wrap,_calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit -Wl,-n -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp ${MEM_DEFINITIONS}
LD_SYS_LIBS := -Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys  -Wl,--end-group

PREPROC = arm-none-eabi-cpp -E -P ${LD_FLAGS}

DEFINITIONS += -D__CMSIS_RTOS
DEFINITIONS += -D__CORTEX_M4
DEFINITIONS += -D__FPU_PRESENT=1
DEFINITIONS += -D__MBED__=1
DEFINITIONS += -D__MBED_CMSIS_RTOS_CM
DEFINITIONS += -DARM_MATH_CM4
DEFINITIONS += -DCOMPONENT_NSPE=1
DEFINITIONS += -DCOMPONENT_PSA_SRV_EMUL=1
DEFINITIONS += -DCOMPONENT_PSA_SRV_IMPL=1
DEFINITIONS += -DMBED_TRAP_ERRORS_ENABLED=1
DEFINITIONS += -DTARGET_CORTEX
DEFINITIONS += -DTARGET_CORTEX_M
DEFINITIONS += -DTARGET_FAMILY_STM32
DEFINITIONS += -DTARGET_LIKE_CORTEX_M4
DEFINITIONS += -DTARGET_LIKE_MBED
DEFINITIONS += -DTARGET_M4
DEFINITIONS += -DTARGET_NAME=UWRT
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
DEFINITIONS += ${MEM_DEFINITIONS}

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
COMMON_FLAGS += -MP
COMMON_FLAGS += -mthumb
COMMON_FLAGS += -Os
COMMON_FLAGS += -include
COMMON_FLAGS += ${TARGET_PATH}/mbed_config_target.h
COMMON_FLAGS += -include
COMMON_FLAGS += $(CONFIG_PATH)/mbed_config.h

THIRD_PARTY_COMMON_FLAGS += -Wno-missing-field-initializers
THIRD_PARTY_COMMON_FLAGS += -Wno-unused-parameter

UWRT_COMMON_FLAGS += -Wall
UWRT_COMMON_FLAGS += -Wextra
UWRT_COMMON_FLAGS += -Wpedantic
# UWRT_COMMON_FLAGS += -Werror # Uncomment this after resolving all warnings

C_FLAGS += -std=gnu11
C_FLAGS += ${DEFINITIONS}
C_FLAGS += ${COMMON_FLAGS}

CXX_FLAGS += -std=gnu++2a
CXX_FLAGS += -fno-rtti
CXX_FLAGS += -Wvla
CXX_FLAGS += ${DEFINITIONS}
CXX_FLAGS += ${COMMON_FLAGS}

ASM_FLAGS += -x
ASM_FLAGS += assembler-with-cpp
ASM_FLAGS += ${MBED_INC}
ASM_FLAGS += ${DEFINITIONS}
ASM_FLAGS += ${COMMON_FLAGS}

# Rules
###############################################################################

.PHONY: all
all: $(BIN_OUT_PATH)/$(PROJECT).bin

$(OBJ_PATH)/%.o: ../%.s
	+@$(call MAKE_DIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
	@$(AS) -c $(ASM_FLAGS) $< -o $@

$(OBJ_PATH)/%.o: ../%.S
	+@$(call MAKE_DIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
	@$(AS) -c $(ASM_FLAGS) $< -o $@

%.o: C_FLAGS += ${THIRD_PARTY_COMMON_FLAGS}
$(OBJ_PATH)/%.o: ../%.c
	+@$(call MAKE_DIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CC) $(C_FLAGS) $(INCLUDE_PATHS) $< -o $@

%.o: CXX_FLAGS += ${THIRD_PARTY_COMMON_FLAGS}
$(OBJ_PATH)/%.o: ../%.cpp
	+@$(call MAKE_DIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CPP) $(CXX_FLAGS) $(INCLUDE_PATHS) $< -o $@

$(UWRT_C_OBJ): C_FLAGS += ${UWRT_COMMON_FLAGS}
$(UWRT_C_OBJ): $(OBJ_PATH)/%.o: ../%.c
	+@$(call MAKE_DIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CC) $(C_FLAGS) $(INCLUDE_PATHS) $< -o $@

$(UWRT_CPP_OBJECTS): CXX_FLAGS += ${UWRT_COMMON_FLAGS}
$(UWRT_CPP_OBJECTS): $(OBJ_PATH)/%.o: ../%.cpp
	+@$(call MAKE_DIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CPP) $(CXX_FLAGS) $(INCLUDE_PATHS) $< -o $@

$(BIN_OUT_PATH)/$(PROJECT).link_script.ld: $(LINKER_SCRIPT)
	+@$(call MAKE_DIR,$(dir $@))
	@$(PREPROC) $< -o $@

$(BIN_OUT_PATH)/$(PROJECT).elf: $(OBJECTS) $(BIN_OUT_PATH)/$(PROJECT).link_script.ld
	+@echo "$(filter %.o, $^)" > $(BIN_OUT_PATH)/.link_options.txt
	+@echo "Link: $(notdir $@)"
	@$(LD) $(LD_FLAGS) -T $(filter-out %.o, $^) $(LIBRARY_PATHS) --output $@ @$(BIN_OUT_PATH)/.link_options.txt $(LIBRARIES) $(LD_SYS_LIBS)

$(BIN_OUT_PATH)/$(PROJECT).bin: $(BIN_OUT_PATH)/$(PROJECT).elf
	+@$(ELF2BIN) -O binary $< $@
	+@echo "Generate Binary: $(notdir $@)\n"
	+@echo "============== BIN FILE READY TO FLASH ==============\n $(@:../%=%)"
	+@echo "====================================================="

# Dependencies
###############################################################################

DEPS = $(OBJECTS:.o=.d)
-include $(DEPS)

endif
