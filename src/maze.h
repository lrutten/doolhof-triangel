#ifndef Maze_H
#define Maze_H

#include "point.h"
#include "wall.h"
#include "triangle.h"

#include <vector>
#include <map>
#include <algorithm>

namespace dt {

class WallMap
{
public:
   WallMap();
   ~WallMap();
   Wall *new_wall(Point *p1, Point *p2);
   void show();
	std::vector<Wall *> getWalls();

private:
   std::map<Point *, std::map<Point*, Wall *>> walls;
};

class Point;
class Wall;

class Maze
{
public:
	std::vector<Triangle *> _triangles;
	WallMap                 _walls;
	std::vector<Point *>    _vertices;
   bool                     found;  // is the solution found?
   Point                   *pc1;    // upper left
   Point                   *pc2;    // upper right
   Point                   *pc3;    // down  right
   Point                   *pc4;    // down  left
   Triangle                *start;  // start triangle
   Triangle                *stop;   // stop triangle
   std::vector<Triangle *> path;    // found path

public:

	Maze() = default;
	Maze(const Maze&) = delete;
	Maze(Maze&&) = delete;
   ~Maze();

	const std::vector<Triangle *>& triangulate(std::vector<Point *> &vertices);
	const std::vector<Triangle *>& getTriangles() const;
	std::vector<Wall *> getWalls();
	const std::vector<Point *>& getVertices() const;
	void setCorners(Point *ppc1, Point *ppc2, Point *ppc3, Point *ppc4);
	Triangle *findTriangle(Point *p);
   void connect();
   void show();
   void make();
   Triangle *getStop();
   void clearDrawn();
   void drawps(char *fn, bool withsol);

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
