##
## CSULB PROJECT, 2020
## client
## File description:
## Makefile
##

NAME	=	client

SRC	=	src/main.c              \

OBJ	=	$(SRC:.c=.o)

CFLAGS  =	-Iinclude/ -W -Wall -Wextra

ifeq		($(DEBUG), 1)
		CFLAGS += -g
endif

RM	=	rm -f

CC	=	gcc

all:		$(NAME)

$(NAME):	$(OBJ)
		@$(CC) -o $(NAME) $(OBJ)

clean:
		@$(RM) $(OBJ)
		@echo "Cleaning" $(NAME)

fclean:		clean
		@$(RM) $(NAME)
		@echo "Deleting" $(NAME)

re:		fclean all

.PHONY:		re all clean fclean
