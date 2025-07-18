#include "raycast.hpp"
#include <GLFW/glfw3.h>

void check_leaks() {
	system("leaks rc");
}

int main(int argc, char **argv) {
	// std::atexit(check_leaks);
	// oASD();

	raycast *game;
	char* defaultMap = "maps/map1";
	
	try {
		game = new raycast();
		
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
		if (argc == 2)
		{
			defaultMap = argv[1];
		}	
		if (game->initGame(defaultMap)) {
			delete game;
			std::cerr << "Failed to initialize game" << std::endl;
			return 1;
		}
		if (game->startGame()) {
			delete game;
			std::cerr << "Failed to start game" << std::endl;
			return 1;
		}
		delete game;
	}
	catch (std::bad_alloc){
		 std::cerr << "Memory allocation failed: std::bad_alloc" << std::endl;
	}
	return 0;
}