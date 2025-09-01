Q ?= @
CC = arm-none-eabi-gcc
BUILD_DIR = output
NWLINK = npx --yes -- nwlink@0.0.16
LINK_GC = 1
LTO = 1

define object_for
$(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(basename $(1))))
endef

src = $(addprefix src/,\
  main.c \
)

CFLAGS = -std=c99
CFLAGS += $(shell $(NWLINK) eadk-cflags)
CFLAGS += -Os -Wall
CFLAGS += -ggdb
LDFLAGS = -Wl,--relocatable
LDFLAGS += -nostartfiles

# Try to use specs if available, fallback gracefully
ifneq ($(shell $(CC) --specs=nano.specs -E -x c /dev/null 2>/dev/null; echo $$?),0)
  ifneq ($(shell $(CC) --specs=nosys.specs -E -x c /dev/null 2>/dev/null; echo $$?),0)
    # Neither nano nor nosys specs available - use basic flags
    LDFLAGS += -nostdlib
  else
    LDFLAGS += --specs=nosys.specs
  endif
else
  LDFLAGS += --specs=nano.specs
endif

ifeq ($(LINK_GC),1)
CFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += -Wl,-e,main -Wl,-u,eadk_app_name -Wl,-u,eadk_app_icon -Wl,-u,eadk_api_level
LDFLAGS += -Wl,--gc-sections
endif

ifeq ($(LTO),1)
CFLAGS += -flto -fno-fat-lto-objects
CFLAGS += -fwhole-program
CFLAGS += -fvisibility=internal
LDFLAGS += -flinker-output=nolto-rel
endif

.PHONY: build
build: $(BUILD_DIR)/pi_stream.nwa

.PHONY: check
check: $(BUILD_DIR)/pi_stream.bin

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.nwa src/input.txt
	@echo "BIN     $@"
	$(Q) $(NWLINK) nwa-bin --external-data src/input.txt $< $@

$(BUILD_DIR)/pi_stream.nwa: $(call object_for,$(src)) $(BUILD_DIR)/icon.o
	@echo "LD      $@"
	$(Q) $(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(addprefix $(BUILD_DIR)/,%.o): %.c | $(BUILD_DIR)
	@echo "CC      $^"
	$(Q) $(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/icon.o: src/icon.png
	@echo "ICON    $<"
	$(Q) $(NWLINK) png-icon-o $< $@

.PRECIOUS: $(BUILD_DIR)
$(BUILD_DIR):
	$(Q) mkdir -p $@/src

.PHONY: clean
clean:
	@echo "CLEAN"
	$(Q) rm -rf $(BUILD_DIR)