//
// Observable.hh for pekwm
// Copyright © 2009 Claes Nästen <me@pekdon.net>
//
// This program is licensed under the GNU GPL.
// See the LICENSE file for more information.
//

#ifndef _OBSERVABLE_HH_
#define _OBSERVABLE_HH_

#ifdef HAVE_SLIST
#include <slist>
#else // HAVE_EXT_SLIST
#include <ext/slist>
#endif // HAVE_SLIST

class Observer;

/**
 * Message sent to observer.
 */
class Observation {
public:
    virtual ~Observation(void) { };
};

class Observable {
public:
    Observable(void) { }
    virtual ~Observable(void) { }

    void notifyObservers(Observation *observation);

    void addObserver(Observer *observer); 
    void removeObserver(Observer *observer); 

private:
    SLIST_NAMESPACE::slist<Observer*> _observers; /**< List of observers. */
};

#endif // _OBSERVABLE_HH_
