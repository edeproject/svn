//
// ManagerWindows.cc for pekwm
// Copyright © 2009 Claes Nästén <me@pekdon.net>
//
// This program is licensed under the GNU GPL.
// See the LICENSE file for more information.
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "Config.hh"
#include "ActionHandler.hh"
#include "Atoms.hh"
#include "ManagerWindows.hh"
#include "Util.hh"

#include <string>

extern "C" {
#include <unistd.h>
}

using std::cerr;
using std::endl;
using std::string;

// Static initializers
const string HintWO::WM_NAME = string("pekwm");
HintWO *HintWO::_instance = 0;
const unsigned int HintWO::DISPLAY_WAIT = 10;

const unsigned long RootWO::EVENT_MASK =
    StructureNotifyMask|PropertyChangeMask|
    SubstructureNotifyMask|SubstructureRedirectMask|
    ColormapChangeMask|FocusChangeMask|EnterWindowMask|
    ButtonPressMask|ButtonReleaseMask|ButtonMotionMask;
const unsigned long RootWO::EXPECTED_DESKTOP_NAMES_LENGTH = 256;

/**
 * Hint window constructor, creates window and sets supported
 * protocols.
 */
HintWO::HintWO(Window root, bool replace) throw (std::string&)
    : PWinObj()
{
    if (_instance) {
        throw string("trying to create HintWO which is already created.");
    }
    _instance = this;

    _type = WO_SCREEN_HINT;
    setLayer(LAYER_NONE);
    _sticky = true; // Hack, do not map/unmap this window
    _iconified = true; // Hack, to be ignored when placing

    // Create window
    _window = XCreateSimpleWindow(_dpy, root, -200, -200, 5, 5, 0, 0, 0);

    // Remove override redirect from window
    XSetWindowAttributes attr;
    attr.override_redirect = True;
    attr.event_mask = PropertyChangeMask;
    XChangeWindowAttributes(_dpy, _window, CWEventMask|CWOverrideRedirect, &attr);

    // Set hints not being updated
    AtomUtil::setString(_window, Atoms::getAtom(NET_WM_NAME), WM_NAME);
    AtomUtil::setWindow(_window, Atoms::getAtom(NET_SUPPORTING_WM_CHECK), _window);

    if (! claimDisplay(replace)) {
        throw string("unable to claim display");
    }
}

/**
 * Hint WO destructor, destroy hint window.
 */
HintWO::~HintWO(void)
{
    XDestroyWindow(PScreen::instance()->getDpy(), _window);
    _instance = 0;
}

/**
 * Get current time of server by generating an event and reading the
 * timestamp on it.
 *
 * @return Time on server.
 */
Time
HintWO::getTime(void)
{
    XEvent event;

    // Generate event on ourselves
    XChangeProperty(_dpy, _window,
		    Atoms::getAtom(WM_CLASS), Atoms::getAtom(STRING),
                    8, PropModeAppend, 0, 0);
    XWindowEvent(_dpy, _window, PropertyChangeMask, &event);

    return event.xproperty.time;
}

/**
 * Claim ownership over the current display.
 *
 * @param replace Replace current running window manager.
 */
bool
HintWO::claimDisplay(bool replace)
{
    bool status = true;

    // Get atom for the current screen and it's owner
    string session_name("WM_S" + Util::to_string<int>(DefaultScreen(_dpy)));
    Atom session_atom = XInternAtom(_dpy, session_name.c_str(), false);
    Window session_owner = XGetSelectionOwner(_dpy, session_atom);

    if (session_owner && session_owner != _window) {
        if (! replace) {
            cerr << " *** WARNING: window manager already running." << endl;
            return false;
        }

        XSync(_dpy, false);
        setXErrorsIgnore(true);
        uint errors_before = xerrors_count;

        // Select event to get notified when current owner dies.
        XSelectInput(_dpy, session_owner, StructureNotifyMask);

        XSync(_dpy, false);
        setXErrorsIgnore(false);
        if (errors_before != xerrors_count) {
            session_owner = None;
        }
    }

    Time timestamp = getTime();

    XSetSelectionOwner(_dpy, session_atom, _window, timestamp);
    if (XGetSelectionOwner(_dpy, session_atom) == _window) {
        if (session_owner) {
            // Wait for the previous window manager to go away and update owner.
            status = claimDisplayWait(session_owner);
            if (status) {
                claimDisplayOwner(session_atom, timestamp);
            }
        }
    } else {
      cerr << "pekwm: unable to replace current window manager." << endl;
      status = false;
    }

    return status;
}


