CC = gcc
CFLAGS = -Wall -g
INCLUDES = $(shell sdl-config --cflags) -I../lapis
INCLUDES += -I/usr/include/lua
LDFLAGS = -Wl,-rpath,'.'
LIBS = -lGL -lGLU $(shell sdl-config --libs) -lSDL_image
LIBS += -lSDL_mixer -lSDL_ttf -L. -llapis
LIBS += -llua

SRCS = \
	main.c \
	player.c \
	map.c \
	map_view.c \
	zombie_controller.c \
	zombie.c \
	bullet.c \
	hud.c

OBJS = $(SRCS:.c=.o)

MAIN = ztrap


.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: depend clean

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LDFLAGS) $(LIBS)


clean:
	rm -f *.o *~ $(MAIN)

depend: $(SRCS)
	$(CC) -M $(CFLAGS) $(INCLUDES) $^ > $@

include depend
