#include "maze.h"

namespace dt
{

template<typename T>
const std::vector<typename Maze<T>::TriangleType *>&
Maze<T>::triangulate(std::vector<VertexType *> &vertices)
{
	// Store the vertices locally
	_vertices = vertices;

	// Determinate the super triangle
	T minX = vertices[0]->x;
	T minY = vertices[0]->y;
	T maxX = minX;
	T maxY = minY;

	for(std::size_t i = 0; i < vertices.size(); ++i)
	{
		if (vertices[i]->x < minX) minX = vertices[i]->x;
		if (vertices[i]->y < minY) minY = vertices[i]->y;
		if (vertices[i]->x > maxX) maxX = vertices[i]->x;
		if (vertices[i]->y > maxY) maxY = vertices[i]->y;
	}

	const T dx = maxX - minX;
	const T dy = maxY - minY;
	const T deltaMax = std::max(dx, dy);
	const T midx = (minX + maxX) / 2;
	const T midy = (minY + maxY) / 2;

	const VertexType *p1 = new VertexType(midx - 20 * deltaMax, midy - deltaMax);
	const VertexType *p2 = new VertexType(midx, midy + 20 * deltaMax);
	const VertexType *p3 = new VertexType(midx + 20 * deltaMax, midy - deltaMax);

	// Create a list of triangles, and add the supertriangle in it
	_triangles.push_back(new TriangleType(p1, p2, p3));

	for(auto p = begin(vertices); p != end(vertices); p++)
	{
		std::vector<WallType *> polygon;

		for(auto & t : _triangles)
		{
			if(t->circumCircleContains(*p))
			{
				t->isBad = true;
				polygon.push_back(new Wall<T>{t->a, t->b});
				polygon.push_back(new Wall<T>{t->b, t->c});
				polygon.push_back(new Wall<T>{t->c, t->a});
			}
		}

		_triangles.erase(std::remove_if(begin(_triangles), end(_triangles), [](TriangleType *t)
		{
			return t->isBad;
		}), end(_triangles));

		for(auto e1 = begin(polygon); e1 != end(polygon); ++e1)
		{
			for(auto e2 = e1 + 1; e2 != end(polygon); ++e2)
			{
				if(almost_equal(*e1, *e2))
				{
               (*e1)->isBad = true;
               (*e2)->isBad = true;
				}
			}
		}

		polygon.erase(std::remove_if(begin(polygon), end(polygon), [](WallType *e)
		{
			return e->isBad;
		}), end(polygon));

		for(const auto e : polygon)
			_triangles.push_back(new TriangleType(e->v, e->w, *p));

	}

	_triangles.erase(std::remove_if(begin(_triangles), end(_triangles), [p1, p2, p3](TriangleType *t)
	{
		return t->containsVertex(p1) || t->containsVertex(p2) || t->containsVertex(p3);
	}), end(_triangles));

	for(const auto t : _triangles)
	{
		_Walls.push_back(new Wall<T>{t->a, t->b});
		_Walls.push_back(new Wall<T>{t->b, t->c});
		_Walls.push_back(new Wall<T>{t->c, t->a});
	}

	return _triangles;
}

template<typename T>
const std::vector<typename Maze<T>::TriangleType *>&
Maze<T>::getTriangles() const
{
	return _triangles;
}

template<typename T>
const std::vector<typename Maze<T>::WallType *>&
Maze<T>::getWalls() const
{
	return _Walls;
}

template<typename T>
const std::vector<typename Maze<T>::VertexType *>&
Maze<T>::getVertices() const
{
	return _vertices;
}

template<typename T>
void Maze<T>::connect()
{
   for (const dt::Wall<T> *wa: _Walls)
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
   }
}



   template class Maze<float>;
template class Maze<double>;

} // namespace dt



#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


/*


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
