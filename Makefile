ifeq ($(OS),Windows_NT)
    CLANG = clang++ -std=c++17
    CFLAGS = -DGL_SILENCE_DEPRECATION -w -MMD -MP
    GLFW_ROOT = glfw-3.4.bin.WIN64
    GLFW_LIB_DIR = $(GLFW_ROOT)/lib
    GLFW_INCLUDE_DIR = $(GLFW_ROOT)/include
    LDFLAGS = -L"$(GLFW_LIB_DIR)" -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lshell32
    INCLUDE = -I./include -I"$(GLFW_INCLUDE_DIR)"
    NAME = rc.exe
    RM_CMD_OBJ = @if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)" 2>NUL || true
    RM_CMD_DEPS = @del /s /q $(subst /,\,$(DEPS)) 2>NUL || true
    RM_CMD_NAME = @del /s /q $(NAME) 2>NUL || true
    MKDIR_P = @if not exist "$(subst /,\\,$(dir $@))" mkdir "$(subst /,\\,$(dir $@))"
else
    CLANG = clang++ -std=c++17
    CFLAGS = -DGL_SILENCE_DEPRECATION -w -MMD -MP
    LDFLAGS = -L/opt/homebrew/lib -lglfw -framework OpenGL
    INCLUDE = -I./include -I/opt/homebrew/include
    NAME = rc
    RM_CMD_OBJ = $(RM) $(OBJ_DIR)
    RM_CMD_DEPS = $(RM) $(DEPS)
    RM_CMD_NAME = $(RM) $(NAME)
    MKDIR_P = @mkdir -p $(dir $@)
    RM = rm -rf
endif

CPP_SRCS = $(wildcard src/*.cpp)
OTHER_SRCS = main.cpp
SRCS = $(CPP_SRCS) $(OTHER_SRCS)
OBJ_DIR = obj
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Build target
all: $(NAME)

$(NAME): $(OBJS)
	$(CLANG) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

# Rule for compiling object files
$(OBJ_DIR)/%.o: %.cpp
	$(MKDIR_P)
	$(CLANG) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	$(RM_CMD_OBJ)
	$(RM_CMD_DEPS)
else
	$(RM_CMD_OBJ)
	$(RM_CMD_NAME)
endif

fclean: clean
	$(RM_CMD_NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
