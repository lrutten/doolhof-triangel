#include "wall.h"

namespace dt
{
Wall::Wall(Point *v1, Point *v2) :
	v(v1), w(v2), drawn(false), open(false), nr(counter++)
{
   // std::cout << "wall " << nr << "\n";
}

bool Wall::operator==(const Wall &e) const
{
	return (*(this->v) == *e.v && *(this->w) == *e.w) ||
			(*(this->v) == *e.w && *(this->w) == *e.v);
}

template<typename U>
std::ostream&
operator<<(std::ostream &str, const Wall &e)
{
   return str << "Wall " << *e.v << ", " << *e.w;
}

bool Wall::isBorder()
{
   return w->border && v->border;
}

bool Wall::isCorner()
{
   return w->corner || v->corner;
}

   bool almost_equal(const Wall *e1, const Wall *e2)
{
   return	(e1->v == e2->v && e1->w == e2->w) ||
            (e1->v == e2->w && e1->w == e2->v);
}

int Wall::counter = 0;

} // namespace dt


