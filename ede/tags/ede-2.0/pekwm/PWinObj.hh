//
// PWinObj.hh for pekwm
// Copyright © 2003-2009 Claes Nästen <me@pekdon.net>
//
// This program is licensed under the GNU GPL.
// See the LICENSE file for more information.
//

#ifndef _PWIN_OBJ_HH_
#define _PWIN_OBJ_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <algorithm>
#include <map>
#include <vector>

#include "pekwm.hh"
#include "Action.hh"
#include "Observable.hh"

//! @brief X11 Window wrapper class.
class PWinObj : public Observable
{
public:
    //! @brief PWinObj inherited types.
    enum Type {
        WO_FRAME = (1<<1), //!< Frame type.
        WO_CLIENT = (1<<2), //!< Client type.
        WO_MENU = (1<<3), //!< PMenu type.
        WO_DOCKAPP = (1<<4), //!< DockApp type.
        WO_SCREEN_EDGE = (1<<5), //!< ScreenEdge type.
        WO_SCREEN_ROOT = (1<<6), //!< PWinObj type for root Window.
        WO_CMD_DIALOG = (1<<7), //!< CmdDialog type.
        WO_STATUS = (1<<8), //!< StatusWindow type.
        WO_WORKSPACE_INDICATOR = (1<<9), //!< WorkspaceIndicator type.
        WO_SCREEN_HINT = (1<<10), /**< Invisible hint window. */
        WO_SEARCH_DIALOG = (1<<11), //!< SearchDialog type
        WO_NO_TYPE = 0 //!< No type.
    };

    PWinObj(void);
    virtual ~PWinObj(void);

    //! @brief Set the X Display structure.
    static void setDisplay(Display *dpy) { _dpy = dpy; }

    //! @brief Returns the focused PWinObj.
    static inline PWinObj *getFocusedPWinObj(void) { return _focused_wo; }
    //! @brief Returns the PWinObj representing the root Window.
    static inline PWinObj *getRootPWinObj(void) {	return _root_wo; }
    //! @brief Sets the focused PWinObj.
    static inline void setFocusedPWinObj(PWinObj *wo) {	_focused_wo = wo; }
    //! @brief Sets the PWinObj representing the root Window.
    static inline void setRootPWinObj(PWinObj *wo) { _root_wo = wo; }
    //! @brief Checks if focused window is of type
    static inline bool isFocusedPWinObj(Type type) {
        return _focused_wo ? _focused_wo->getType() == type : false;
    }

    //! @brief Searches for the PWinObj matching Window win.
    //! @param win Window to match PWinObjs against.
    //! @return PWinObj pointer on match, else 0.
    static inline PWinObj *findPWinObj(Window win) {
        std::map<Window, PWinObj*>::iterator it(_wo_map.find(win));
        return (it != _wo_map.end()) ? it->second : 0;
    }

    //! @brief Searches in PWinObj list if PWinObj wo exists.
    //! @param wo PWinObj to search for.
    //! @return true if found, else false.
    static inline bool windowObjectExists(PWinObj *wo) {
        std::vector<PWinObj*>::iterator it =
            std::find(_wo_list.begin(), _wo_list.end(), wo);
        if (it != _wo_list.end())
            return true;
        return false;
    }

    //! @brief Return Window this PWinObj represents.
    inline Window getWindow(void) const { return _window; }
    //! @brief Sets Window this PWinObj represents.
    inline void setWindow(Window window) { _window = window; }
    //! @brief Returns parent PWinObj.
    inline PWinObj *getParent(void) const { return _parent; }
    //! @brief Sets parent PWinObj.
    inline void setParent(PWinObj *wo) { _parent = wo; }
    //! @brief Returns type of PWinObj.
    inline Type getType(void) const { return _type; }

    void addChildWindow(Window win);
    void removeChildWindow(Window win);

    //! @brief Returns x coordinate of PWinObj.
    inline int getX(void) const { return _gm.x; }
    //! @brief Returns y coordinate of PWinObj.
    inline int getY(void) const { return _gm.y; }
    //! @brief Returns right edge x coordinate of PWinObj.
    inline int getRX(void) const { return _gm.x + _gm.width; }
    //! @brief Returns bottom edge y coordinate of PWinObj.
    inline int getBY(void) const { return _gm.y + _gm.height; }

    //! @brief Returns width of PWinObj.
    inline uint getWidth(void) const { return _gm.width; }
    //! @brief Returns height of PWinObj:
    inline uint getHeight(void) const { return _gm.height; }

    //! @brief Returns workspace PWinObj is on.
    inline uint getWorkspace(void) const { return _workspace; }
    /** @brief Returns layer PWinObj is in. */
    inline unsigned int getLayer(void) const { return _layer; }

    //! @brief Returns mapped state of PWinObj.
    inline bool isMapped(void) const { return _mapped; }
    //! @brief Returns iconofied state of PWinObj.
    inline bool isIconified(void) const { return _iconified; }
    //! @brief Returns hidden state of PWinObj.
    inline bool isHidden(void) const { return _hidden; }
    //! @brief Returns focused state of PWinObj.
    inline bool isFocused(void) const { return _focused; }
    //! @brief Returns sticky state of PWinObj.
    inline bool isSticky(void) const { return _sticky; }
    //! @brief Returns Focusable state of PWinObj.
    inline bool isFocusable(void) const { return _focusable; }

#ifdef OPACITY
    //! @brief Returns transparency state of PWinObj
    inline bool isOpaque(void) const { return _opaque; }
    void setOpacity(uint focused, uint unfocused, bool enabled=true);
    inline void setOpacity(uint value) { setOpacity(value, value); }
    inline void setOpacity(PWinObj *child) { setOpacity(child->_opacity.focused, child->_opacity.unfocused, !child->_opaque); }
    void updateOpacity(void);
#endif // OPACITY
 