/**
 * After claiming the display, wait for the previous window manager to
 * go away.
 */
bool
HintWO::claimDisplayWait(Window session_owner)
{
    XEvent event;

    cerr << " *** INFO: waiting for previous window manager to exit. " << endl;

    for (uint waited = 0; waited < HintWO::DISPLAY_WAIT; ++waited) {
        if (XCheckWindowEvent(_dpy, session_owner, StructureNotifyMask, &event)
            && event.type == DestroyNotify) {
            return true;
        }

        sleep(1);
    }

    cerr << " *** INFO: previous window manager did not exit. " << endl;

    return false;
}

/**
 * Send message updating the owner of the screen.
 */
void
HintWO::claimDisplayOwner(Window session_atom, Time timestamp)
{
    XEvent event;
    // FIXME: One should use _root_wo here?
    Window root = RootWindow(_dpy, DefaultScreen(_dpy));

    event.xclient.type = ClientMessage;
    event.xclient.message_type = Atoms::getAtom(MANAGER);
    event.xclient.display = _dpy;
    event.xclient.window = root;
    event.xclient.format = 32;
    event.xclient.data.l[0] = timestamp;
    event.xclient.data.l[1] = session_atom;
    event.xclient.data.l[2] = _window;
    event.xclient.data.l[3] = 0;

    XSendEvent(_dpy, root, false, SubstructureNotifyMask, &event);
}

/**
 * Root window constructor, reads geometry and sets basic atoms.
 */
RootWO::RootWO(Window root)
    : PWinObj()
{
    _type = WO_SCREEN_ROOT;
    setLayer(LAYER_NONE);
    _mapped = true;

    _window = root;
    _gm.width = PScreen::instance()->getWidth();
    _gm.height = PScreen::instance()->getHeight();


    XSync(_dpy, false);
    setXErrorsIgnore(true);
    uint errors_before = xerrors_count;

    // Select window events
    XSelectInput(_dpy, _window, RootWO::EVENT_MASK);

    XSync(_dpy, false);
    setXErrorsIgnore(false);
    if (errors_before != xerrors_count) {
        cerr << "pekwm: root window unavailable, can't start!" << endl;
        exit(1);
    }

    // Set hits on the hint window, these are not updated so they are
    // set in the constructor.
    AtomUtil::setLong(_window, Atoms::getAtom(NET_WM_PID), static_cast<long>(getpid()));
    AtomUtil::setString(_window, Atoms::getAtom(WM_CLIENT_MACHINE), Util::getHostname());

    AtomUtil::setWindow(_window, Atoms::getAtom(NET_SUPPORTING_WM_CHECK), HintWO::instance()->getWindow());
    Atoms::setEwmhAtomsSupport(_window);
    AtomUtil::setLong(_window, Atoms::getAtom(NET_NUMBER_OF_DESKTOPS), Config::instance()->getWorkspaces());
    AtomUtil::setLong(_window, Atoms::getAtom(NET_CURRENT_DESKTOP), 0);

    long desktop_geometry[2];
    desktop_geometry[0] = _gm.width;
    desktop_geometry[1] = _gm.height;
    AtomUtil::setLongs(_window, Atoms::getAtom(NET_DESKTOP_GEOMETRY), desktop_geometry, 2);

    woListAdd(this);
    _wo_map[_window] = this;
}

/**
 * Root window destructor, clears atoms set.
 */
RootWO::~RootWO(void)
{
    // Remove atoms, PID will not be valid on shutdown.
    AtomUtil::unsetProperty(_window, Atoms::getAtom(NET_WM_PID));
    AtomUtil::unsetProperty(_window, Atoms::getAtom(WM_CLIENT_MACHINE));

    _wo_map.erase(_window);
    woListRemove(this);
}

/**
 * Button press event handler, gets actions from root list.
 */
ActionEvent*
RootWO::handleButtonPress(XButtonEvent *ev)
{
    return ActionHandler::findMouseAction(ev->button, ev->state, MOUSE_EVENT_PRESS,
                                          Config::instance()->getMouseActionList(MOUSE_ACTION_LIST_ROOT));
}

