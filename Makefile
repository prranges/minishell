# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: prranges <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/06 14:41:12 by prranges          #+#    #+#              #
#    Updated: 2021/10/06 14:41:19 by prranges         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 	= minishell

LIBFT	= ./libft/libft.a
LFT_DIR = ./libft

SRCS	= minishell.c \
		  env_01_lists.c \
		  env_02_array.c \
		  signals.c \
		  pipe_01.c \
		  utils_01.c \
		  redir_01.c \
		  redir_02.c \
		  lexer_01_quotes.c \
		  lexer_02_dollar.c \
		  token_01.c \
		  parcer_01.c \
		  executer_01.c \
		  builtins_01_cd.c \
		  builtins_02_export.c \
		  builtins_03_unset.c \
		  builtins_04_env_exit.c \
		  builtins_05_pwd_echo.c \
		  preparcer_01.c \
		  pipex/pipex.c \
		  get_next_line/get_next_line.c \
		  get_next_line/get_next_line_utils.c

CC	= gcc
CFLAGS 	= -Wall -Wextra -Werror

RM	= rm -rf

OBJS_DIR 	= temp

OBJS	= $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))

HDR	= -I./
LFT_HDR = -I$(LFT_DIR)
LFT_BIN = -L$(LFT_DIR) -lft
RDLN	= -lreadline -L ~/.brew/Cellar/readline/8.1.1/lib \
		  -I~/.brew/Cellar/readline/8.1.1/include

all: $(LIBFT) $(NAME)

FORCE:	;

$(LIBFT): FORCE
	make -C $(LFT_DIR)

$(OBJS_DIR):
	mkdir $@
	mkdir $@/pipex
	mkdir $@/get_next_line

$(OBJS_DIR)/%.o: %.c Makefile $(OBJS_DIR)
	gcc -g $(CFLAGS) $(HDR) $(LFT_HDR) -c $< -o $@

$(NAME): $(OBJS) ./minishell.h
	gcc -g $(OBJS) $(LFT_BIN) -o $@ $(RDLN)

clean:
	$(RM) $(OBJS_DIR)
	make -C ./libft clean
	
fclean: clean
	$(RM) $(NAME)
	make -C ./libft fclean

re: fclean all

.PHONY:		all clean fclean re
