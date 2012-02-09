// --simple.h--
// Simple structures and classes

#ifndef SIMPLE_H
#define SIMPLE_H

//x and y for the last mouseclick
struct Point2i
{
	int x;
	int y;

	Point2i()
	{
		this->x = 0;
		this->y = 0;
	}

	Point2i(int _x, int _y)
	{
		this->x = _x;
		this->y = _y;
	}

	bool operator==(Point2i &original)
	{
		return (this->x == original.x && this->y == original.y);
	}
};

// Point2f is used to represent points in 2-space
struct Point2f
{
	float x;
	float y;

	Point2f()
	{
		this->x = 0;
		this->y = 0;
	}

	Point2f(float new_x, float new_y)
	{
		this->x = new_x;
		this->y = new_y;
	}
};

#endif