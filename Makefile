# Makefile - Build only
CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread -Iinclude

TARGET = scheduler
SRC_DIR = src
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/scheduler.c $(SRC_DIR)/parser.c \
       $(SRC_DIR)/worker.c $(SRC_DIR)/logger.c $(SRC_DIR)/queue.c \
       $(SRC_DIR)/metrics.c

all: $(TARGET)

$(TARGET): $(SRCS)
	@echo "[+] Compiling $(TARGET)..."
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)
	@echo "[+] Compilation successful."

clean:
	rm -f $(TARGET)
	@echo "[+] Cleaned up executable."

.PHONY: all clean