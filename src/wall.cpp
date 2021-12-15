#include "wall.h"

namespace dt
{
template<typename T>
Wall<T>::Wall(const VertexType *v1, const VertexType *v2) :
	v(v1), w(v2), drawn(false), open(false), nr(counter++)
{
   // std::cout << "wall " << nr << "\n";
}

template<typename T>
bool
Wall<T>::operator ==(const Wall<T> &e) const
{
	return (*(this->v) == *e.v && *(this->w) == *e.w) ||
			(*(this->v) == *e.w && *(this->w) == *e.v);
}

template<typename U>
std::ostream&
operator<<(std::ostream &str, const Wall<U> &e)
{
   return str << "Wall " << *e.v << ", " << *e.w;
}

template class Wall<float>;
template class Wall<double>;

template<class T> int Wall<T>::counter = 0;

} // namespace dt


