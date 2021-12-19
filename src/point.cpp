#include "point.h"

namespace dt {

Point::Point(const double vx, const double vy) :
	x(vx), y(vy), border(false), corner(false)
{
}

double Point::getBorder()
{
   return border;
}

void Point::setBorder(bool b)
{
   border = b;
}

   double Point::dist2(const Point &v) const
{
	const double dx = x - v.x;
	const double dy = y - v.y;
	return dx * dx + dy * dy;
}

double Point::dist(const Point &v) const
{
	return hypot(x - v.x, y - v.y);
}

double Point::norm2() const
{
	return x * x + y * y;
}

void Point::show()
{
   std::cout << "   Point " << x << " " << y << " ";
   if (border)
   {
      std::cout << "b";
   }
   else
   {
      std::cout << "-";
   }
   if (corner)
   {
      std::cout << "c";
   }
   else
   {
      std::cout << "-";
   }
   std::cout << "\n";
}

bool Point::operator==(const Point &v) const
{
	return (this->x == v.x) && (this->y == v.y);
}

std::ostream &operator<<(std::ostream &str, const Point &v)
{
	return str << "Point x: " << v.x << " y: " << v.y;
}

bool almost_equal(const Point *v1, const Point *v2)
{
   return v1 == v2;
   //return almost_equal(v1->x, v2->x) && almost_equal(v1->y, v2->y);
}

} // namespace dt
