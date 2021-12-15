#ifndef H_Point
#define H_Point

#include "numeric.h"

#include <iostream>
#include <cmath>
#include <type_traits>

namespace dt {

template<typename T>
class Point
{
public:
	using Type = T;
	Point() = default;
	Point(const Point<T> &v) = default;
	Point(Point<T>&&) = default;
	Point(const T vx, const T vy);

	T dist2(const Point<T> &v) const;
	T dist(const Point<T> &v) const;
	T norm2() const;

	Point &operator=(const Point<T>&) = default;
	Point &operator=(Point&&) = default;
	bool operator ==(const Point<T> &v) const;
	template<typename U>
	friend std::ostream &operator <<(std::ostream &str, const Point<U> &v);

	T x;
	T y;
   bool corner;
   bool border;

	static_assert(std::is_floating_point<Point<T>::Type>::value,
		"Type must be floating-point");
};

template<typename T>
bool almost_equal(const Point<T> *v1, const Point<T> *v2)
{
   return v1 == v2;
	//return almost_equal(v1->x, v2->x) && almost_equal(v1->y, v2->y);
}

} // namespace dt

#endif
