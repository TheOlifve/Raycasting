#ifndef RAYCAST_HPP
#define RAYCAST_HPP

//UTILS
#include <chrono>
#include <thread>

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdlib.h>
#include <cstdlib>
#include <map> 
#include <vector>


#include "player.hpp"
#include "scene.hpp"
#include "texture.hpp"

#define M_PI 3.14159265359
#define M_PI_2 1.57079632679

#define playerX	pl->getX()
#define playerY	pl->getY()
#define OBJ		(it->second)
#define pAngle	pl->getAngle()

#define TEXTURE_SIZE 64*64*4

class	player;
class	scene;
class	obj;

struct	keyPressed;

constexpr int rayCnt = 480;

extern double rayStep;
extern double playerStep;
extern double playerSpeed;
extern double cosArr[rayCnt];
extern double FOV;
extern keyPressed keys;

class	raycast
{
	public:
		raycast();
		~raycast();

		const int	startGame(void);
		const int	checkValidity(void) const;
		const int	initGame(const char *);
		const int	mapParse(const char *);
		void		NewFunction(double &rAngle);
		void		playerInput(void);
		void		addBottonsToScene(void);
		void		renderScene(scene &);
		void		renderGame(const int &, const int &, const int &, const int &);
		void		renderMinimap(const int &, const int &, const int &, const int &);
		void		renderBotton(const int &, const int &, const int &, const int &, const std::string &);
		void		renderMapCreateToolField(const int &, const int &, const int &, const int &);
		void		newScene(void);
		int			loadCoinTexture(const char*);
		void		addObjectToScene(scene &, const int &, const int &, const int &, const int &, const std::string &);
		void		addObjectToScene(scene &, const int &, const int &, const int &, const int &, const int &,const std::string &);
		void		addObjectToScene(scene &, const int &, const int &, const int &, const int &, const int &, const std::string &, const std::string &);
		void		floodFill(const int, const int, std::vector<std::string> &) const;
		int			openTexture(const char *);
		int			loadRawTexture(const char, const char *);
		void		normalizeAngle(double &angle) const;
		void		drawTexture(int, int);

		void			drawBackground(int);
		void			determineTextureColor(unsigned char ***, char, double, double, int, int, int);
		void			determineTextureColor2(unsigned char ***, char, double, double, int, int, int);
		const double	getVerticalRay(double rayAngle, double &, double &);
		const double	getHorizontalRay(double rayAngle, double &, double &);
		const double	getShortestRay(double rayAngle, char &dir);

		bool collision(double &, double &);

		obj	*cursorOnObj(const double &cursorX, const double &cursorY) const;
		static void	window_size_callback(GLFWwindow*, int, int);

	private:
		int										asd = 0;
		scene									*currScene;
		player									*pl;
		GLFWwindow								*window;
		std::vector<scene>						*scenes;
		std::vector<std::string>				*map;
		std::vector<std::string>				*newMap;
		int										mapWidth, mapHeight;
		int										screenWidth, screenHeight;
		int										screenBuffWidth, screenBuffHeight;
		int										coinPosition;
		double									mX = 0;
		double									mY = 0;
		char									brush;
		double									cursorX, cursorY;
		std::vector<texture>					textures;
		std::map<const char, unsigned char ***>	allTextures;
		std::map<int, unsigned char ***>		coinTexture;
		std::map<double, std::pair<int, int>>					sprites;
};

void		key_callback(GLFWwindow*, int, int, int, int);
const int	inRange(const double, const double, const double);
void oASD();
std::vector<std::string> strSplit(const std::string &, const char);


#endif