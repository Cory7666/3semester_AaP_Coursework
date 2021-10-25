.PHONY: all clean



EXEC_PATH = exec
INCLUDE_PATH = include

SRC_PATH  = src
SRC_CORE_PATH = $(SRC_PATH)/core



CORE_FILES = $(addprefix $(SRC_CORE_PATH)/,\
	$(addprefix date/,\
		date.c \
	)\
	$(addprefix file_system/,\
		files.c \
		csv_files.c \
		bin_files.c \
		text_files.c \
	)\
	$(addprefix lists/,\
		lists.c \
		lists_cmp_elems.c \
		lists_sort.c \
	)\
	$(addprefix non-standart_funcs/,\
		wcs_converters.c \
		cmp_functions.c \
	)\
	additional_task.c \
)

PROG_NAME = program
GCC = gcc
CFLAGS = -I $(INCLUDE_PATH) -g
CLIBS = 



MAIN_FILE = main.c

all: $(PROG_NAME)
clean:
	find . -name "*.o" | while read -r file; do rm "$$file"; done

$(PROG_NAME): $(SRC_PATH)/$(subst .c,.o,$(MAIN_FILE)) $(SRC_PATH)/libcore.a
	$(GCC) $(CFLAGS) -o $@ $^ $(CLIBS)

%.o: %.c
	$(GCC) -c $(CFLAGS) -o $@ $^ $(CLIBS)

$(SRC_PATH)/libcore.a: $(subst .c,.o,$(CORE_FILES))
	ar rc $@ $^
	ranlib $@