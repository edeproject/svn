#ifndef __EDBUSCONTAINER_H__
#define __EDBUSCONTAINER_H__

#include <assert.h>
#include <list>

#ifndef SKIP_DOCS
template <typename T>
struct EdbusContainerImpl {
	std::list<T> lst;
	unsigned int ref;
};
#endif

/**
 * \class EdbusContainer
 * \brief Abstract container for D-BUS containers
 *
 * EdbusContainer is a class for easier creating EdbusDict and EdbusList containers.
 * It uses implicit sharing so all concrete implementations copy internal data only
 * when is needed.
 *
 * This class should be used as base class so inherited classes can provide additional
 * funcionality.
 */
template <typename T>
class EdbusContainer {
	public:
		typedef typename std::list<T>::iterator iterator;
		typedef typename std::list<T>::const_iterator const_iterator;
		typedef EdbusContainerImpl<T> EdbusContainerPrivate;

	protected:
		EdbusContainerPrivate* impl;

		/**
		 * Clears internal data
		 */
		void dispose(void) {
			if(!impl)
				return;

			delete impl;
			impl = 0;
		}

		/**
		 * Do actual copying. Referece counter is set to 1.
		 * This function must be called when inherited implementations
		 * do write or change internal data
		 */
		void unhook(void) {
			assert(impl != NULL);

			if(impl->ref == 1)
				return;

			EdbusContainerPrivate* new_one = new EdbusContainerPrivate;
			new_one->ref = 1;

			/* 
			 * Copy the content
			 *
			 * edelib::list does not have implemented copy operator
			 * and that is the way I like
			 */
			if(impl->lst.size() > 0) {
				iterator it = impl->lst.begin(), it_end = impl->lst.end();

				while(it != it_end) {
					new_one->lst.push_back(*it);
					++it;
				}
			}

			impl->ref--;
			impl = new_one;
		}

		/**
		 * Create empty container
		 */
		EdbusContainer() : impl(0) {
			impl = new EdbusContainerPrivate;
			impl->ref = 1;
		};

		/**
		 * Do a shallow copying from other container
		 */
		EdbusContainer(const EdbusContainer& other) {
			if(this == &other)
				return;

			impl = other.impl;
			other.impl->ref++;
		}

		/**
		 * Decrease reference counter and if reached 0 it will
		 * clear allocated data
		 */
		~EdbusContainer() {
			impl->ref--;

			if(impl->ref == 0)
				dispose();
		}

		/**
		 * Do a shallow copying from other container
		 */
		EdbusContainer& operator=(const EdbusContainer& other) {
			other.impl->ref++;
			impl->ref--;

			if(impl->ref == 0)
				dispose();

			impl = other.impl;
			return *this;
		}
};

#endif
