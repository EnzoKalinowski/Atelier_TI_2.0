BIN_DIR ?= bin
BUILD_DIR ?= build
SRC_DIRS ?= src

SRCS := $(shell find $(SRC_DIRS) -name *.c++ -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

main: create_folders $(OBJS)
	gcc $(OBJS) -o $(BIN_DIR)/main 

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	gcc $(CPPFLAGS) -c $< -o $@ 

create_folders:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)

.PHONY:doc
doc:
	@doxygen ./Doxyfile
	
clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)
