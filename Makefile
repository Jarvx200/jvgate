CC	=	gcc
FLAGS	=	-Wextra

SRCDIR	=	src/

PROGRAM	=	jvgate

all: $(PROGRAM)

$(PROGRAM): $(SRCDIR)main.c
	$(CC) $(SRCDIR)main.c $(SRCDIR)logic_elements.c $(SRCDIR)gates.c -o $(PROGRAM)