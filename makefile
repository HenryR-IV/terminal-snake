OBJ_DIR=obj
BIN_DIR=bin

CC=g++
SRCS:=$(wildcard src/*.cpp)
OBJS=$(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
FLAGS=-I include/

all: $(BIN_DIR)/snake

install: FLAGS += -D INSTALL

install: all .snake
	cp $(BIN_DIR)/snake $(HOME)/.local/bin/

uninstall:
	rm $(HOME)/.local/bin/snake
	rm -r $(HOME)/.snake

$(BIN_DIR)/snake: $(OBJS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(FLAGS)

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CC) $< -c -o $@ $(FLAGS)

$(OBJ_DIR):
	@mkdir -p $@

$(BIN_DIR):
	@mkdir -p $@

.snake:
	@mkdir -p $(HOME)/$@

clean:
	rm $(OBJ_DIR)/* $(BIN_DIR)/*
