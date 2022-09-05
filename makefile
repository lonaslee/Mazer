CC := gcc
CMDSEP := &

SRCDIR := source/
ODIR := output/

IARGS := -Iheaders/ -Iheaders/mazes -ISDL2/x86_64-w64-mingw32/include/SDL2 -ISDL2_image/x86_64-w64-mingw32/include/SDL2
LARGS := -LD:/c/mazer/SDL2/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -LD:/c/mazer/SDL2_image/x86_64-w64-mingw32/lib -lSDL2_image
CFLAGS := -c $(IARGS)


all: cmazes cmp lnk
	@echo done
	a.exe

z: cmazes lnk
	@echo done

n: cmp lnk
	@echo done

cmp:
	@echo compiling
	$(foreach sf, $(wildcard $(SRCDIR)*.c), $(CC) $(sf) $(CFLAGS) -o $(ODIR)$(notdir $(basename $(sf))).o $(CMDSEP))

cmazes:
	@echo compiling mazes
	$(foreach mf, $(wildcard $(SRCDIR)mazes/*.c), $(CC) $(mf) $(CFLAGS) -o $(ODIR)$(notdir $(basename $(mf))).o $(CMDSEP))

lnk:
	@echo linking
	$(CC) $(wildcard $(ODIR)*.o) -o a.exe $(LARGS)
