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
      Wall *w1 = _walls.new_wall(t->a, t->b);
      Wall *w2 = _walls.new_wall(t->b, t->c);
      Wall *w3 = _walls.new_wall(t->c, t->a);
      
      w1->addTriangle(t);
      w2->addTriangle(t);
      w3->addTriangle(t);
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
   Triangle *dr = findTriangle(pc3);
   Triangle *dl = findTriangle(pc4);
   
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
   if (dr != nullptr && dr->neighbours.size() == 1)
   {
      start = dr;
   }
   else
   if (dl != nullptr && dl->neighbours.size() == 1)
   {
      start = dl;
   }
   else
   {
      start = dr;
   }
}



void Maze::connect()
{
   for (const dt::Wall *wa: _walls.getWalls())
   {
      std::cout << "Wall " << wa->nr << "\n";
      if (wa->v->border && wa->w->border)
      {
         std::cout << "   border wall\n";
      }
      if (wa->v->corner || wa->w->corner)
      {
         std::cout << "   corner wall\n";
      }
      if (wa->isBad)
      {
         std::cout << "   isbad\n";
      }
   }
}


void Maze::make()
{
   if (start != nullptr)
   {
      start->step(0);
   }
}

} // namespace dt


/*


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>




// Maak een doolhof.
Doolhof::Doolhof()
{
   gevonden = false;

   // maak eerst alle cellen
   for (int ih = 0; ih<ho; ih++)
   {
      for (int ib = 0; ib<br; ib++)
      {
         cellen[ih][ib] = new Cel();
      }
   }

   // maak alle muren tussen de cellen
   for (int iy = 0; iy<ho; iy++)
   {
      for (int ix = 0; ix<br; ix++)
      {
         Cel *c = cellen[iy][ix];
         Cel *cbo = getCel(ix,   iy-1);
         Cel *cre = getCel(ix+1, iy);
         Cel *con = getCel(ix,   iy+1);
         Cel *cli = getCel(ix-1, iy);


         // boven ok
         if (c->boven == NULL || cbo != NULL && cbo->onder == NULL)
         {
            Muur *m = new Muur();
            if (c->boven == NULL)
            {
               c->boven = m;
            }
            if (cbo != NULL && cbo->onder == NULL)
            {
               cbo->onder = m;
            }
         }

         // rechts ok
         if (c->rechts == NULL || cre != NULL && cre->links == NULL)
         {
            Muur *m = new Muur();
            if (c->rechts == NULL)
            {
               c->rechts = m;
            }
            if (cre != NULL && cre->links == NULL)
            {
               cre->links = m;
            }
         }

         // onder ok
         if (c->onder == NULL || con != NULL && con->boven == NULL)
         {
            Muur *m = new Muur();
            if (c->onder == NULL)
            {
               c->onder = m;
            }
            if (con != NULL && con->boven == NULL)
            {
               con->boven = m;
            }
         }
         // links
         if (c->links == NULL || cli != NULL && cli->rechts == NULL)
         {
            Muur *m = new Muur();
            if (c->links == NULL)
            {
               c->links = m;
            }
            if (cli != NULL && cli->rechts == NULL)
            {
               cli->rechts = m;
            }
         }
      }
   }

   cellen[0][0]->boven->open = true;
   cellen[ho-1][br-1]->onder->open = true;
}

// Breek een doolhof af.
Doolhof::~Doolhof()
{
   for (int ih = 0; ih<ho; ih++)
   {
      for (int ib = 0; ib<br; ib++)
      {
         delete cellen[ih][ib];
      }
   }
}

// Markeer een cel als wit
void Doolhof::zetwit(int x, int y)
{
   Cel *c = getCel(x, y);
   if (c != NULL)
   {
      c->zetwit();
   }
}

// Voeg een positie bij aan het pad
void Doolhof::zetpad(int d, int x, int y)
{
   if (!gevonden)
   {
      printf("zetpad %d %d %d\n", d, x, y);

      xx[d] = x;
      yy[d] = y;
   }
   if (x == br-1 && y == ho-1)
   {
      gevonden = true;
      diepte = d+1;
      printf("gevonden %d\n", diepte);
   }
}

// Recursieve stapmethode
// Zoekt een volgende niet-bezochte buurcel
void Doolhof::stap(int d, int x, int y)
{
   Cel *c = getCel(x,y);
   if (c != NULL)
   {
      if (!c->bezocht)
      {
         c->bezocht = true;

         //printf("x %d y %d\n", x, y);
         //toonmetrand();

         zetpad(d, x, y);

           //Elke bit in vlaggen stelt een
           //richting voor.
         int vlaggen = 15;
         while (vlaggen != 0)
         {
             // Overloop de richtingen 
             // in een willekeurige volgorde. 
             int keuze = random() % 4;
             //printf("keuze %d\n", keuze);

             // Is de richting nog niet bezocht?
             int masker = 1 << keuze;
             if ((vlaggen & masker) != 0)
             {
                vlaggen &= ~(1 << keuze);
                //printf("keuze2 %d\n", keuze);

                int x2;
                int y2;
                Cel *c2;
                switch (keuze)
                {
                   case 0:
                      // boven
                      // Ga naar de buurcel.
                      x2 = x;
                      y2 = y-1;
                      c2 = getCel(x2, y2);

                      // Indien de cel bestaat en 
                      // niet bezocht is.
                      if (c2 != NULL && !c2->bezocht)
                      {
                         // Stel de muur open
                         c->boven->open = true;
                         stap(d+1, x2, y2);
                      }
                      break;

                   case 1:
                      // rechts
                      x2 = x+1;
                      y2 = y;
                      c2 = getCel(x2, y2);
                      if (c2 != NULL && !c2->bezocht)
                      {
                         c->rechts->open = true;
                         stap(d+1, x2, y2);
                      }
                      break;

                   case 2:
                      // onder
                      x2 = x;
                      y2 = y+1;
                      c2 = getCel(x2, y2);
                      if (c2 != NULL && !c2->bezocht)
                      {
                         c->onder->open = true;
                         stap(d+1, x2, y2);
                      }
                      break;

                   case 3:
                      // links
                      x2 = x-1;
                      y2 = y;
                      c2 = getCel(x2, y2);
                      if (c2 != NULL && !c2->bezocht)
                      {
                         c->links->open = true;
                         stap(d+1, x2, y2);
                      }
                      break;
                }
             }
         }
      }
   }
}

// Maak het doolhof.
void Doolhof::maak()
{
   stap(0, 0, 0);

   if (gevonden)
   {
      // Markeer de cellen die
      // deel uitmaken van het pad.
      for (int i=0; i<diepte; i++)
      {
         Cel *c = getCel(xx[i],yy[i]);
         c->pad = true;
      }
   }
}

// Haal een cel op als die bestaat.
Cel *Doolhof::getCel(int x, int y)
{
   if (x >= 0 && x < br && y >=0 && y <ho)
   {
      return cellen[y][x];
   }
   else
   {
      return NULL;
   }
}


 //Toon het doolhof. Geef aan of een
 //cel bezocht is of niet.

void Doolhof::toon()
{
   for (int ih = 0; ih<ho; ih++)
   {
      for (int ib = 0; ib<br; ib++)
      {
         if (cellen[ih][ib]->bezocht)
         {
            printf("x");
         }
         else
         {
            printf(".");
         }

      }
      printf("\n");
   }
}

// Toon het doolhof met muren.
void Doolhof::toonmetrand()
{
   for (int ih = 0; ih<ho; ih++)
   {
      // boven
      for (int ib = 0; ib<br; ib++)
      {
         if (cellen[ih][ib]->boven->open)
         {
            printf("+ +");
         }
         else
         {
            printf("+-+");
         }

      }
      printf("\n");

      // links, midden en rechts
      for (int ib = 0; ib<br; ib++)
      {
         if (cellen[ih][ib]->links->open)
         {
            printf(" ");
         }
         else
         {
            printf("|");
         }

         if (cellen[ih][ib]->bezocht)
         {
            printf("x");
         }
         else
         {
            printf(" ");
         }

         if (cellen[ih][ib]->rechts->open)
         {
            printf(" ");
         }
         else
         {
            printf("|");
         }

      }
      printf("\n");

      // onder
      for (int ib = 0; ib<br; ib++)
      {
         if (cellen[ih][ib]->onder->open)
         {
            printf("+ +");
         }
         else
         {
            printf("+-+");
         }
      }
      printf("\n");
   }
}


void Doolhof::toonmetenkelerand()
{
   for (int ih = 0; ih<ho; ih++)
   {
      // boven
      for (int ib = 0; ib<br; ib++)
      {
         if (cellen[ih][ib]->boven->open)
         {
            printf("+ ");
         }
         else
         {
            printf("+-");
         }

      }
      printf("+\n");

      // links, midden
      for (int ib = 0; ib<br; ib++)
      {
         if (cellen[ih][ib]->links->open)
         {
            printf(" ");
         }
         else
         {
            printf("|");
         }

         if (cellen[ih][ib]->pad)
         {
            printf("x");
         }
         else
         {
            printf(" ");
         }
      }

      // rechterrand
      if (cellen[ih][br-1]->rechts->open)
      {
         printf(" ");
      }
      else
      {
         printf("|");
      }
      printf("\n");
   }

   // onderrand
   for (int ib = 0; ib<br; ib++)
   {
      if (cellen[ho-1][ib]->onder->open)
      {
         printf("+ ");
      }
      else
      {
         printf("+-");
      }
    }
    printf("+\n");
}


void Doolhof::toonmetmuren()
{
   for (int ih = 0; ih<ho; ih++)
   {
      // boven
      for (int ib = 0; ib<br; ib++)
      {
         printf("  %2d  ", cellen[ih][ib]->boven->nr);
      }
      printf("\n");

      // links en rechts
      for (int ib = 0; ib<br; ib++)
      {
         printf("%2d %2d ",  
           cellen[ih][ib]->links->nr,
           cellen[ih][ib]->rechts->nr);
      }
      printf("\n");

      // onder
      for (int ib = 0; ib<br; ib++)
      {
         printf("  %2d  ", cellen[ih][ib]->onder->nr);
      }
      printf("\n");
   }
}


// Markeer alle muren als niet getekend.
void Doolhof::cleargetekend()
{
   for (int ih = 0; ih<ho; ih++)
   {
      for (int ib = 0; ib<br; ib++)
      {
         cellen[ih][ib]->boven->getekend  = false;
         cellen[ih][ib]->rechts->getekend = false;
         cellen[ih][ib]->onder->getekend  = false;
         cellen[ih][ib]->links->getekend  = false;
      }
   }
}


// Schrijf het ps bestand.
void Doolhof::tekenps(const char *fn, bool metpad)
{
   cleargetekend();

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
      const double bx        = 12.0; // was 12.0
      const double by        = 22.0; // was 12.0

      for (int pas=0; pas<2; pas++)
      {
         for (int iy=0; iy<ho; iy++)
         {
            for (int ix=0; ix<br; ix++)
            {
               Cel *c = getCel(ix,iy);

               double x1 = bx + zijde *ix;
               double y1 = by + zijde *iy;
               double x2 = x1 + zijde;
               double y2 = y1;
               double x3 = x1 + zijde;
               double y3 = y1 + zijde;
               double x4 = x1;
               double y4 = y1 + zijde;

               fprintf(fp, "%% cel %d, %d\n", ix, iy);

               if (pas == 0)
               {
                  const bool gekleurd = true;

                  double r = 1.0;
                  double g = 1.0;
                  double b = 1.0;

                  if (c->pad && metpad)
                  {
                     r = 0.8;
                     g = 0.8;
                     b = 0.8;
                  }
                  else
                  if (gekleurd)
                  {
                     int ra = random() % 3;
                     double vol  = 1.0;
                     double half = 0.6;
                     switch (ra)
                     {
                        case 0:
                           r = vol;
                           g = half;
                           b = half;
                           break;

                        case 1:
                           r = half;
                           g = vol;
                           b = half;
                           break;

                        case 2:
                           r = half;
                           g = half;
                           b = vol;
                           break;
                     }
                  }

                  // teken het vullen
                  if (!c->wit && (gekleurd || c->pad && metpad))
                  {
                     fprintf(fp, "%lf %lf newpath moveto\n", x1, y1);
                     fprintf(fp, "%lf %lf lineto\n", x2, y2);
                     fprintf(fp, "%lf %lf lineto\n", x3, y3);
                     fprintf(fp, "%lf %lf lineto\n", x4, y4);
                     //   fprintf(fp, "%lf %lf lineto\n", x1, y1);
                     fprintf(fp, "closepath\n");
                     fprintf(fp, "%lf %lf %lf setrgbcolor\n", r, g, b);
                     //fprintf(fp, "0.8 setgray\n");
                     fprintf(fp, "fill\n");
                     fprintf(fp, "\n");
                  }
               }
               else
               {

               // geen lijnen als de cel wit is
               if (!c->wit)
               {
               // teken de lijnen

               fprintf(fp, "%lf setlinewidth\n", linewidth);
               fprintf(fp, "%lf %lf newpath moveto\n", x1, y1);

               Muur *m;

               // boven
               m = c->boven;
               fprintf(fp, "%% muur %d boven %d\n", m->nr, m->getekend);
               if (!m->open && !m->getekend)
               {
                  fprintf(fp, "%lf %lf lineto\n", x2, y2);
               }
               else
               {
                  fprintf(fp, "%lf %lf moveto\n", x2, y2);
               }
               m->getekend = true;


               // rechts
               m = c->rechts;
               fprintf(fp, "%% muur %d rechts %d\n", c->rechts->nr, c->rechts->getekend);
               if (!m->open && !m->getekend)
               {
                  fprintf(fp, "%lf %lf lineto\n", x3, y3);
               }
               else
               {
                  fprintf(fp, "%lf %lf moveto\n", x3, y3);
               }
               m->getekend = true;

               // onder
               m = c->onder;
               fprintf(fp, "%% muur %d onder %d\n", c->onder->nr, c->onder->getekend);
               if (!m->open && !m->getekend)
               {
                  fprintf(fp, "%lf %lf lineto\n", x4, y4);
               }
               else
               {
                  fprintf(fp, "%lf %lf moveto\n", x4, y4);
               }
               m->getekend = true;

               // links
               m = c->links;
               fprintf(fp, "%% muur %d links %d\n", m->nr, m->getekend);
               if (!m->open && !m->getekend)
               {
                  fprintf(fp, "%lf %lf lineto\n", x1, y1);
               }
               else
               {
                  fprintf(fp, "%lf %lf moveto\n", x1, y1);
               }
               m->getekend = true;

               //fprintf(fp, "closepath\n");
               fprintf(fp, "0 setgray\n");
               fprintf(fp, "%lf setlinewidth\n", linewidth);
               fprintf(fp, "stroke\n");

               fprintf(fp, "\n");
               }
               }
            }
         }
      }

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


      fprintf(fp, "showpage\n");

      fclose(fp);
   }
}


int main()
{
   srandom(getpid());

   Doolhof *d = new Doolhof();

   // Neem de witruimte over in het doolhof.
   for (int x=0; x<Doolhof::br; x++)
   {
      for (int y=0; y<Doolhof::ho; y++)
      {
         if (figuur[x][y] != ' ')
         {
            d->zetwit(x,y);
         }
      }
   }
   d->maak();

   //d->toonmetenkelerand();

   //printf("-----------------\n");
   //d->toonmetmuren();

   d->tekenps("labyrint1.ps", false);
   d->tekenps("labyrint1-opl.ps", true);
   delete d;
}
 */
