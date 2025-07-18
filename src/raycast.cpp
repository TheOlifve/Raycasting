#include "raycast.hpp"

double playerStep = 0.025;
double playerSpeed = playerStep;
double cosArr[rayCnt];
double FOV = M_PI / 3;
double rayStep = FOV / rayCnt;

const int TARGET_FPS = 90;
const int FRAME_DURATION = 1000 / TARGET_FPS;

raycast::raycast() {
	window = nullptr;
	map = nullptr;
	newMap = nullptr;
	pl = nullptr;
	scenes = nullptr;
	mapWidth = 0;
	mapHeight = 0;
	screenWidth = 0;
	screenHeight = 0;
	brush = '0';
	coinPosition = 0;
	//std::cout << "[ Raycast default constructor called ]" << std::endl;
}

raycast::~raycast() {
	if (map)
		delete map;
	if (pl)
		delete pl;
	if (newMap)
		delete newMap;
	if (scenes)
		delete scenes;
	for (auto it = allTextures.begin(); it != allTextures.end(); ++it) {
		for (int i = 0; i < 64; ++i) {
			for (int j = 0; j < 64; ++j) {
				delete[] it->second[i][j];
			}
			delete[] it->second[i];
		}
		delete[] it->second;
	}
	for (auto it = coinTexture.begin(); it != coinTexture.end(); ++it) {
		for (int i = 0; i < 64; ++i) {
			for (int j = 0; j < 64; ++j) {
				delete[] it->second[i][j];
			}
			delete[] it->second[i];
		}
		delete[] it->second;
	}
	glfwTerminate();
	//std::cout << "[ Raycast destructor called ]" << std::endl;
}

void	raycast::addBottonsToScene(void) {
	std::map<std::string, obj>	&tmp = (*scenes)[1].getObjs();

	auto it = tmp.find("mapCreate");
	int tileWidth = it->second.getTileWidth();
	int tileHegiht = it->second.getTileHeight();
	int x = it->second.getX1();
	int y = it->second.getY1() - tileHegiht;

	addObjectToScene((*scenes)[1], x, y, tileWidth, tileHegiht, 1, "buttonBrush1", "gray");
	addObjectToScene((*scenes)[1], x + tileWidth, y, tileWidth, tileHegiht, 1, "buttonBrush0", "white");
	addObjectToScene((*scenes)[1], x + tileWidth * 2, y, tileWidth, tileHegiht, 1, "buttonBrushP", "red");
}

const int raycast::initGame(const char *filename) {
	pl = new player();
	if (mapParse(filename)) {
		std::cerr << "Failed to parse map" << std::endl;
		return 1;
	}
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return 1;
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	screenWidth = mode->width;
	screenHeight = mode->height;
	window = glfwCreateWindow(screenWidth, screenHeight, "RayCasting", glfwGetPrimaryMonitor() , NULL);
	glfwGetFramebufferSize(window, &screenBuffWidth, &screenBuffHeight);
	std::cout << "Screen: " << screenWidth << ":" << screenHeight << std::endl;

	int	size = std::min(screenBuffWidth, screenBuffHeight);
	scenes = new std::vector<scene>();


	newScene();
	addObjectToScene((*scenes)[0], 0, 0, screenBuffWidth, screenBuffHeight, "MainGame");
	addObjectToScene((*scenes)[0], 0, 0, size / 4, size / 4, "minimap");


	newScene();
	addObjectToScene((*scenes)[1], (screenBuffWidth - size / 1.2) / 2,\
	(screenBuffHeight - size / 1.2) / 2, size / 1.2, size / 1.2, 32, "mapCreate");
	addBottonsToScene();

	double angle = pAngle - FOV / 2;
	for (int i = 0; i < rayCnt; ++i, angle += rayStep)
		cosArr[i] = cos(pAngle - angle) * 0.55;
	return 0;
}

void	raycast::newScene(void) {
	scenes->emplace_back();
}

const int	raycast::startGame(void) {
	glfwSetWindowUserPointer(window, this);
	if (!window) {
		glfwTerminate();
		std::cerr << "Failed to create GLFW window" << std::endl;
		return 1;
	}
	currScene = &(*scenes)[0];
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	
	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	
	
	while (!glfwWindowShouldClose(window)) 
	{
		auto frameStart = std::chrono::high_resolution_clock::now();

		start = std::chrono::high_resolution_clock::now();
		glClear(GL_COLOR_BUFFER_BIT);
		playerInput();
		renderScene(*currScene);
		glfwSwapBuffers(window);
		end = std::chrono::high_resolution_clock::now();

		glfwPollEvents();
		auto frameEnd = std::chrono::high_resolution_clock::now();
    	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();
    	if (elapsed < FRAME_DURATION) 
		{
        	std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_DURATION - elapsed));
    	}
    frameStart = std::chrono::high_resolution_clock::now();

	}
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
	glfwTerminate();
}

const double raycast::getVerticalRay(double rayAngle, double &_mX, double &_mY)
{
	double vertLengthX = 0;
	double vertLengthY = 0;
	double offsetY = tan(rayAngle);
	double distV = 100;

	normalizeAngle(rayAngle);
	
	if ((rayAngle >= 0 && rayAngle < M_PI_2) || (rayAngle > 3 * M_PI_2 && rayAngle <= 2 * M_PI))			//right
		vertLengthX = 1 - (playerX - (int)playerX);
	else if (rayAngle > M_PI_2 && rayAngle < 3 * M_PI_2)													//left
		vertLengthX = -(playerX - (int)playerX);

	vertLengthY = tan(rayAngle) * vertLengthX;

	double mXV = playerX + vertLengthX;
	double mYV = playerY + vertLengthY;

	while (mXV >= 0 && mXV < mapWidth && mYV >= 0 && mYV < mapHeight) 
	{
		if ((rayAngle >= 0 && rayAngle <= M_PI_2) || (rayAngle > 3 * M_PI_2 && rayAngle <= 2 * M_PI))
		{
			if (((*map)[mYV][mXV] >= '1' && (*map)[mYV][mXV] <= '9') || ((*map)[mYV][mXV] >= 'A' && (*map)[mYV][mXV] <= 'Z') || (*map)[mYV][mXV] == 'd') 
			{
				_mX = mXV;
				distV = sqrt((mXV - playerX) * (mXV - playerX) + (mYV-playerY) * (mYV-playerY));
				break;
			}
			if ((*map)[mYV][mXV] == 'c') {
				sprites[sqrt((mXV - playerX) * (mXV - playerX) + (mYV - playerY) * (mYV - playerY))].first = (mYV - (int)mYV) * 64;
			}
			mYV += offsetY;
			++mXV;
		}
		else
		{
			if (((*map)[(int)mYV][(int)mXV-1] >= '1' && (*map)[(int)mYV][(int)mXV-1] <= '9') || ((*map)[mYV][mXV-1] >= 'A' && (*map)[mYV][mXV-1] <= 'Z') || (*map)[mYV][mXV-1] == 'd') 
			{
				_mX = mXV - 1;
				distV = sqrt((mXV-playerX)*(mXV-playerX) + (mYV-playerY)*(mYV-playerY));
				break;
			}
			if ((*map)[mYV][mXV - 1] == 'c') {
				sprites[sqrt((mXV-playerX)*(mXV-playerX) + (mYV-playerY)*(mYV-playerY))].first = (mYV - (int)mYV) * 64;
			}
			--mXV;
			mYV -= offsetY;
		}
		
	}
	_mY = mYV;
	return distV;
}

const double  raycast::getHorizontalRay(double rayAngle, double &_mX, double &_mY)
{
	double horLengthX = 0;
	double horLengthY = 0;
	double offsetX = 1/tan(rayAngle);
	double distH = 100;
	
	normalizeAngle(rayAngle);

	if(rayAngle < M_PI)
		horLengthY = 1 - (playerY - (int)playerY);
	else if (rayAngle > M_PI)
		horLengthY = - (playerY - (int)playerY);

	horLengthX = horLengthY/tan(rayAngle);

	double mXH = playerX + horLengthX;
	double mYH = playerY + horLengthY;
	
	while (mXH >= 0 && mXH < mapWidth && mYH >= 0 && mYH < mapHeight)
	{
		if(rayAngle < M_PI)
		{
			if (((*map)[(int)mYH][(int)mXH] >= '1' && (*map)[(int)mYH][(int)mXH] <= '9') || ((*map)[(int)mYH][(int)mXH] >= 'A' && (*map)[(int)mYH][(int)mXH] <= 'Z') || (*map)[(int)mYH][(int)mXH] == 'd')
			{
				_mY = mYH;
				distH = sqrt((mXH-playerX)*(mXH-playerX) + (mYH-playerY)*(mYH-playerY));
				break;
			}
			if ((*map)[(int)mYH][(int)mXH] == 'c') {
				// sprites[sqrt((mXH-playerX)*(mXH-playerX) + (mYH-playerY)*(mYH-playerY))].second = (mXH - (int)mXH) * 64;
			}
			++mYH;
			mXH += offsetX;
		}
		else if (rayAngle > M_PI)
		{
			if (((*map)[(int)mYH - 1][(int)mXH] >= '1' && (*map)[(int)mYH - 1][(int)mXH] <= '9') || ((*map)[(int)mYH - 1][(int)mXH] >= 'A' && (*map)[(int)mYH - 1][(int)mXH] <= 'Z') || (*map)[(int)mYH - 1][(int)mXH] == 'd') 
			{
				_mY = mYH - 1;
				distH = sqrt((mXH-playerX)*(mXH-playerX) + (mYH-playerY)*(mYH-playerY));
				break;
			}
			if ((*map)[(int)mYH - 1][(int)mXH] == 'c') {
				// sprites[sqrt((mXH-playerX)*(mXH-playerX) + (mYH-playerY)*(mYH-playerY))].second = (mXH - (int)mXH) * 64;
			}
			--mYH;
			mXH -= offsetX;
		}
	}
	_mX = mXH;
	return distH;
}

const double raycast::getShortestRay(double rayAngle, char &dir)
{
	
	double	dist = 0;
	double	xV, yV, xH, yH;
	double	distV = getVerticalRay(rayAngle, xV, yV);
	double	distH = getHorizontalRay(rayAngle, xH, yH);

	if (distV <= distH) 
	{
		dist = distV;
		dir = 'v';
		mX = xV;
		mY = yV;
	} 
	else 
	{
		dist = distH;
		dir = 'h';
		mX = xH;
		mY = yH;
	}

	return dist;
}

void raycast::normalizeAngle(double &angle) const
{
	if (angle > 2 * M_PI)
		angle = angle - 360 * 0.0174533;
	if (angle < 0)
		angle += 2 * M_PI;
}