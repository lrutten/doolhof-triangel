#include <random>
#include <set>

#include "triangle.h"

namespace dt {

Triangle::Triangle(Point *v1, Point *v2, Point *v3) :
	a(v1), b(v2), c(v3), isBad(false),
   visited(false), onpath(false), white(false)
{
}

bool Triangle::containsVertex(const Point *v) const
{
	return a == v || b == v || c == v;
}

std::pair<int, int> Triangle::getMiddle()
{
   int x = (a->x + b->x + c->x)/3;
   int y = (a->y + b->y + c->y)/3;
   return std::pair<int, int>(x, y);
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


// find the wall between this and the nb triangle
Wall *Triangle::getWall(Triangle *nb)
{
   std::cout << "getWall " << walls.size() << " " << nb->walls.size() << "\n";
   for (Wall *w1: walls)
   {
      for (Wall *w2: nb->walls)
      {
         std::cout << "w1 w2\n";
         if (w1 == w2)
         {
            std::cout << "neigh found\n";
            return w1;
         }
      }
   }
   std::cout << "neigh not found\n";
   return nullptr;
}

Wall *Triangle::findCornerWall()
{
   for (Wall *w: walls)
   {
      if (w->isBorder() && w->isCorner())
      {
         return w;
      }
   }
   return nullptr;
}

std::default_random_engine eng(std::random_device{}());
//std::uniform_real_distribution<double> dist_h(0, height);

bool Triangle::step(int d, Maze *mz)
{
   if (!visited)
   {
      std::cout << "visited d " << d << "nb " << neighbours.size() << "\n";
      visited = true;

      // add this triangle to the path
      if (!mz->found)
      {
         std::cout << "path push " << mz->path.size() << "\n";
         mz->path.push_back(this);
      }

      if (this == mz->getStop())
      {
         std::cout << "stop found\n";
         mz->found = true;
      }

      std::uniform_int_distribution<int> dist_nb(0, neighbours.size() - 1);
      std::set<int> visits;
      while (visits.size() < neighbours.size())
      {
         int j = dist_nb(eng);
         visits.insert(j);
         std::cout << "   visited nb " << j << " visits " << visits.size() << "\n";
         Triangle *nb = neighbours[j];
         if (!nb->visited)
         {
            bool r = nb->step(d + 1, mz);
            if (!mz->found && r)
            {
               mz->found = true;
            }

            // break the wall to this neighbour
            Wall *w = getWall(nb);
            if (w != nullptr)
            {
               std::cout << "open\n";
               w->open = true;
            }
         }
      }

      if (!mz->found)
      {
         // remove this triangle from path
         std::cout << "path pop " << mz->path.size() << "\n";
         mz->path.pop_back();
      }
   }
   return mz->found;
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


