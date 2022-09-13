#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install -y make libglfw3-dev pkg-config build-essential
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

#CXX = g++
#CXX = clang++

NAME			= app
IMGUI_DIR		= lib/imgui

SRCEXT      	= cpp
SRCDIR      	= src
OBJEXT      	= o
HEADEREXT		= hpp
HEADERDIR		= include
BUILDDIR    	= obj
SRC				= $(wildcard $(SRCDIR)/*.$(SRCEXT))

SRC 			+= $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SRC 			+= $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS				= $(foreach src,$(SRC),$(BUILDDIR)/$(notdir $(src:.$(SRCEXT)=.$(OBJEXT))))

UNAME_S			:= $(shell uname -s)
LINUX_GL_LIBS	= -lGL

FLAGS			= -std=c++17 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I $(HEADERDIR)
FLAGS			+= -g -Wall -Wextra -Wformat
# I don't care what you say, if you want to fix it make a pull request
FLAGS			+= -DGL_SILENCE_DEPRECATION
LIBS			=

ifeq ($(UNAME_S), Linux) #LINUX
	LIBS += -lGL `pkg-config --static --libs glfw3`
	# LIBS += -lGLESv2 `pkg-config --static --libs glfw3`

	FLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(FLAGS)

	NCPU = $(shell nproc --all) $(NAME)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	FLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(FLAGS)

	NCPU = $(shell sysctl -n hw.ncpu) $(NAME)
endif

MAKEFLAGS+="j $(NCPU)"

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

all: $(NAME)
	@echo Build complete

$(NAME): $(BUILDDIR) $(OBJS)
	$(CXX) -o $@ $(OBJS) $(FLAGS) $(LIBS)

$(BUILDDIR)/%.o: %.cpp
	$(CXX) $(FLAGS) -c -o $(BUILDDIR)/$(notdir $@) $<

$(BUILDDIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(FLAGS) -c -o $(BUILDDIR)/$(notdir $@) $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(FLAGS) -c -o $(BUILDDIR)/$(notdir $@) $<

$(BUILDDIR)/%.o: $(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(FLAGS) -c -o $(BUILDDIR)/$(notdir $@) $<

$(BUILDDIR)/:
	mkdir -p $(BUILDDIR)

clean:
ifneq ($(BUILDDIR),.)
	/bin/rm -rf $(BUILDDIR)/
endif
	rm -f $(NAME)