    // interface
    virtual void mapWindow(void);
    virtual void mapWindowRaised(void);
    virtual void unmapWindow(void);
    virtual void iconify(void);
    virtual void stick(void);

    virtual void move(int x, int y);
    virtual void resize(uint width, uint height);
    virtual void moveResize(int x, int y, uint width, uint height);
    //! @brief Raises PWinObj without respect of layer.
    virtual void raise(void) { XRaiseWindow(_dpy, _window); }
    //! @brief Lowers PWinObj without respect of layer.
    virtual void lower(void) { XLowerWindow(_dpy, _window); }

    virtual void setWorkspace(uint workspace);
    virtual void setLayer(uint layer);
    virtual void setFocused(bool focused);
    virtual void setSticky(bool sticky);
#ifdef OPACITY
    void setOpaque(bool opaque);
#endif // OPACITY
    /** Set focusable flag. */
    virtual void setFocusable(bool focusable) { _focusable = focusable; }
    virtual void setHidden(bool hidden);

    virtual void giveInputFocus(void);
    virtual void reparent(PWinObj *parent, int x, int y);

    virtual bool getSizeRequest(Geometry &request);

    // event interface

    //! @brief Handles button press events, always return 0.
    virtual ActionEvent *handleButtonPress(XButtonEvent *ev) { return 0; }
    //! @brief Handles button release events, always return 0.
    virtual ActionEvent *handleButtonRelease(XButtonEvent *ev) { return 0; }
    //! @brief Handles key press events, always return 0.
    virtual ActionEvent *handleKeyPress(XKeyEvent *ev) { return 0; }
    //! @brief Handles key release vents, always return 0.
    virtual ActionEvent *handleKeyRelease(XKeyEvent *ev) { return 0; }
    //! @brief Handles motion events, always return 0.
    virtual ActionEvent *handleMotionEvent(XMotionEvent *ev) { return 0; }
    //! @brief Handles enter events, always return 0.
    virtual ActionEvent *handleEnterEvent(XCrossingEvent *ev) { return 0; }
    //! @brief Handles leave events, always return 0.
    virtual ActionEvent *handleLeaveEvent(XCrossingEvent *ev) { return 0; }
    //! @brief Handles expose events, always return 0.
    virtual ActionEvent *handleExposeEvent(XExposeEvent *ev) { return 0; }

    //! @brief Handles handle map request events, always return 0.
    virtual ActionEvent *handleMapRequest(XMapRequestEvent *ev) { return 0; }
    //! @brief Handles handle unmap events, always return 0.
    virtual ActionEvent *handleUnmapEvent(XUnmapEvent *ev) { return 0; }

    // operators

    //! @brief Operator matching against Window PWinObj represents..
    virtual bool operator == (const Window &window) {
        return (_window == window);
    }
    //! @brief Operator matching against Window PWinObj represents.
    virtual bool operator != (const Window &window) {
        return (_window != window);
    }

    // other window commands

    //! @brief Clears Window causing a redraw.
    inline void clear(void) { XClearWindow(_dpy, _window); }
    //! @brief Sets Window background colour.
    inline void setBackground(long pixel) {
        XSetWindowBackground(_dpy, _window, pixel);
    }
    //! @brief Sets Window background pixmap.
    inline void setBackgroundPixmap(Pixmap pm) {
        XSetWindowBackgroundPixmap(_dpy, _window, pm);
    }

protected:
    static void woListAdd(PWinObj *wo);
    static void woListRemove(PWinObj *wo);

protected:
    static Display *_dpy; //!< Display PWinObj is on.
    Window _window; //!< Window PWinObj represents.
    PWinObj *_parent; //!< Parent PWinObj.

    Type _type; //!< Type of PWinObj.
#ifdef OPACITY
    // Opacity information
    class Opacity {
    public:
        Opacity(void)
            : current(EWMH_OPAQUE_WINDOW),
              focused(EWMH_OPAQUE_WINDOW),
              unfocused(EWMH_OPAQUE_WINDOW) { }
        uint current, focused, unfocused;
    } _opacity;
#endif // OPACITY

    Geometry _gm; //!< Geometry of PWinObj (always in absolute coordinates).
    uint _workspace; //!< Workspace PWinObj is on.
    unsigned int _layer; //!< Layer PWinObj is in.
    bool _mapped; //!< Mapped state of PWinObj.
    bool _iconified; //!< Iconified state of PWinObj.
    bool _hidden; //!< Hidden state of PWinObj.
    bool _focused; //!< Focused state of PWinObj.
    bool _sticky; //!< Sticky state of PWinObj.
    bool _focusable; //!< Focusable state of PWinObj.
#ifdef OPACITY
    bool _opaque; //!< Opaque set state of PWinObj
#endif // OPACITY
    static PWinObj *_root_wo; //!< Static root PWinObj pointer.
    static PWinObj *_focused_wo; //!< Static focused PWinObj pointer.
    static std::vector<PWinObj*> _wo_list; //!< List of PWinObjs.
    static std::map<Window, PWinObj*> _wo_map; //!< Mapping of Window to PWinObj
};

#endif // _PWIN_OBJ_HH_
