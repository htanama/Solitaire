  #OBJS specifies which files to compile as part of the project                                                               
  OBJS = Card.cpp Game.cpp main.cpp
  
  #CC specifies which compiler we're using
  CC = x86_64-w64-mingw32-g++-win32
  
  #INCLUDE_PATHS specifies the additional include paths we'll need
  INCLUDE_PATHS = -I/usr/local/include/mingw64/SFML-2.6.1/include -I/usr/include/c++/12
  
  #LIBRARY_PATHS specifies the additional library paths we'll need
  LIBRARY_PATHS = -L/usr/local/include/mingw64/SFML-2.6.1/lib
  
  #COMPILER_FLAGS specifies the addtional compilation options we're using
  #-static-libgcc -static-libstdc++
  #-Wunsafe-loop-optimizations -Weffc++ -pedantic
  COMPILER_FLAGS = -g -Wall -static-libgcc -static-libstdc++
  
  #LINKER_FLAGS specifies the libraries we're linking against -lSDL2_ttf -lSDL2_mixer
  #-static -lpthread -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
  LINKER_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
  
  #OBJ_NAME specifies the name of our exectuable
  OBJ_NAME = TestSolitaire.exe
  
  #This is the target that compiles our executable
  #Ex: g++ SaveMainLight.cpp -IC:\mingw64\include -LC:\mingw64\lib -g -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o Mingw3  2_SaveMainLight.exe
  all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