/**
 * Button release event handler, gets actions from root list.
 */
ActionEvent*
RootWO::handleButtonRelease(XButtonEvent *ev)
{
    MouseEventType mb = MOUSE_EVENT_RELEASE;

    // first we check if it's a double click
    if (PScreen::instance()->isDoubleClick(ev->window, ev->button - 1, ev->time,
                                           Config::instance()->getDoubleClickTime())) {
        PScreen::instance()->setLastClickID(ev->window);
        PScreen::instance()->setLastClickTime(ev->button - 1, 0);

        mb = MOUSE_EVENT_DOUBLE;

    } else {
        PScreen::instance()->setLastClickID(ev->window);
        PScreen::instance()->setLastClickTime(ev->button - 1, ev->time);
    }

    return ActionHandler::findMouseAction(ev->button, ev->state, mb,
                                          Config::instance()->getMouseActionList(MOUSE_ACTION_LIST_ROOT));
}

/**
 * Motion event handler, gets actions from root list.
 */
ActionEvent*
RootWO::handleMotionEvent(XMotionEvent *ev)
{
    unsigned int button = PScreen::instance()->getButtonFromState(ev->state);

    return ActionHandler::findMouseAction(button, ev->state, MOUSE_EVENT_MOTION,
                                          Config::instance()->getMouseActionList(MOUSE_ACTION_LIST_ROOT));
}

/**
 * Enter event handler, gets actions from root list.
 */
ActionEvent*
RootWO::handleEnterEvent(XCrossingEvent *ev)
{
    return ActionHandler::findMouseAction(BUTTON_ANY, ev->state, MOUSE_EVENT_ENTER,
                                          Config::instance()->getMouseActionList(MOUSE_ACTION_LIST_ROOT));
}

/**
 * Leave event handler, gets actions from root list.
 */
ActionEvent*
RootWO::handleLeaveEvent(XCrossingEvent *ev)
{
    return ActionHandler::findMouseAction(BUTTON_ANY, ev->state, MOUSE_EVENT_LEAVE,
                                          Config::instance()->getMouseActionList(MOUSE_ACTION_LIST_ROOT));
}

/**
 * Update _NET_WORKAREA property.
 *
 * @param workarea Geometry with work area.
 */
void
RootWO::setEwmhWorkarea(const Geometry &workarea)
{
    long workarea_array[4];
    workarea_array[0] = workarea.x;
    workarea_array[1] = workarea.y;
    workarea_array[2] = workarea.width;
    workarea_array[3] = workarea.height;
    AtomUtil::setLongs(_window, Atoms::getAtom(NET_WORKAREA), workarea_array, 4);
}

/**
 * Update _NET_ACTIVE_WINDOW property.
 *
 * @param win Window to set as active window.
 */
void
RootWO::setEwmhActiveWindow(Window win)
{
    AtomUtil::setWindow(PScreen::instance()->getRoot(), Atoms::getAtom(NET_ACTIVE_WINDOW), win);
}

/**
 * Reads the _NET_DESKTOP_NAMES hint and sets the workspaces names accordingly.
 */
void
RootWO::readEwmhDesktopNames(void)
{
    uchar *data;
    ulong data_length;
    if (AtomUtil::getProperty(PScreen::instance()->getRoot(),
                              Atoms::getAtom(NET_DESKTOP_NAMES),
                              Atoms::getAtom(UTF8_STRING),
                              EXPECTED_DESKTOP_NAMES_LENGTH, &data, &data_length)) {
        Config::instance()->setDesktopNamesUTF8(reinterpret_cast<char *>(data), data_length);

        XFree(data);
    }
}

/**
 * Update _NET_DESKTOP_NAMES property on the root window.
 */
void
RootWO::setEwmhDesktopNames(void)
{
    unsigned char *desktopnames = 0;
    unsigned int length = 0;
    Config::instance()->getDesktopNamesUTF8(&desktopnames, &length);

    if (desktopnames) {
        AtomUtil::setUtf8StringArray(PScreen::instance()->getRoot(),
                                     Atoms::getAtom(NET_DESKTOP_NAMES), desktopnames, length);
        delete [] desktopnames;
    }
}


/**
 * Edge window constructor, create window, setup strut and register
 * window.
 */
