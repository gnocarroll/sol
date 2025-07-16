include .env
export

CFLAGS= -Wall -Werror -I$(INCLUDE_DIR) -I$(RESOURCE_DIR) 

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

MAIN_EXE_PATH=$(BIN_DIR)/$(MAIN_EXE)

all: $(MAIN_EXE_PATH)

$(MAIN_EXE_PATH): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ_FILES) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-rm -rf $(OBJ_FILES) $(MAIN_EXE_PATH)

.PHONY: run
run: $(MAIN_EXE_PATH)
	@$(MAIN_EXE_PATH)

.PHONY: test
test:
	$(PYTHON) -m $(TEST_MODULE)
