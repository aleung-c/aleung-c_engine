#---------------------------------------------------#
#													#
#	aleung-c engine C++ Makefile.					#
#													#
#---------------------------------------------------#

NAME = libaleung-c_engine.a

SRC = ./src/GameEngineController/GameEngineController.cpp \
./src/GameEngineController/GameEngineController.cpp \
./src/GameEngineController/GameEngineController_freetype_font.cpp \
./src/GameEngineController/GameEngineController_parsing.cpp \
./src/GameEngineController/GameEngineController_shaders.cpp \
./src/GameEngineController/GameEngineController_textures.cpp \
./src/GameEngineController/GameEngineController_matrices.cpp \
./src/GameEngineController/GameEngineController_camera.cpp \
./src/GameEngineController/GameEngineController_rendering.cpp \
./src/Managers/TextureManager.cpp \
./src/ObjectsComponents/TextureComponent.cpp \
./src/ObjectsComponents/MorphTargetAnimation.cpp \
./src/EngineObjects/EngineObject.cpp \
./src/EngineObjects/GameObject.cpp \
./src/EngineObjects/GameObject_Parsing.cpp \
./src/EngineObjects/GameUIObject.cpp \
./src/EngineObjects/GameTextObject.cpp

OBJ = $(SRC:.cpp=.o)

CC = clang++ -g -c -Wall -Werror -Wextra $(FREETYPE2_I)

# Linking opengl and lib GLFW.
GL = -framework OpenGL
GLFW = -framework Cocoa -framework CoreVideo -framework IOKit -framework GLUT -L./glfw-3.2.1/src -lglfw3

# For freetype true font loading library
FREETYPE2_L = -L./freetype-2.4.0/objs -lfreetype
FREETYPE2_I = -I./freetype-2.4.0/include

all: freetype $(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)
	echo - aleung-c engine compiled

%.o: %.cpp
	$(CC) -o $@ -c $<

freetype :
	make -C ./freetype-2.4.0/

clean:
	rm -rf $(OBJ)
	make clean -C ./freetype-2.4.0/

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
