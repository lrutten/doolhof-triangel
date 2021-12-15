#include "point.h"

namespace dt {

template<typename T>
Point<T>::Point(const T vx, const T vy) :
	x(vx), y(vy)
{}

template<typename T>
T
Point<T>::dist2(const Point<T> &v) const
{
	const T dx = x - v.x;
	const T dy = y - v.y;
	return dx * dx + dy * dy;
}

template<>
float
Point<float>::dist(const Point<float> &v) const
{
	return hypotf(x - v.x, y - v.y);
}

template<>
double
Point<double>::dist(const Point<double> &v) const
{
	return hypot(x - v.x, y - v.y);
}

template<typename T>
T
Point<T>::norm2() const
{
	return x * x + y * y;
}

template<typename T>
bool
Point<T>::operator ==(const Point<T> &v) const
{
	return (this->x == v.x) && (this->y == v.y);
}

template<typename U>
std::ostream &
operator <<(std::ostream &str, const Point<U> &v)
{
	return str << "Point x: " << v.x << " y: " << v.y;
}

template struct Point<float>;
template struct Point<double>;

} // namespace dt
