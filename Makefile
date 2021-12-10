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
		  env_01.c \
		  signals.c \
		  pipe_01.c \
		  utils_01.c \
		  redir_01.c \
		  redir_02.c \
		  lexer_01.c \
		  token_01.c \
		  parcer_01.c \
		  executer_01.c \
		  builtins_01.c \
		  builtins_02.c \
		  preparcer_01.c

CC	= gcc
CFLAGS 	= -Wall -Wextra -Werror

RM	= rm -rf

OBJS_DIR 	= temp

OBJS	= $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))

HDR	= -I./
LFT_HDR = -I$(LFT_DIR)
LFT_BIN = -L$(LFT_DIR) -lft -lreadline

all: $(LIBFT) $(NAME)

FORCE:	;

$(LIBFT): FORCE
	make -C $(LFT_DIR)

$(OBJS_DIR):
	mkdir $@

$(OBJS_DIR)/%.o: %.c Makefile $(OBJS_DIR)
	gcc -g $(CFLAGS) $(HDR) $(LFT_HDR) -c $< -o $@

$(NAME): $(OBJS) ./minishell.h
	gcc -g $(OBJS) $(LFT_BIN) -o $@

clean:
	$(RM) $(OBJS_DIR)
	make -C ./libft clean

bonus:	re
	
fclean: clean
	$(RM) $(NAME)
	make -C ./libft fclean

re: fclean all
