#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <array>
#include <random>
#include <chrono>

#include <SFML/Graphics.hpp>

#include "point.h"
#include "triangle.h"
#include "maze.h"

double distance(const dt::Point *p1, const dt::Point *p2)
{
   double dx = p1->x - p2->x;
   double dy = p1->y - p2->y;

   return sqrt(dx*dx + dy*dy);
}

bool too_close(const	std::vector<dt::Point *> &points, const dt::Point *pn, const double max_dist)
{
   for (const auto p : points)
   {
      if (distance(p, pn) < max_dist)
      {
         return true;
      }
   }
   return false;
}

int main(int argc, char * argv[])
{
   const int width  = 800;
   const int height = 800;
   const int grid   = 400;
   const double dfactor = 0.8;
   const double max_dist = dfactor*grid;
	int numberPoints = 40;
	if (argc>1)
	{
		numberPoints = atoi(argv[1]);
	}


	std::default_random_engine eng(std::random_device{}());
	std::uniform_real_distribution<double> dist_w(0, width);
	std::uniform_real_distribution<double> dist_h(0, height);

	std::cout << "Generating " << numberPoints << " random points" << std::endl;

	std::vector<dt::Point *> points;

	// horizontal border
   for (int i = 1; i < width/grid; ++i)
   {
      dt::Point *p = new dt::Point{static_cast<double>(grid*i), 0.0};
      p->border = true;
      points.push_back(p);
      dt::Point *p2 = new dt::Point{static_cast<double>(grid*i), static_cast<double>(height)};
      p2->border = true;
      points.push_back(p2);
   }

   // vertical border
   for (int i = 1; i < height/grid; ++i)
   {
      dt::Point *p = new dt::Point{0.0, static_cast<double>(grid*i)};
      points.push_back(p);
      dt::Point *p2 = new dt::Point{static_cast<double>(width), static_cast<double>(grid*i)};
      points.push_back(p2);
   }

   // 4 corners
   dt::Point *pc1 = new dt::Point{static_cast<double>(0.0), static_cast<double>(0.0)};
   pc1->border = true;
   pc1->corner = true;
   points.push_back(pc1);
   dt::Point *pc2 = new dt::Point{static_cast<double>(width), static_cast<double>(0.0)};
   pc2->border = true;
   pc2->corner = true;
   points.push_back(pc2);
   dt::Point *pc3 = new dt::Point{static_cast<double>(0.0), static_cast<double>(height)};
   pc3->border = true;
   pc3->corner = true;
   points.push_back(pc3);
   dt::Point *pc4 = new dt::Point{static_cast<double>(width), static_cast<double>(height)};
   pc4->border = true;
   pc4->corner = true;
   points.push_back(pc4);


   for (int i = 0; i < numberPoints; ++i)
	{
      dt::Point *p = new dt::Point{dist_w(eng), dist_h(eng)};

      if (!too_close(points, p, max_dist))
      {
         points.push_back(p);
      }
      else
      {
         //std::cout << "too close\n";
      }
	}

	dt::Maze *triangulation = new dt::Maze();
	const auto start = std::chrono::high_resolution_clock::now();
	const std::vector<dt::Triangle *> triangles = triangulation->triangulate(points);
	const auto end = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> diff = end - start;

	std::cout << triangles.size() << " triangles generated in " << diff.count() << "s\n";
	const std::vector<dt::Wall *> walls = triangulation->getWalls();

   triangulation->connect();

   /*
	for (const dt::Wall<double> *wa: walls)
   {
	   std::cout << "Wall " << wa->nr << "\n";
   }
    */

   bool gui = true;
   if (gui)
   {
      // SFML window
      sf::RenderWindow window(sf::VideoMode(width, height), "Maze triangulation");
      window.setFramerateLimit(1);

      // Transform each points of each vector as a rectangle
      for (const auto p : points) {
         sf::RectangleShape s{sf::Vector2f(4, 4)};
         s.setPosition(static_cast<float>(p->x), static_cast<float>(p->y));
         window.draw(s);
      }

      std::vector<std::array<sf::Vertex, 2> > lines;
      for (const auto &e: walls) {
         const std::array<sf::Vertex, 2> line
                 {{
                          sf::Vertex(sf::Vector2f(
                                  static_cast<float>(e->v->x + 2.),
                                  static_cast<float>(e->v->y + 2.))),
                          sf::Vertex(sf::Vector2f(
                                  static_cast<float>(e->w->x + 2.),
                                  static_cast<float>(e->w->y + 2.))),
                  }};
         window.draw(line.data(), 2, sf::Lines);
      }

      window.display();

      while (window.isOpen()) {
         sf::Event event;
         while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
               window.close();
         }
      }
   }

   delete triangulation;

	return 0;
}



/*
int main2()
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
