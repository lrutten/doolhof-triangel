#ifndef H_TRIANGLE
#define H_TRIANGLE

#include <vector>

#include "numeric.h"
#include "point.h"
#include "wall.h"

namespace dt {

template<typename T>
class Triangle
{
public:
	using Type = T;
	using VertexType = Point<Type>;
	using WallType   = Wall<Type>;

	Triangle() = default;
	Triangle(const Triangle&) = default;
	Triangle(Triangle&&) = default;
	Triangle(const VertexType *v1, const VertexType *v2, const VertexType *v3);

	bool containsVertex(const VertexType *v) const;
	bool circumCircleContains(const VertexType *v) const;

	Triangle &operator=(const Triangle&) = default;
	Triangle &operator=(Triangle&&) = default;
	bool operator ==(const Triangle &t) const;

	template<typename U>
	friend std::ostream &operator <<(std::ostream &str, const Triangle<U> &t);

	const VertexType *a;
	const VertexType *b;
	const VertexType *c;
	bool              isBad = false;
   bool              visited; // is already visited
   bool              onpad;   // this triangle is part of the path
   bool              white;   // this triangle must be avoided
   std::vector<Wall<T>> walls;

   void setwhite()
   {
      white   = true;
      visited = true;
   }

	static_assert(std::is_floating_point<Triangle<T>::Type>::value,
		"Type must be floating-point");
};

template<typename T>
bool almost_equal(const Triangle<T> &t1, const Triangle<T> &t2)
{
	return	(almost_equal(*t1.a , *t2.a) || almost_equal(*t1.a , *t2.b) || almost_equal(*t1.a , *t2.c)) &&
			(almost_equal(*t1.b , *t2.a) || almost_equal(*t1.b , *t2.b) || almost_equal(*t1.b , *t2.c)) &&
			(almost_equal(*t1.c , *t2.a) || almost_equal(*t1.c , *t2.b) || almost_equal(*t1.c , *t2.c));
}

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
