BIN_DIR ?= bin
BUILD_DIR ?= build
SRC_DIRS ?= src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

main: create_folders $(OBJS)
	g++ $(OBJS) -o $(BIN_DIR)/main `pkg-config --cflags --libs opencv` -g -Wall -pg

$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	g++ $(CPPFLAGS) -pg -c $< -o $@ `pkg-config --cflags --libs opencv` -g -Wall 

create_folders:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)

.PHONY:doc
doc:
	@doxygen ./Doxyfile
	
clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)



