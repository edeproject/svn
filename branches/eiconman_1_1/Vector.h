#ifndef __VECTOR_H__
#define __VECTOR_H__

/*
 * Herb Sutter said:
 *  "Reuse code specially standard library code instead of handcrafting your own. 
 *   It's faster, easier, and safer."
 *
 * I say:
 *  "Yea right. This is more fun!"
 */

#include <stddef.h>
#include <string.h>	// memcpy, memmove
#include <assert.h>

inline void* operator new(size_t s, void* p)
{
	return p;
}

#define __IT_ASSERT(it) assert(it != NULL && "Iterator have NULL value")

template <typename T>
inline void __Construct(T* p)
{
	new (p) T;
}

template <typename T>
inline void __Construct(T* p, const T& val)
{
	new (p) T(val);
}

template <typename T>
inline void __Destruct(T* p)
{
	p->~T();
}

template <typename T>
class Vector
{
	private:
		T*     ptr_;
		size_t size_;
		size_t alloc_;

		Vector(const Vector<T>&);
		void operator=(const Vector<T>&);
		void append(size_t v);
		void __Realloc(size_t s);

	public:
		typedef size_t   size_type;
		typedef T*       iterator;
		typedef const T* const_iterator;

		Vector()         : ptr_(0), size_(0), alloc_(0) { }
		Vector(size_t n) : ptr_(0), size_(0), alloc_(0) { append(n); }
		Vector(size_t n, const T& value) : ptr_(0), size_(0), alloc_(0) { insert(ptr_+size_, n, value); }
		~Vector();

		iterator begin()             { return ptr_; }
		const_iterator begin() const { return ptr_; }

		iterator end()               { return ptr_ + size_; }
		const_iterator end() const   { return ptr_ + size_; }

		size_type capacity(void) const  { return alloc_; }
		size_type size(void)            { return size_; }

		void clear(void)             { erase(begin(), end()); }

		T& operator[](size_t i) { return ptr_[i]; }
		const T& operator[](size_t i) const { return ptr_[i]; }

		iterator erase(const_iterator b, const_iterator e);
		void reserve(size_t s) { if(alloc_ < s) __Realloc(s); }
		void insert(const_iterator pos, size_t s, const T& value);

		void push_back(const T& t);
		bool empty(void)             { return (size_ == 0); }
};

template<typename T>
void Vector<T>::append(size_t s)
{
	reserve(size_ + s);
	while(s-- > 0)
	{
		__Construct(ptr_ + size_);
		size_++;
	}
}

template<typename T>
T* Vector<T>::erase(const T* first, const T* last)
{
	if(first == last)
		return (T*) first;

	__IT_ASSERT(first); __IT_ASSERT(last);

	// bounds check
	int bounds = size_;

	for(const_iterator p = first; p != last && bounds >= 0; ++p, --bounds)
		__Destruct(p);
	
	// now rellocate data if needed
	if(last != ptr_ + size_)
		memmove((T*)first, last, sizeof(T) * ((ptr_ + size_) - last));
	size_ -= last - first;
	return (T*) first;
}

template<typename T>
Vector<T>::~Vector()
{
	if(!ptr_)
		return;
	erase(begin(), end());
	::operator delete((void*)ptr_);
}

template<typename T>
void Vector<T>::__Realloc(size_t sz)
{
	printf("Request %i size: %i\n", alloc_, sz);
	size_t newsz = alloc_ * 2;
	if(sz > newsz)
		newsz += sz;
	void* p = ::operator new(newsz * sizeof(T));
	alloc_ = newsz;
	if(ptr_)
	{
		memcpy(p, ptr_, size_ * sizeof(T));
		::operator delete((void*)ptr_);
	}
	ptr_ = (T*) p;
}

template<typename T>
void Vector<T>::push_back(const T& val)
{
	reserve(size_ + 1);
	__Construct(ptr_ + size_, val);
	size_++;
}

template<typename T>
void Vector<T>::insert(const T* pos, size_t s, const T& value)
{
	__IT_ASSERT(pos);

	size_t i = pos - ptr_;
	reserve(size_ + s);
	if(i != size_)
		memmove(i + ptr_ + s, ptr_ + i, (size_ - i) * sizeof(T));
	for(T* pp = ptr_ + i; s-- > 0; ++pp)
	{
		__Construct(pp, value);
		size_++;
	}
}

#define vector Vector

#endif
