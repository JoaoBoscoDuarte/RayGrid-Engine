# Variáveis de Compilador e Flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lm

# Pastas
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

# Nome do Executável
TARGET = $(BIN_DIR)/raygrid

# Localizar todos os arquivos .c na pasta src
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Transformar a lista de .c em uma lista de .o na pasta build
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

# Linkagem final do executável
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compilação dos arquivos de objeto (.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean