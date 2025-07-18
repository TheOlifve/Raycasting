#include "player.hpp"
#define _USE_MATH_DEFINES 
#include <cmath> 

player::player(): pX(0), pY(0), angle(M_PI), hitbox(nullptr){ hitbox = new std::pair<double, double>[4]; };

player::~player(){ delete[] hitbox; };

void	player::calculateHitBoxPosition(void) {
	hitbox[0] = {pX - 0.1, pY - 0.1};
	hitbox[1] = {pX + 0.1, pY - 0.1};
	hitbox[2] = {pX + 0.1, pY + 0.1};
	hitbox[3] = {pX - 0.1, pY + 0.1};
}