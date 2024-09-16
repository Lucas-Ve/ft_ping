.PHONY: all clean fclean re

NAME 		= ft_ping.a

SRCS 		= src/main.c
							
OBJS        = ${SRCS:.c=.o}

HEADER 		= includes/ft_ping.h

CC 			= gcc
FLAGS 		= -Wall -Wextra -Werror

LIB 		= ar rc

RM 			= rm -f

all:    	${NAME} Makefile

%.o: 		%.c $(HEADER) Makefile 
			${CC} ${FLAGS} -include${HEADER} -c $< -o $@

$(NAME):    $(OBJS) Makefile
			${LIB} ${NAME} ${OBJS}
			${CC} ${FLAGS} ${NAME} -o ft_ping

clean:
			${RM} ${OBJS}
			${RM} ft_ping

fclean:		clean
			${RM} ${NAME}

re:        	fclean all