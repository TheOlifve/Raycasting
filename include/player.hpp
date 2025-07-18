#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raycast.hpp"
#include <vector>

#define M_PI 3.14159265359
#define M_PI_2 1.57079632679

#define rotationSpeed 0.05

struct keyPressed {
	bool	esc = false;
	bool	moveUp = false;
	bool	moveDown = false;
	bool	moveLeft = false;
	bool	moveRight = false;
	bool	openMap = false;
	bool	rotateLeft = false;
	bool	rotateRight = false;
	bool	run = false;
	bool	use = false;
};

class player
{
	public:
		player();
		~player();

		double	getX(void) const { return pX; }
		void	setX(double X) { pX += X; }
		double	getY(void) const {return pY; }
		void	setY(double Y) { pY += Y; }
		double	getAngle(void) const { return angle; }
		void	setAngle(double angle)
		{
			this->angle += angle;
			if (this->angle < 0)
			{
				this->angle = 2 * M_PI;
			}
			if (this->angle > 2 * M_PI)
			{
				this->angle = 0;
			}
		}
		std::pair<double, double> *getHitBox(void) const { return hitbox;}
		void					calculateHitBoxPosition(void);
	private:
		double	pX;
		double	pY;
		std::pair<double, double>	*hitbox;
		double angle;
};

#endif