#ifndef Point_H
#define Point_H

#include "numeric.h"

#include <iostream>
#include <cmath>
#include <type_traits>

namespace dt {

class Point
{
public:
	Point() = default;
	Point(const Point &v) = default;
	Point(Point &&) = default;
	Point(const double vx, const double vy);

	double getBorder();
   void setBorder(bool b);
	double dist2(const Point &v) const;
	double dist(const Point &v) const;
	double norm2() const;
	void show();

	Point &operator=(const Point &) = default;
	Point &operator=(Point &&) = default;
	bool operator ==(const Point &v) const;
	friend std::ostream &operator <<(std::ostream &str, const Point &v);

	double x;
	double y;
   bool corner;

private:
   bool border;
};


} // namespace dt

#endif
