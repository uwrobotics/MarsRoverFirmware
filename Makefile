# Default Target
.DEFAULT_GOAL := build

# Delete Suffix Rules
.SUFFIXES :=

# Phony Targets
.PHONY := build verify_app_target_tuple_config verify_app_target_tuple_is_specified clean all

# Environment Options
UWRT_FIRMWARE_MAX_JOBS ?= $(nproc)

# PATHS
MAKEFILE_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
TARGETS_DIR  := $(abspath $(MAKEFILE_DIR)/targets)
APPS_DIR     := $(abspath $(MAKEFILE_DIR)/apps)

TARGETS_LIST := $(sort $(patsubst $(TARGETS_DIR)/%/,%, $(wildcard $(TARGETS_DIR)/*/)))
APPS_LIST    := $(sort $(patsubst $(APPS_DIR)/%/,%, $(wildcard $(APPS_DIR)/*/)))

NUMBER_OF_SUPPORTED_CONFIGS := $(shell python3 scripts/build_configurations_helper.py count-supported-configs)

verify_app_target_tuple_is_specified:
ifeq (,$(findstring $(TARGET), $(TARGETS_LIST)))
	$(info)
	$(info TARGET is not set or $(TARGET) is not a target within the $(TARGETS_DIR) folder)
	$(info)
	$(info Using TARGET=board_name, select one of the following detected board targets:)
	$(foreach TARGET_NAME,$(TARGETS_LIST),$(info $(TARGET_NAME)))
	$(error )
endif
ifeq (,$(findstring $(APP), $(APPS_LIST)))
	$(info APP is not set or $(APP) is not a target within the $(APPS_DIR) folder)
	$(info )
	$(info Using APP=app_name, select one of the following detected apps:)
	$(foreach APP_NAME,$(APPS_LIST),$(info $(APP_NAME)))
	$(error )
endif

verify_app_target_tuple_config: verify_app_target_tuple_is_specified
	@python3 scripts/build_configurations_helper.py verify-config --APP=$(APP) --TARGET=$(TARGET) ; \
	if [ $$? -ne 0 ]; \
	then \
		echo "Do you want to try to build anyways? (Y/N)"; \
		read RESPONSE; \
		if !([ "$$RESPONSE" = "Y" ] || [ "$$RESPONSE" = "y" ] || [ "$$RESPONSE" = "YES" ] || [ "$$RESPONSE" = "yes" ]); \
		then \
			false ; \
		fi; \
	fi

build: verify_app_target_tuple_config
	@mkdir -p build-$(TARGET)-board
	@cmake -S $(MAKEFILE_DIR) -B build-$(TARGET)-board -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=$(MAKEFILE_DIR)/toolchain.cmake -DAPP=$(APP) -DTARGET=$(TARGET)
	@echo Building $(APP) app for $(TARGET) board with max $(UWRT_FIRMWARE_MAX_JOBS) concurrent jobs
	@cmake --build build-$(TARGET)-board --target $(APP).$(TARGET)-board.elf --parallel $(UWRT_FIRMWARE_MAX_JOBS)



all:
	@echo Building all $(NUMBER_OF_SUPPORTED_CONFIGS) supported app/target configs with max $(UWRT_FIRMWARE_MAX_JOBS) concurrent jobs
	@number=0 ; while [ $$number -lt $(NUMBER_OF_SUPPORTED_CONFIGS) ] ; do \
		i=$$(python3 scripts/build_configurations_helper.py print-supported-config $$number) ; \
		make $$i ; \
		number=$$((number+1)) ; \
	done


clean:
	@echo "Deleting all build files"
	rm -rf build-*-board

