# Default Target
.DEFAULT_GOAL := build

# Delete Suffix Rules
.SUFFIXES :=

# Phony Targets
.PHONY := build verify_app_target_tuple_config verify_app_target_tuple_is_specified clean all

# Options
CLEAN ?=
BUILD_PROFILE ?= develop

MBED_TOOLS_FLAGS :=
ifneq (,$(CLEAN))
	MBED_TOOLS_FLAGS += --clean
endif
ifneq (,$(BUILD_PROFILE))
	MBED_TOOLS_FLAGS += --profile $(BUILD_PROFILE)
endif

# PATHS
MAKEFILE_DIR  := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
APPS_DIR      := $(abspath $(MAKEFILE_DIR)/apps)
TEST_APPS_DIR := $(abspath $(MAKEFILE_DIR)/test-apps)
TARGETS_DIR   := $(abspath $(MAKEFILE_DIR)/targets)

CUSTOM_TARGETS_JSON := $(abspath $(TARGETS_DIR)/custom_targets.json)

# APP and TARGET lists
APPS_LIST := $(sort $(patsubst $(APPS_DIR)/%/,%, $(wildcard $(APPS_DIR)/*/)))
APPS_LIST += $(sort $(patsubst $(TEST_APPS_DIR)/%/,%, $(wildcard $(TEST_APPS_DIR)/*/)))

TARGETS_LIST := $(sort $(patsubst $(TARGETS_DIR)/TARGET_%/,%, $(wildcard $(TARGETS_DIR)/TARGET_*/)))

NUMBER_OF_SUPPORTED_CONFIGS := $(shell python3 scripts/build_configurations_helper.py count-supported-configs)

verify_app_target_tuple_is_specified:
ifeq (,$(findstring $(APP), $(APPS_LIST)))

ifeq (,$(APP))
	$(warning APP is not set)
else
	$(warning $(APP) is not an app within the $(APPS_DIR) folder)
endif

	$(warning Using APP=app_name, select one of the following detected apps:)
	$(foreach APP_NAME,$(APPS_LIST),$(warning $(APP_NAME)))
	$(error )
endif

ifeq (,$(findstring $(TARGET), $(TARGETS_LIST)))

ifeq (,$(TARGET))
	$(warning TARGET is not set)
else
	$(warning $(TARGET) is not a target within the $(TARGETS_DIR) folder)
endif

	$(warning Using TARGET=board_name, select one of the following detected board targets:)
	$(foreach TARGET_NAME,$(TARGETS_LIST),$(warning $(TARGET_NAME)))
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
	@echo Building $(APP) app for $(TARGET) board with $(BUILD_PROFILE) profile
	APP=$(APP); mbed-tools compile --toolchain GCC_ARM --mbed-target $(TARGET) --custom-targets-json $(CUSTOM_TARGETS_JSON) $(MBED_TOOLS_FLAGS)

all:
	@echo Building all $(NUMBER_OF_SUPPORTED_CONFIGS) supported app/target configs
	@n=0; \
	success=0; \
	while [ $${n} -lt $(NUMBER_OF_SUPPORTED_CONFIGS) ] && [ $${success} -eq 0 ]; do \
		make $$(python3 scripts/build_configurations_helper.py print-supported-config $${n}); \
		success=$${?}; \
		n=`expr $$n + 1`; \
	done; \
	exit $${success};
	@echo Successfully built all $(NUMBER_OF_SUPPORTED_CONFIGS) supported app/target configs

clean:
	@echo "Deleting all build files"
	rm -rf cmake_build

