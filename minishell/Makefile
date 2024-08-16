# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adrian <adrian@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/04 16:50:24 by adrian            #+#    #+#              #
#    Updated: 2024/08/16 15:38:09 by adrian           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME            = minishell
CC              = cc
CFLAGS          = -Wall -Wextra -Werror -fPIE
SOURCES         =	minishell.c \
					lexer.c \
					executor.c \
					executor2.c \
					special_split.c \
					split_token.c \
					signals.c \
					env.c \
					env2.c \
					env3.c \
					pwd.c \
					pwd2.c \
					status.c \
					heredoc.c \
					print.c \
					echo.c \
					cd.c \
					mat.c \
					handle.c \
					handle2.c \
					handle_fd.c \
					pipes.c \
					exit.c \
					builtins.c \
					unset.c \
					export.c \
					redirs.c \
					redirs2.c \
					args.c \
					key.c \
					clean.c \
					token.c \
					checkers.c \
					utils.c \
					utils2.c

OBJS_PATH       = ./build
HEADER          = minishell.h
LIBFT_PATH      = ./libft
LIBFT           = $(LIBFT_PATH)/libft.a
LIBDIR          = libs
READLINEDIR     = $(LIBDIR)/readline-4.3
LIBREADLINE     = $(READLINEDIR)/lib/libreadline.a
LIBHISTORY      = $(READLINEDIR)/lib/libhistory.a
INCLUDEDIR      = $(READLINEDIR)/include
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
	$(MAKE) -C $(READLINEDIR) clean
	rm -f $(READLINEDIR)/lib/libhistory.a
	rm -f $(READLINEDIR)/lib/libhistory.old
	rm -f $(READLINEDIR)/lib/libreadline.a
	rm -f $(READLINEDIR)/lib/libreadline.old
	rm -f $(READLINEDIR)/lib/libhistory.so*
	rm -f $(READLINEDIR)/lib/libreadline.so*

fclean: clean
	rm -rf $(NAME)
	rm -rf $(LIBREADLINE) $(LIBHISTORY)
	make fclean -C $(LIBFT_PATH)
	$(MAKE) -C $(READLINEDIR) distclean

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
