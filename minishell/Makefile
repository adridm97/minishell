NAME            = minishell
CC              = gcc
CFLAGS          = -Wall -Wextra -Werror -g
SOURCES         = minishell.c lexer.c executor.c special_split.c split_token.c signals.c
OBJS_PATH       = ./build
HEADER          = minishell.h
LIBFT_PATH      = ./libft
LIBFT           = $(LIBFT_PATH)/libft.a
LIBDIR          = libs
READLINEDIR     = $(LIBDIR)/readline-4.3
INCLUDEDIR      = $(READLINEDIR)/include
READLINE_LIB_HISTORY = $(READLINE_LIB_DIR)/libhistory.a
READLINE_LIB_READLINE = $(READLINE_LIB_DIR)/libreadline.a
# Ruta absoluta del directorio de Readline
ABS_READLINEDIR := $(shell pwd)/$(READLINEDIR)

all: makes $(NAME)

makes:
	make -C $(LIBFT_PATH)
	$(MAKE) $(LIBREADLINE)

OBJS = $(addprefix $(OBJS_PATH)/, ${SOURCES:.c=.o})

$(NAME): $(OBJS) $(LIBFT) $(LIBREADLINE)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBREADLINE) -o $@ $(OSFLAG)

$(LIBFT):
	make -C $(LIBFT_PATH)

$(LIBREADLINE): $(READLINEDIR)/Makefile
	cd $(READLINEDIR) && ./configure --prefix=$(ABS_READLINEDIR)
	$(MAKE) -C $(READLINEDIR)
	$(MAKE) -C $(READLINEDIR) install

$(READLINEDIR)/Makefile:
	cd $(READLINEDIR) && ./configure --prefix=$(ABS_READLINEDIR)

$(OBJS_PATH)/%.o: %.c $(HEADER) Makefile
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -I$(ABS_READLINEDIR)/include -c $< -o $@ 

clean:
	rm -rf $(OBJS_PATH)
	make clean -C $(LIBFT_PATH)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(READLINE_LIB_HISTORY) $(READLINE_LIB_READLINE)
	make fclean -C $(LIBFT_PATH)

re: fclean all

OSFLAG              :=
ifeq ($(OS),Windows_NT)
	OSFLAG += -D WIN32
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		OSFLAG += -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		OSFLAG += -D IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OSFLAG = -L$(ABS_READLINEDIR)/lib -I$(ABS_READLINEDIR)/include -lreadline -lncurses
	endif
	ifeq ($(UNAME_S),Darwin)
		OSFLAG = -L$(ABS_READLINEDIR)/lib -I$(ABS_READLINEDIR)/include -lreadline -lncurses
	endif
endif

.PHONY: all clean fclean re
