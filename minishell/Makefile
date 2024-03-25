NAME 			= minishell
CC 				= gcc
CFLAGS 			= -lreadline
SOURCES 		= minishell.c
OBJS_PATH		= ./build
HEADER			= minishell.h

all: $(NAME)

OBJS = $(addprefix $(OBJS_PATH)/, ${SOURCES:.c=.o})

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(OSFLAG)

$(OBJS_PATH)/%.o: %.c $(HEADER) Makefile
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf $(OBJS_PATH)

fclean: clean
	rm -rf $(NAME)

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
#	ifeq ($(UNAME_S),Darwin)
#		OSFLAG += -D OSX
#	endif
#		UNAME_P := $(shell uname -p)
#	ifeq ($(UNAME_P),x86_64)
#		OSFLAG += -D AMD64
#	endif
#		ifneq ($(filter %86,$(UNAME_P)),)
#	OSFLAG += -D IA32
#		endif
#	ifneq ($(filter arm%,$(UNAME_P)),)
#		OSFLAG += -D ARM
#	endif
endif

.PHONY: all clean fclean re
