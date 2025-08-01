CC	=	gcc
FLAGS	=	-Wextra

SRCDIR	=	src/

PROGRAM	=	jvgate

all: $(PROGRAM)

$(PROGRAM): $(SRCDIR)main.c
	$(CC) $(SRCDIR)main.c $(SRCDIR)gate_functions.c -o $(PROGRAM)