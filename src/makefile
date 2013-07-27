CC=g++
INCDIR = -I lib\glut-3.7.6\include -I lib\libpng\include -I lib\zlib-1.2.3 -I lib\OpenAL_SDK\include -I lib\freealut-1.1.0-bin\include
LIBDIR = -L lib\glut_bin -L lib\libpng\lib -L lib\zlib-1.2.3\lib\mt08 -L lib\OpenAL_SDK\libs\Win32 -L lib\freealut-1.1.0-bin\lib
LIB = -lglut32 -lopengl32 -lglaux -llibpng -lzlib lib\OpenAL_SDK\libs\Win32\OpenAL32.lib -lalut
OPT = $(INCDIR) $(LIBDIR) -Wno-Wwrite-strings
EXE = pp.exe
main.exe: main.o pp.o
	$(CC) $(OPT) -o $(EXE) obj\main.o obj\pp.o $(LIB)

main.o: main.cpp
	$(CC) $(OPT) -c main.cpp -o obj\main.o

pp.o: pp.h pp.cpp
	$(CC) $(OPT) -c pp.cpp -o obj\pp.o

clean:
	del obj\main.o $(EXE)

test:
	$(EXE)