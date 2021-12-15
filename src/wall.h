#ifndef WALL_H
#define WALL_H

#include "point.h"

namespace dt
{

template<typename T>
class Wall
{
public:
	using Type = T;
	using VertexType = Point<Type>;

	Wall() = default;
	Wall(const Wall&) = default;
	Wall(Wall&&) = default;
	Wall(const VertexType *v1, const VertexType *v2);

	Wall &operator=(const Wall&) = default;
	Wall &operator=(Wall&&) = default;
	bool operator ==(const Wall &e) const;

	template<typename U>
	friend std::ostream &operator <<(std::ostream &str, const Wall<U> &e);

	const VertexType *v;
	const VertexType *w;
	bool              isBad = false;
   bool              open;      // is the wall open
   bool              drawn;  // is the muur drawn
   int               nr;
   static int        counter;

   bool isOpen()
   {
      return open;
   }

	static_assert(std::is_floating_point<Wall<T>::Type>::value,
		"Type must be floating-point");
};

template<typename T>
bool
almost_equal(const Wall<T> *e1, const Wall<T> *e2)
{
	return	(almost_equal(e1->v, e2->v) && almost_equal(e1->w, e2->w)) ||
			(almost_equal(e1->v, e2->w) && almost_equal(e1->w, e2->v));
}

} // namespace dt

#endif
