#ifndef WALL_H
#define WALL_H

#include <vector>
#include "point.h"

namespace dt
{
class Triangle;

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
   void show();

	friend std::ostream &operator <<(std::ostream &str, const Wall &e);

	Point *v;
	Point *w;
	std::vector<Triangle *> triangles;
	bool              isBad = false;
   bool              open;      // is the wall open
   bool              drawn;  // is the muur drawn
   int               nr;
   static int        counter;

   bool isOpen()
   {
      return open;
   }
   bool isBorder();
   bool isCorner();

   void addTriangle(Triangle *tr)
   {
      triangles.push_back(tr);
   }
};

bool almost_equal(const Wall *e1, const Wall *e2);

} // namespace dt

#endif
