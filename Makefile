CC	=	gcc
CFLAGS	=	-Wextra -Wall 

SRCDIR	=	src/
LDFLAGS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11 -g 


SRCS := $(wildcard $(SRCDIR)*.c)
OBJS := $(patsubst $(SRCDIR)%.c, $(SRCDIR)%.o, $(SRCS))

RAYGUI_DEFINES = -DRAYGUI_IMPLEMENTATION


PROGRAM	=	jvgate

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS)  -o $(PROGRAM) $(CFLAGS) $(LDFLAGS)
	rm -rf $(OBJS) # TODO: might employ fractional compilation based on modules and hot reloading later



%.o: %.c
	$(CC) $(RAYGUI_DEFINES) $(CFLAGS) -c $< -o $@


clean: $(PROGRAM)
	rm -rf $(PROGRAM)


.PHONY: clean