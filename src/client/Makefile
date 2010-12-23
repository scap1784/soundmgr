BINDIR	=	..\..\bin
BINEXT	=	.exe
NAME	=	$(BINDIR)\client
SRCS	=	client.c	\
			commands.c	\
			iodevice.c	\
			reboot.c
OBJS	=	$(SRCS:.c=.o)

INCDIR	=	../../inc
LIBDIR	=	../../lib

CC		=	gcc.exe
RM		=	del /F /Q

CFLAGS	=	-Wall -Wextra -ansi -pedantic -I$(INCDIR)
LDFLAGS	=	-L$(LIBDIR)

$(NAME)	:	$(OBJS)

all		:	$(NAME)
			$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(NAME)
			
clean	:
			$(RM) $(OBJS) 2> nul

fclean	:	clean
			$(RM) $(NAME)$(BINEXT) 2> nul

re		:	fclean all
