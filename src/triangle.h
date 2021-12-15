#ifndef H_TRIANGLE_H
#define H_TRIANGLE_H

#include <vector>

#include "numeric.h"
#include "point.h"
#include "wall.h"

namespace dt {

class Triangle
{
public:
	Triangle() = default;
	Triangle(const Triangle&) = default;
	Triangle(Triangle&&) = default;
	Triangle(Point *v1, Point *v2, Point *v3);

	bool containsVertex(const Point *v) const;
	bool circumCircleContains(const Point *v) const;

	Triangle &operator=(const Triangle&) = default;
	Triangle &operator=(Triangle&&) = default;
	bool operator ==(const Triangle &t) const;

	friend std::ostream &operator <<(std::ostream &str, const Triangle &t);

	Point *a;
	Point *b;
	Point *c;
	std::vector<Wall *>     walls;
   std::vector<Triangle *> neighbours;
	bool              isBad = false;
   bool              visited; // is already visited
   bool              onpad;   // this triangle is part of the path
   bool              white;   // this triangle must be avoided

   void setwhite()
   {
      white   = true;
      visited = true;
   }

	static_assert(std::is_floating_point<double>::value,
		"Type must be floating-point");
};


} // namespace dt


/*
  Deze klasse stelt een cel voor.
 */
/*
class Cel
{
   friend class Veld;
   friend class Doolhof;

private:
   bool bezocht; // is reeds bezocht
   bool pad;     // deze cel is deel van het pad
   bool wit;     // deze cel moet vremeden worden
   Muur *boven;
   Muur *rechts;
   Muur *onder;
   Muur *links;

public:
   Cel();
   void zetwit()
   {
      wit     = true;
      bezocht = true;
   }
};
 */


#endif
