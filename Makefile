UNAME_S := $(shell uname -s)

NAME	= ft_vox

CXX		= c++
CC		= cc

GLAD_DIR 	= includes/glad
STB_DIR		= includes/stb 
SCRS_DIR	= scripts

GLAD_FILE	= $(GLAD_DIR)/src/glad.c
STB_FILE	= $(STB_DIR)/stb_image.h

CXXFLAGS	= -Wall -Wextra -Werror -std=c++17 \
			  		-I./includes \
			  		-I$(GLAD_DIR)/include \
			  		-MMD -MP

CFLAGS		= -Wall -Wextra -Werror \
			  		-I./includes \
			  		-I$(GLAD_DIR)/include \
						-MMD -MP

# macOSとLinuxでライブラリを分ける
ifeq ($(UNAME_S), Darwin)
	CXXFLAGS += -I/usr/local/opt/glfw/include
	CFLAGS	 += -I/usr/local/opt/glfw/include
	LIBS	= -L/usr/local/opt/glfw/lib -lglfw \
					-framework OpenGL \
					-framework Cocoa \
					-framework IOKit \
					-framework CoreVideo
else
	LIBS	= $(shell pkg-config --libs glfw3) -lGL -ldl -lpthread
endif

SRCS	= $(GLAD_DIR)/src/glad.c \
				srcs/main.cpp \
				srcs/gl/Window.cpp \
				srcs/gl/Shader.cpp \
				srcs/gl/TextureArray.cpp \
				srcs/gl/Manipulator.cpp \
				srcs/gl/Camera.cpp \
				srcs/gl/StbImage.cpp \
				srcs/math/Mat4.cpp \
				srcs/math/Vec2.cpp \
				srcs/math/Vec3.cpp \
				srcs/math/Vec4.cpp \
				srcs/voxel/ChunkMesh.cpp \
				srcs/voxel/ChunkData.cpp \
				srcs/voxel/Block.cpp \
				srcs/voxel/VoxCommon.cpp \
				srcs/world/TerrainGenerator.cpp \
				srcs/world/DrawWorld.cpp


OBJS	= $(SRCS:.cpp=.o)
OBJS	:= $(OBJS:.c=.o)

DEPS	= $(OBJS:.o=.d)

all: $(NAME)

# glad生成
$(GLAD_DIR)/src/%.o: $(GLAD_DIR)/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(GLAD_FILE): $(SCRS_DIR)/install_glad.sh
	./$(SCRS_DIR)/install_glad.sh

$(STB_FILE): $(SCRS_DIR)/install_stb.sh
	./$(SCRS_DIR)/install_stb.sh

$(OBJS): | $(GLAD_FILE) $(STB_FILE)

srcs/gl/StbImage.o: $(STB_FILE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(LIBS) -o $(NAME)

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean 
	$(MAKE) all

.PHONY: all clean fclean re

-include $(DEPS)
