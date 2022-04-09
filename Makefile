TARGET = ./main.out
UTARGET = project/test/main_test.o
HDRS_DIR = project/include

SRCS = \
		project/src/main.c \
		project/src/master_write.c \
		project/src/transaction_write.c \
		project/src/update_data.c \
		project/src/file_reader.c

USRCS = \
		project/src/master_write.c \
        project/src/transaction_write.c \
        project/src/update_data.c \
        project/src/file_reader.c \
        project/test/main_test.c

.PHONY: all build rebuild check test utest memtest clean

all: clean check test utest memtest

$(TARGET): $(SRCS)
	$(CC) -Wpedantic -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET) $(CFLAGS) $(SRCS)

$(UTARGET):
	$(CC) -Wpedantic -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(UTARGET) $(CFLAGS) $(USRCS)

build: $(TARGET) $(UTARGET)

rebuild: clean build

check:
	./run_linters.sh

test: $(TARGET)
	./btests/run.sh $(TARGET)

utest: $(UTARGET)
	eval "$(UTARGET)"
	@rm -rf $(UTARGET) *.dat

memtest: $(TARGET)
	./btests/run.sh $(TARGET) --memcheck

clean:
	rm -rf $(TARGET) $(UTARGET) *.dat
