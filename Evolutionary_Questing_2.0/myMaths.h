#pragma once


class vector2D
{
public:

	vector2D()
		: x_pos(0), y_pos(0)
	{}

	vector2D(float x, float y)
		: x_pos(x), y_pos(y)
	{}

	vector2D(const vector2D &copyFrom)
		: x_pos(copyFrom.x_pos), y_pos(copyFrom.y_pos)
	{
	
	}

	float getX() { return x_pos; }
	float getY() { return y_pos; }


	float getDistance(const vector2D & other)
	{
		return sqrt((x_pos - other.x_pos) * (x_pos - other.x_pos) + (y_pos - other.y_pos) * (other.y_pos - y_pos));
	}


private:

	float x_pos;
	float y_pos;
};