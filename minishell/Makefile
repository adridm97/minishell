NAME 			= minishell
CC 				= gcc
CFLAGS 			= -Wall -Wextra -Werror -g
SOURCES 		= minishell.c lexer.c executor.c special_split.c split_token.c
OBJS_PATH		= ./build
HEADER			= minishell.h
LIBFT_PATH 		= ./libft
LIBFT			 = $(LIBFT_PATH)/libft.a

all: makes $(NAME)

makes:
	make -C $(LIBFT_PATH)

OBJS = $(addprefix $(OBJS_PATH)/, ${SOURCES:.c=.o})

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $^ -o $@ $(OSFLAG)

$(LIBFT):
	make -C $(LIBFT_PATH)

$(OBJS_PATH)/%.o: %.c $(HEADER) Makefile
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf $(OBJS_PATH)
	make clean -C $(LIBFT_PATH)

fclean: clean
	rm -rf $(NAME)
	make fclean -C $(LIBFT_PATH)

re: fclean all

OSFLAG 				:=
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
		OSFLAG = -lreadline -L /home/linuxbrew/.linuxbrew/opt/readline/lib -I /home/linuxbrew/.linuxbrew/opt/readline/include
	endif
	ifeq ($(UNAME_S),Darwin)
		OSFLAG = -lreadline
	endif
	# 	UNAME_P := $(shell uname -p)
	# ifeq ($(UNAME_P),x86_64)
	# 	OSFLAG += -D AMD64
	# endif
	# 	ifneq ($(filter %86,$(UNAME_P)),)
	# OSFLAG += -D IA32
	# 	endif
	# ifneq ($(filter arm%,$(UNAME_P)),)
	# 	OSFLAG += -D ARM
	# endif
endif

.PHONY: all clean fclean re
