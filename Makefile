.PHONY: all clean cleanall



EXEC_PATH = exec
INCLUDE_PATH = include

SRC_PATH  = src
SRC_CORE_PATH = $(SRC_PATH)/core
SRC_TUI_PATH = $(SRC_PATH)/tui



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

TUI_FILES = $(addprefix $(SRC_TUI_PATH)/,\
	$(addprefix popups/,\
		popup_pattern.c \
		popup_text_message.c \
		popup_select.c \
	)\
	$(addprefix menu/,\
		menu.c \
	)\
)


PROG_NAME = program
GCC = gcc
CFLAGS = -I $(INCLUDE_PATH) -g
CLIBS = -lpanelw -lncursesw



MAIN_FILE = main.c

all: $(PROG_NAME)
clean:
	find . -name "*.o" | while read -r file; do rm "$$file"; done
cleanall: clean
	find . -name "*.a" | while read -r file; do rm "$$file"; done
	if [ -f "$(PROG_NAME)" ]; then rm -rf "$(PROG_NAME)"; fi

$(PROG_NAME): $(SRC_PATH)/$(subst .c,.o,$(MAIN_FILE)) $(SRC_PATH)/libcore.a $(SRC_PATH)/libtui.a
	$(GCC) $(CFLAGS) -o $@ $^ $(CLIBS)

%.o: %.c
	$(GCC) -c $(CFLAGS) -o $@ $^ $(CLIBS)

$(SRC_PATH)/libcore.a: $(subst .c,.o,$(CORE_FILES))
	ar rc $@ $^
	ranlib $@

$(SRC_PATH)/libtui.a: $(subst .c,.o,$(TUI_FILES))
	ar rc $@ $^
	ranlib $@