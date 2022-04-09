TARGET = ./main_test.out
HDRS_DIR = ../include

SRCS = \
		../src/master_write.c \
		../src/transaction_write.c \
		../src/update_data.c \
		../src/file_reader.c \
		./src/main_test.c

.PHONY: all build rebuild test clean

all: clean test

$(TARGET): $(SRCS)
	$(CC) -Wpedantic -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET) $(CFLAGS) $(SRCS)

build: $(TARGET)

rebuild: clean build

test: $(TARGET)
	eval "$(TARGET)"

memtest: $(TARGET)
	../../btests/run.sh $(TARGET) --memcheck

clean:
	rm -rf $(TARGET) *.dat
