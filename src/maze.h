#ifndef H_Maze
#define H_Maze

#include "point.h"
#include "wall.h"
#include "triangle.h"

#include <vector>
#include <algorithm>

namespace dt {

template<typename T>
class Maze
{
public:
	using Type         = T;
	using VertexType   = Point<Type>;
	using WallType     = Wall<Type>;
	using TriangleType = Triangle<Type>;

	static_assert(std::is_floating_point<Maze<T>::Type>::value,
		"Type must be floating-point");

	std::vector<TriangleType *> _triangles;
	std::vector<WallType *>     _Walls;
	std::vector<VertexType *>   _vertices;
   bool                      found;  // is the solution found?

public:

	Maze() = default;
	Maze(const Maze&) = delete;
	Maze(Maze&&) = delete;

	const std::vector<TriangleType *>& triangulate(std::vector<VertexType *> &vertices);
	const std::vector<TriangleType *>& getTriangles() const;
	const std::vector<WallType *>& getWalls() const;
	const std::vector<VertexType *>& getVertices() const;
   void connect();

	Maze& operator=(const Maze&) = delete;
	Maze& operator=(Maze&&) = delete;
};

} // namespace dt


// was
//   59
//   77
// nu
//     57
//     77

/*
  Een doolhof bestaat uit rechthoekig gebied
  bestaande uit cellen.
 */

/*
class Doolhof
{
public:
   static const int br = 57; // breedte
   static const int ho = 77; // hoogte

private:
   Cel *cellen[ho][br]; // het raster van cellen

   int xx[ho*br];  // array voor de x van het pad
   int yy[ho*br];  // array voor de y van het pad
   int diepte;

public:
   Doolhof();
   ~Doolhof();
   Cel *getCel(int x, int y);
   void zetpad(int d, int x, int y);
   void stap(int d, int x, int y);
   void maak();
   void toon();
   void toonmetrand();
   void toonmetenkelerand();
   void toonmetmuren();
   void tekenps(const char *fn, bool metpad);
   void zetwit(int x, int y);
   void cleargetekend();
};
 */
#endif
