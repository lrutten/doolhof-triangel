#include <map>

#include "maze.h"


/*
function BowyerWatson (pointList)
   // pointList is a set of coordinates defining the points to be triangulated
   triangulation := empty triangle mesh data structure
   add super-triangle to triangulation // must be large enough to completely contain all the points in pointList
   for each point in pointList do // add all the points one at a time to the triangulation
      badTriangles := empty set
      for each triangle in triangulation do // first find all the triangles that are no longer valid due to the insertion
         if point is inside circumcircle of triangle
            add triangle to badTriangles
      polygon := empty set
      for each triangle in badTriangles do // find the boundary of the polygonal hole
         for each edge in triangle do
            if edge is not shared by any other triangles in badTriangles
               add edge to polygon
         for each triangle in badTriangles do // remove them from the data structure
            remove triangle from triangulation
         for each edge in polygon do // re-triangulate the polygonal hole
            newTri := form a triangle from edge to point
            add newTri to triangulation
   for each triangle in triangulation // done inserting points, now clean up
      if triangle contains a vertex from original super-triangle
         remove triangle from triangulation
   return triangulation
*/

namespace dt
{

WallMap::WallMap()
{
}

WallMap::~WallMap()
{
   for (auto el: getWalls())
   {
      delete el;
   }
}

Wall *WallMap::new_wall(Point *p1, Point *p2)
{
   if (walls.find(p1) != walls.end() && walls[p1].find(p2) != walls[p1].end())
   {
      return walls[p1][p2];
   }
   else
   if (walls.find(p2) != walls.end() && walls[p2].find(p1) != walls[p2].end())
   {
      return walls[p2][p1];
   }
   else
   {
      Wall *w = new Wall(p1, p2);
      walls[p1][p2] = w;
      return w;
   }
}

void WallMap::show()
{
   std::cout << "walls\n";
   for (auto &el: walls)
   {
      for (auto & el2: el.second)
      {
         Wall *wl = el2.second;
         std::cout << "   walls\n";
         
         for (auto tr: wl->triangles)
         {
            std::cout << "      triangle\n";
         }
      }
   }
}

std::vector<Wall *> WallMap::getWalls()
{
   std::vector<Wall *> wls;
   for (auto &el: walls)
   {
      for (auto & el2: el.second)
      {
         std::cout << "wall in map\n";
         wls.push_back(el2.second);
      }
   }
   return wls;
}





const std::vector<Triangle *>&
Maze::triangulate(std::vector<Point *> &vertices)
{
	// Store the vertices locally
	_vertices = vertices;

	// Determinate the super triangle
   double minX = vertices[0]->x;
   double minY = vertices[0]->y;
   double maxX = minX;
   double maxY = minY;

	for (std::size_t i = 0; i < vertices.size(); ++i)
	{
		if (vertices[i]->x < minX) minX = vertices[i]->x;
		if (vertices[i]->y < minY) minY = vertices[i]->y;
		if (vertices[i]->x > maxX) maxX = vertices[i]->x;
		if (vertices[i]->y > maxY) maxY = vertices[i]->y;
	}

	const double dx = maxX - minX;
	const double dy = maxY - minY;
	const double deltaMax = std::max(dx, dy);
	const double midx = (minX + maxX) / 2;
	const double midy = (minY + maxY) / 2;

	// these are the points of the supertriangle
	Point *sp1 = new Point(midx - 20 * deltaMax, midy - deltaMax);
	Point *sp2 = new Point(midx, midy + 20 * deltaMax);
	Point *sp3 = new Point(midx + 20 * deltaMax, midy - deltaMax);

	// Create a list of triangles, and add the supertriangle in it
	_triangles.push_back(new Triangle(sp1, sp2, sp3));

	for (auto p = begin(vertices); p != end(vertices); p++)
	{
		std::vector<Wall *> polygon;

		// if the current point p fits inside the current triangle,
		// this triangle will become bad.
		for (auto &t : _triangles)
		{
			if (t->circumCircleContains(*p))
			{
			   // this is a bad triangle
				t->isBad = true;

				// keep the walls of the bad triangle
				polygon.push_back(new Wall(t->a, t->b));
				polygon.push_back(new Wall(t->b, t->c));
				polygon.push_back(new Wall(t->c, t->a));
			}
		}

		// remove all the bad triangles
      std::vector<Triangle *> delt;
		_triangles.erase(std::remove_if(begin(_triangles), end(_triangles),
      [&delt](Triangle *t)
		{
			bool c = t->isBad;
			if (c)
         {
			   delt.push_back(t);
         }
			return c;
		}), end(_triangles));
      for (auto tr: delt)
      {
         std::cout << "delete triangle a\n";
         delete tr;
      }

		// mark all the walls which are shared between 2 bad triangles as bad walls
		for (auto e1 = begin(polygon); e1 != end(polygon); ++e1)
		{
			for (auto e2 = e1 + 1; e2 != end(polygon); ++e2)
			{
				if (almost_equal(*e1, *e2))
				{
               (*e1)->isBad = true;
               (*e2)->isBad = true;
				}
			}
		}

		// remove all bad walls
      std::vector<Wall *> dele;
		polygon.erase(std::remove_if(begin(polygon), end(polygon), [&dele](Wall *e)
		{
			bool c = e->isBad;
			if (c)
         {
			   dele.push_back(e);
         }
			return c;
		}), end(polygon));
      for (auto ee: dele)
      {
         std::cout << "delete wall\n";
         delete ee;
      }

		// for each of the remaining walls
		//    make a new triangle the wall and the current point
		// don't keep the walls, only use the endpoints of each
		for (const auto e : polygon)
		{
         _triangles.push_back(new Triangle(e->v, e->w, *p));
         delete e;
      }
	}

	// sp1, sp2 and sp3 are the vertices of the super triangle
	// remove all the triangles which contain a vertex from the supertriangle
	std::vector<Triangle *> del;
	_triangles.erase(std::remove_if(begin(_triangles), end(_triangles), [sp1, sp2, sp3, &del](Triangle *t)
	{
	   bool c = t->containsVertex(sp1) || t->containsVertex(sp2) || t->containsVertex(sp3);
	   if (c)
      {
	      del.push_back(t);
      }
	   return c;
	}), end(_triangles));
   for (auto tr: del)
   {
      std::cout << "delete triangle b\n";
      delete tr;
   }
   
   // delete all the supertriangle vertices
   delete sp1;
   delete sp2;
   delete sp3;

   // make a list of all the walls
	for (const auto t : _triangles)
	{
	   // construct 3 walls a-b, b-c and c-a
      Wall *w1 = _walls.new_wall(t->a, t->b);
      Wall *w2 = _walls.new_wall(t->b, t->c);
      Wall *w3 = _walls.new_wall(t->c, t->a);

      // add triangle to each wall
      w1->addTriangle(t);
      w2->addTriangle(t);
      w3->addTriangle(t);

      // add the 3 walls to the triangle
      t->walls.push_back(w1);
      t->walls.push_back(w2);
      t->walls.push_back(w3);
	}
   // _walls.show();
   
   // connect the triangles as neighbours
   for (Wall *wl: _walls.getWalls())
   {
      if (wl->triangles.size() >= 2)
      {
         Triangle *tr0 = wl->triangles[0];
         Triangle *tr1 = wl->triangles[1];
         
         tr0->neighbours.push_back(tr1);
         tr1->neighbours.push_back(tr0);
      }
   }

	return _triangles;
}

const std::vector<Triangle *>&
Maze::getTriangles() const
{
	return _triangles;
}

std::vector<Wall *> Maze::getWalls()
{
	return _walls.getWalls();
}

Triangle *Maze::getStop()
{
   return stop;
}

const std::vector<Point *>&
Maze::getVertices() const
{
	return _vertices;
}

Maze::~Maze()
{
   std::cout << "~Maze()\n";
   for (auto t: _triangles)
   {
      delete t;
   }
   for (auto v: _vertices)
   {
      delete v;
   }
}

void Maze::show()
{
   std::cout << "----Maze-----\n";
   _walls.show();
   
   for (Triangle *tr: _triangles)
   {
      std::cout << "   triangle  neighb " << tr->neighbours.size() << "\n";
      std::cout << "      ";
      if (tr->a->border)
      {
         std::cout << "b";
      }
      else
      {
         std::cout << "-";
      }
      if (tr->a->corner)
      {
         std::cout << "c";
      }
      else
      {
         std::cout << "-";
      }
      if (tr->b->border)
      {
         std::cout << "b";
      }
      else
      {
         std::cout << "-";
      }
      if (tr->b->corner)
      {
         std::cout << "c";
      }
      else
      {
         std::cout << "-";
      }
      if (tr->c->border)
      {
         std::cout << "b";
      }
      else
      {
         std::cout << "-";
      }
      if (tr->c->corner)
      {
         std::cout << "c";
      }
      else
      {
         std::cout << "-";
      }
      std::cout << "\n";
   }
}

// find a triangle which has p as vertex
Triangle *Maze::findTriangle(Point *p)
{
   for (Triangle *tr: _triangles)
   {
      if (tr->containsVertex(p))
      {
         return tr;
      }
   }
   return nullptr;
}


void Maze::setCorners(Point *ppc1, Point *ppc2, Point *ppc3, Point *ppc4)
{
   pc1 = ppc1;
   pc2 = ppc2;
   pc3 = ppc3;
   pc4 = ppc4;
   
   Triangle *ul = findTriangle(pc1);
   Triangle *ur = findTriangle(pc2);
   Triangle *br = findTriangle(pc3);
   Triangle *bl = findTriangle(pc4);
   
   // set stop
   if (ul != nullptr && ul->neighbours.size() == 1)
   {
      stop = ul;
   }
   else
   if (ur != nullptr && ur->neighbours.size() == 1)
   {
      stop = ur;
   }
   else
   {
      stop = ul;
   }
   
   // set start
   if (br != nullptr && br->neighbours.size() == 1)
   {
      start = br;
   }
   else
   if (bl != nullptr && bl->neighbours.size() == 1)
   {
      start = bl;
   }
   else
   {
      start = br;
   }

   // open the start wall
   if (start != nullptr)
   {
      Wall *wsta = start->findCornerWall();
      if (wsta != nullptr)
      {
         wsta->open = true;
      }
      else
      {
         std::cout << "start wall not found\n";
      }
   }

   // open the stop wall
   if (stop != nullptr)
   {
      Wall *wsto = stop->findCornerWall();
      if (wsto != nullptr)
      {
         wsto->open = true;
      }
      else
      {
         std::cout << "stop wall not found\n";
      }
   }
}


void Maze::make()
{
   if (start != nullptr)
   {
      found = false;
      bool r = start->step(0, this);
      std::cout << "Maze make end path " << path.size() << "\n";

      for (Triangle *tr: path)
      {
         tr->onpath = true;
      }
   }
}





// Markeer alle muren als niet getekend.
void Maze::clearDrawn()
{
   for (Wall *w: _walls.getWalls())
   {
      w->drawn = false;
   }
}


// Schrijf het ps bestand.
void Maze::drawps(char *fn, bool metpad)
{
   clearDrawn();

   FILE *fp = fopen(fn,"w");
   if (fp != NULL)
   {
      fprintf(fp, "%!PS\n");
      fprintf(fp, "\n");
      //fprintf(fp, "matrix currentmatrix /originmat exch def\n");
      //fprintf(fp, "/umatrix {originmat matrix concatmatrix setmatrix} def\n");
      //fprintf(fp, "[28.3465 0 0 28.3465 10.5 100.0] umatrix\n");

      fprintf(fp, "\n");
      //fprintf(fp, "0 setgray\n");

      // Maten A4 in Postscript 595 x 842

      const double zijde     = 10;
      const double linewidth = 0.5;
      const double box        = 12.0; // was 12.0
      const double boy        = 22.0; // was 12.0

      // Draw all the filled polygons of the triangles
      int itrf = 0;
      for (Triangle *tr: _triangles)
      {
         fprintf(fp, "%% triangle %d fill\n", itrf++);

         double ax = box + tr->a->x;
         double ay = boy + tr->a->y;
         double bx = box + tr->b->x;
         double by = boy + tr->b->y;
         double cx = box + tr->c->x;
         double cy = boy + tr->c->y;

         if (metpad && tr->onpath)
         {
            switch (itrf%2)
            {
               case 0:
                  fprintf(fp, "0.9 0.75 0.75 setrgbcolor\n");
                  break;
               case 1:
                  fprintf(fp, "0.8 0.65 0.65 setrgbcolor\n");
                  break;
            }
         }
         else
         {
            switch (itrf%4)
            {
               case 0:
                  fprintf(fp, "0.8 0.8 0.8 setrgbcolor\n");
                  break;
               case 1:
                  fprintf(fp, "0.8 0.8 0.9 setrgbcolor\n");
                  break;
               case 2:
                  fprintf(fp, "0.8 0.9 0.8 setrgbcolor\n");
                  break;
               case 3:
                  fprintf(fp, "0.8 0.9 0.9 setrgbcolor\n");
                  break;
            }
         }
         fprintf(fp, "newpath\n");
         fprintf(fp, "%lf %lf moveto\n", ax, ay);
         fprintf(fp, "%lf %lf lineto\n", bx, by);
         fprintf(fp, "%lf %lf lineto\n", cx, cy);
         fprintf(fp, "fill\n");
         fprintf(fp, "\n");
      }


      // Draw all the walls of the triangles
      fprintf(fp, "0.0 0.0 0.0 setrgbcolor\n");
      int itr = 0;
      for (Triangle *tr: _triangles)
      {
         fprintf(fp, "%% triangle %d walls\n", itr++);

         int iw = 0;
         for (Wall *wl: tr->walls)
         {
            if (!wl->open)
            {
               double vx = box + wl->v->x;
               double vy = boy + wl->v->y;
               double wx = box + wl->w->x;
               double wy = boy + wl->w->y;

               fprintf(fp, "%% wall %d\n", iw++);
               fprintf(fp, "%lf %lf newpath moveto\n", vx, vy);
               fprintf(fp, "%lf %lf lineto\n", wx, wy);
               fprintf(fp, "stroke\n");
               fprintf(fp, "\n");
            }
         }
      }

      // draw all the lines of the solution
      double x1;
      double y1;
      int iso = 0;
      bool first = true;
      if (metpad)
      {
         for (dt::Triangle *tr: path) {
            std::pair<int, int> pr = tr->getMiddle();
            double x2 = box + pr.first;
            double y2 = boy + pr.second;

            if (first) {
               first = false;
            } else {
               fprintf(fp, "%% solution line %d\n", iso++);

               fprintf(fp, "0 255 0 %lf setrgbcolor\n");
               fprintf(fp, "%lf %lf newpath moveto\n", x1, y1);
               fprintf(fp, "%lf %lf lineto\n", x2, y2);
               fprintf(fp, "stroke\n");
               fprintf(fp, "\n");
            }

            x1 = x2;
            y1 = y2;
         }
      }

      /*
      // ingang
      double x1 = bx + zijde * (0);
      double y1 = by + zijde * (-1);
      double x2 = x1 + zijde;
      double y2 = y1;
      double x3 = x1 + zijde;
      double y3 = y1 + zijde;
      double x4 = x1;
      double y4 = y1 + zijde;

      fprintf(fp, "%lf setlinewidth\n", linewidth);
      fprintf(fp, "%lf %lf newpath moveto\n", x1, y1);
      fprintf(fp, "%lf %lf moveto\n", x2, y2);
      fprintf(fp, "%lf %lf lineto\n", x3, y3);
      fprintf(fp, "%lf %lf moveto\n", x4, y4);
      fprintf(fp, "%lf %lf lineto\n", x1, y1);
      fprintf(fp, "0 setgray\n");
      fprintf(fp, "%lf setlinewidth\n", linewidth);
      fprintf(fp, "stroke\n");

      
      // uitgang
      x1 = bx + zijde * (br-1);
      y1 = by + zijde * ho;
      x2 = x1 + zijde;
      y2 = y1;
      x3 = x1 + zijde;
      y3 = y1 + zijde;
      x4 = x1;
      y4 = y1 + zijde;

      fprintf(fp, "%lf setlinewidth\n", linewidth);
      fprintf(fp, "%lf %lf newpath moveto\n", x1, y1);
      fprintf(fp, "%lf %lf moveto\n", x2, y2);
      fprintf(fp, "%lf %lf lineto\n", x3, y3);
      fprintf(fp, "%lf %lf moveto\n", x4, y4);
      fprintf(fp, "%lf %lf lineto\n", x1, y1);
      fprintf(fp, "0 setgray\n");
      fprintf(fp, "%lf setlinewidth\n", linewidth);
      fprintf(fp, "stroke\n");
       */

      fprintf(fp, "showpage\n");

      fclose(fp);
   }
}

} // namespace dt
