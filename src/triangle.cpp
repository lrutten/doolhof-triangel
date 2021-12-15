#include "triangle.h"

namespace dt {

Triangle::Triangle(Point *v1, Point *v2, Point *v3) :
	a(v1), b(v2), c(v3), isBad(false),
   visited(false), onpad(false), white(false)
{
}

bool Triangle::containsVertex(const Point *v) const
{
	return a == v || b == v || c == v;
}

bool Triangle::circumCircleContains(const Point *v) const
{
	const double ab = a->norm2();
	const double cd = b->norm2();
	const double ef = c->norm2();

	const double ax = a->x;
	const double ay = a->y;
	const double bx = b->x;
	const double by = b->y;
	const double cx = c->x;
	const double cy = c->y;

	const double circum_x = (ab * (cy - by) + cd * (ay - cy) + ef * (by - ay)) / (ax * (cy - by) + bx * (ay - cy) + cx * (by - ay));
	const double circum_y = (ab * (cx - bx) + cd * (ax - cx) + ef * (bx - ax)) / (ay * (cx - bx) + by * (ax - cx) + cy * (bx - ax));

	const Point circum(circum_x / 2, circum_y / 2);
	const double circum_radius = a->dist2(circum);
	const double dist = v->dist2(circum);
	return dist <= circum_radius;
}


void Triangle::step(int d)
{
   if (!visited)
   {
      std::cout << "visited " << d << "\n";
      visited = true;

      for (Triangle *nb: neighbours)
      {
         nb->step(d + 1);
      }
   }
}

bool Triangle::operator ==(const Triangle &t) const
{
	return	(*this->a == *t.a || *this->a == *t.b || *this->a == *t.c) &&
			(*this->b == *t.a || *this->b == *t.b || *this->b == *t.c) &&
			(*this->c == *t.a || *this->c == *t.b || *this->c == *t.c);
}

std::ostream&
operator <<(std::ostream &str, const Triangle &t)
{
	return str << "Triangle:" << "\n\t" <<
			*t.a << "\n\t" <<
			*t.b << "\n\t" <<
			*t.c << '\n';
}

bool almost_equal(const Triangle &t1, const Triangle &t2)
{
   return	(t1.a == t2.a || t1.a == t2.b || t1.a == t2.c) &&
            (t1.b == t2.a || t1.b == t2.b || t1.b == t2.c) &&
            (t1.c == t2.a || t1.c == t2.b || t1.c == t2.c);
}

} // namespace dt