EdgeWO::EdgeWO(Window root, EdgeType edge, bool set_strut)
    : PWinObj(),
      _edge(edge)
{
    _type = WO_SCREEN_EDGE;
    setLayer(LAYER_NONE); // hack, goes over LAYER_MENU
    _sticky = true; // don't map/unmap
    _iconified = true; // hack, to be ignored when placing
    _focusable = false; // focusing input only windows crashes X

    XSetWindowAttributes sattr;
    sattr.override_redirect = True;
    sattr.event_mask = EnterWindowMask|LeaveWindowMask|ButtonPressMask|ButtonReleaseMask;

    _window = XCreateWindow(_dpy, root, 0, 0, 1, 1, 0, CopyFromParent, InputOnly, CopyFromParent,
                            CWOverrideRedirect|CWEventMask, &sattr);

    configureStrut(set_strut);
    PScreen::instance()->addStrut(&_strut);

    woListAdd(this);
    _wo_map[_window] = this;
}

/**
 * Edge window destructor, remove strut and destroy window resources.
 */
EdgeWO::~EdgeWO(void)
{
    PScreen::instance()->removeStrut(&_strut);
    _wo_map.erase(_window);
    woListRemove(this);

    XDestroyWindow(_dpy, _window);
}

/**
 * Configure strut on edge window.
 *
 * @param set_strut If true, set actual values on the strut, false sets all to 0.
 */
void
EdgeWO::configureStrut(bool set_strut)
{
    // Reset value, on strut to zero.
    _strut.left = _strut.right = _strut.top = _strut.bottom = 0;

    // Set strut if requested.
    if (set_strut) {
        switch (_edge) {
        case SCREEN_EDGE_TOP:
            _strut.top = _gm.height;
            break;
        case SCREEN_EDGE_BOTTOM:
            _strut.bottom = _gm.height;
            break;
        case SCREEN_EDGE_LEFT:
            _strut.left = _gm.width;
            break;
        case SCREEN_EDGE_RIGHT:
            _strut.right = _gm.width;
            break;
        case SCREEN_EDGE_NO:
        default:
            // do nothing
            break;
        }
    }
}

/**
 * Edge version of mapped window, makes sure the iconified state is
 * set at all times in order to avoid counting the edge windows when
 * snapping windows etc.
 */
void
EdgeWO::mapWindow(void)
{
    if (_mapped) {
        return;
    }

    PWinObj::mapWindow();
    _iconified = true;
}

/**
 * Enter event handler, gets actions from EdgeList on _edge.
 */
ActionEvent*
EdgeWO::handleEnterEvent(XCrossingEvent *ev)
{
    return ActionHandler::findMouseAction(BUTTON_ANY, ev->state, MOUSE_EVENT_ENTER,
                                          Config::instance()->getEdgeListFromPosition(_edge));
}

/**
 * Button press event handler, gets actions from EdgeList on _edge.
 */
ActionEvent*
EdgeWO::handleButtonPress(XButtonEvent *ev)
{
    return ActionHandler::findMouseAction(ev->button, ev->state, MOUSE_EVENT_PRESS,
                                          Config::instance()->getEdgeListFromPosition(_edge));
}

/**
 * Button release event handler, gets actions from EdgeList on _edge.
 */
ActionEvent*
EdgeWO::handleButtonRelease(XButtonEvent *ev)
{
    // Make sure the release is on the actual window. This probably
    // could be done smarter.
    if (ev->x_root < _gm.x || ev->x_root > static_cast<int>(_gm.x + _gm.width)
        || ev->y_root < _gm.y || ev->y_root > static_cast<int>(_gm.y + _gm.height)) {
        return 0;
    }

    MouseEventType mb = MOUSE_EVENT_RELEASE;

    // first we check if it's a double click
    if (PScreen::instance()->isDoubleClick(ev->window, ev->button - 1, ev->time,
                                           Config::instance()->getDoubleClickTime())) {
        PScreen::instance()->setLastClickID(ev->window);
        PScreen::instance()->setLastClickTime(ev->button - 1, 0);

        mb = MOUSE_EVENT_DOUBLE;
    } else {
        PScreen::instance()->setLastClickID(ev->window);
        PScreen::instance()->setLastClickTime(ev->button - 1, ev->time);
    }

    return ActionHandler::findMouseAction(ev->button, ev->state, mb,
                                          Config::instance()->getEdgeListFromPosition(_edge));
}
