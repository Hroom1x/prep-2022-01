TARGET = ./main.out
TARGET_TEST = ./main_test.out
HDRS_DIR = project/include

SRCS = \
		project/src/main.c \
		project/src/master_write.c \
		project/src/transaction_write.c \
		project/src/update_data.c

.PHONY: all build rebuild check test memtest clean

all: clean check test memtest

$(TARGET): $(SRCS)
	$(CC) -Wpedantic -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET) $(CFLAGS) $(SRCS)

$(TARGET_TEST): $(SRCS)
	$(CC) -Wpedantic -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET_TEST) $(CFLAGS) project/src/test_module.c

build: $(TARGET) $(TARGET_TEST)

rebuild: clean build

check:
	./run_linters.sh

test: $(TARGET)
	./btests/run.sh $(TARGET)

memtest: $(TARGET)
	./btests/run.sh $(TARGET) --memcheck

clean:
	rm -rf $(TARGET) *.dat
