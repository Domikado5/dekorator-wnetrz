LIBS=-lGL -lglfw -lGLEW -lassimp
HEADERS=constants.h  lodepng.h  myCube.h  myTeapot.h  shaderprogram.h Furniture.h Room.h
FILES=lodepng.cpp  main_file.cpp  shaderprogram.cpp Furniture.cpp Room.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
