CC = gcc
FLAGS = -Wall -Werror -Wextra
TARGET = ./main
FP_SRCS = fixedpoint/*.c fixedpoint/utils/*.c fixedpoint/fixedtrig/*.c
FLM_SRCS = fixedlinmath/*.c

# LUT generator
GEN_SRCS = ./fixedpoint/generators/*.c ./fixedpoint/convert.c
GEN_TARGET = ./fixedpoint/generators/fixedgenerators
GEN_LINKS = -lm

# tests
TEST_TARGET = ./tests/testFixed
TEST_FLAGS = -fsanitize=address -fsanitize=undefined
TEST_SRCS = ./tests/libtesting/*.c ./tests/libtesting/fileio/*.c

all:
	$(CC) $(FLAGS) -o $(GEN_TARGET) $(GEN_SRCS) $(GEN_LINKS)
	$(GEN_TARGET)
	clear
	$(CC) $(FLAGS) -g -o $(TARGET) main.c $(FLM_SRCS) $(FP_SRCS) -lm
	$(TARGET)

