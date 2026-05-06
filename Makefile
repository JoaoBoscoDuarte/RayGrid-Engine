# ============================================================
#  Compilador
# ============================================================
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -Iinclude $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lm

# ============================================================
#  Diretórios
# ============================================================
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin
LOG_DIR = logs

# ============================================================
#  Alvo final
# ============================================================
TARGET = $(BIN_DIR)/raygrid

# ============================================================
#  Fontes e objetos (detectados automaticamente)
# ============================================================
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# ============================================================
#  Regras
# ============================================================
all: $(TARGET)

# Linka os objetos no executável final
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compila cada .c em seu respectivo .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Remove artefatos de build
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LOG_DIR)

.PHONY: all clean
