#ifndef WALL_H
#define WALL_H

#include "point.h"

namespace dt
{

class Wall
{
public:
	Wall() = default;
	Wall(const Wall&) = default;
	Wall(Wall&&) = default;
	Wall(Point *v1, Point *v2);

	Wall &operator=(const Wall&) = default;
	Wall &operator=(Wall&&) = default;
	bool operator ==(const Wall &e) const;

	friend std::ostream &operator <<(std::ostream &str, const Wall &e);

	Point *v;
	Point *w;
	bool              isBad = false;
   bool              open;      // is the wall open
   bool              drawn;  // is the muur drawn
   int               nr;
   static int        counter;

   bool isOpen()
   {
      return open;
   }

	static_assert(std::is_floating_point<double>::value,
		"Type must be floating-point");
};

bool almost_equal(const Wall *e1, const Wall *e2);

} // namespace dt

#endif
