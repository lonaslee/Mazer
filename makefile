SRCDIR := source/
ODIR := output/

CC := gcc
CMDSEP := &

IARGS := -Iheaders/ -ISDL2/x86_64-w64-mingw32/include/SDL2 -ISDL2_image/x86_64-w64-mingw32/include/SDL2
LARGS := -LD:/c/mazer/SDL2/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -LD:/c/mazer/SDL2_image/x86_64-w64-mingw32/lib -lSDL2_image
CFLAGS := -c $(IARGS)


all: cmp lnk
	@echo done

cmp:
	@echo compiling
	$(foreach sf, $(wildcard $(SRCDIR)*.c), $(CC) $(sf) $(CFLAGS) -o $(ODIR)$(notdir $(basename $(sf))).o $(CMDSEP))

lnk: cmp
	@echo linking
	$(CC) $(wildcard $(ODIR)*.o) -o app.exe $(LARGS)