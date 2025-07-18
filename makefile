include .env
export

CFLAGS= -g -std=c++2b -Wall -Werror -I$(INCLUDE_DIR) -I$(RESOURCE_DIR) 

SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp $(SRC_DIR)/*/*/*.cpp)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

MAIN_EXE_PATH=$(BIN_DIR)/$(MAIN_EXE)

all: $(MAIN_EXE_PATH)

$(MAIN_EXE_PATH): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_FILES) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-rm -rf $(OBJ_FILES) $(MAIN_EXE_PATH)

.PHONY: run
run: $(MAIN_EXE_PATH)
	@$(MAIN_EXE_PATH)

.PHONY: test
test:
	$(PYTHON) -m $(TEST_MODULE)
