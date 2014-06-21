//
// CmdDialog.hh for pekwm
// Copyright © 2009 Claes Nästén <me@pekdon.net>
//
// This program is licensed under the GNU GPL.
// See the LICENSE file for more information.
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <list>
#include <cwchar>
#include <cwctype>

#include "InputDialog.hh"
#include "KeyGrabber.hh"
#include "PScreen.hh"
#include "PixmapHandler.hh"
#include "ScreenResources.hh"
#include "Workspaces.hh"

extern "C" {
#include <X11/Xutil.h>

#ifdef __minix
# define XK_MISCELLANY
# include <X11/keysymdef.h>
#endif
}

using std::list;
using std::map;
using std::wstring;
using std::iswprint;

map<KeySym, wchar_t> InputDialog::_keysym_map;

/**
 * InputDialog constructor.
 */
InputDialog::InputDialog(Theme *theme, const std::wstring &title)
    : PDecor(theme, "INPUTDIALOG"), PWinObjReference(0),
      _data(theme->getCmdDialogData()),
      _pixmap_bg(None), _pos(0), _buf_off(0), _buf_chars(0)
{
    // PWinObj attributes
    setLayer(LAYER_NONE); // hack, goes over LAYER_MENU
    _hidden = true; // don't care about it when changing worskpace etc

    if (! _keysym_map.size()) {
        reloadKeysymMap();
    }

    // Add action to list, going to be used from close and exec
    ::Action action;
    _ae.action_list.push_back(action);

    titleAdd(&_title);
    titleSetActive(0);
    setTitle(title);

    _text_wo = new PWinObj;
    XSetWindowAttributes attr;
    attr.override_redirect = false;
    attr.event_mask = ButtonPressMask|ButtonReleaseMask|ButtonMotionMask|
    FocusChangeMask|KeyPressMask|KeyReleaseMask;
    _text_wo->setWindow(XCreateWindow(_dpy, _window,
                                   0, 0, 1, 1, 0,
                                   CopyFromParent, InputOutput, CopyFromParent,
                                   CWOverrideRedirect|CWEventMask, &attr));

    addChild(_text_wo);
    addChildWindow(_text_wo->getWindow());
    activateChild(_text_wo);
    _text_wo->mapWindow();

    // setup texture, size etc
    loadTheme();

    Workspaces::instance()->insert(this);
    woListAdd(this);
    _wo_map[_window] = this;
}

/**
 * InputDialog destructor.
 */
InputDialog::~InputDialog(void)
{
    Workspaces::instance()->remove(this);
    _wo_map.erase(_window);
    woListRemove(this);

    // Free resources
    if (_text_wo) {
        _child_list.remove(_text_wo);
        removeChildWindow(_text_wo->getWindow());
        XDestroyWindow(_dpy, _text_wo->getWindow());
        delete _text_wo;
    }

    unloadTheme();
}

void
InputDialog::reloadKeysymMap(void)
{
    _keysym_map.clear();

    addKeysymToKeysymMap(XK_KP_0, L'0');
    addKeysymToKeysymMap(XK_KP_1, L'1');
    addKeysymToKeysymMap(XK_KP_2, L'2');
    addKeysymToKeysymMap(XK_KP_3, L'3');
    addKeysymToKeysymMap(XK_KP_4, L'4');
    addKeysymToKeysymMap(XK_KP_5, L'5');
    addKeysymToKeysymMap(XK_KP_6, L'6');
    addKeysymToKeysymMap(XK_KP_7, L'7');
    addKeysymToKeysymMap(XK_KP_8, L'8');
    addKeysymToKeysymMap(XK_KP_9, L'9');
}

void
InputDialog::addKeysymToKeysymMap(KeySym keysym, wchar_t chr)
{
    Display *dpy = PScreen::instance()->getDpy();

    int keysyms_per_keycode;
    KeyCode keycode = XKeysymToKeycode(dpy, keysym);
    KeySym *keysyms = XGetKeyboardMapping(dpy, keycode, 1, &keysyms_per_keycode);

    for (int i = 0; i < keysyms_per_keycode; i++) {
        if (keysyms[i] != NoSymbol) {
            _keysym_map[keysyms[i]] = chr;
        }
    }

    XFree(keysyms);
}

/**
 * Handles ButtonPress, moving the text cursor
 */
ActionEvent*
InputDialog::handleButtonPress(XButtonEvent *ev)
{
    if (*_text_wo == ev->window) {
        // FIXME: move cursor
        return 0;
    } else {
        return PDecor::handleButtonPress(ev);
    }
}

/**
 * Handles KeyPress, editing the buffer
 */
ActionEvent*
InputDialog::handleKeyPress(XKeyEvent *ev)
{
    ActionEvent *c_ae, *ae = 0;

    if ( (c_ae = KeyGrabber::instance()->findAction(ev, _type)) ) {
        list<Action>::iterator it(c_ae->action_list.begin());
        for (; it != c_ae->action_list.end(); ++it) {
            switch (it->getAction()) {
            case INPUT_INSERT:
                bufAdd(ev);
                completeReset();
                break;
            case INPUT_REMOVE:
                bufRemove();
                break;
            case INPUT_CLEAR:
                bufClear();
                completeReset();
                break;
            case INPUT_CLEARFROMCURSOR:
                bufKill();
                completeReset();
                break;
            case INPUT_EXEC:
                ae = exec();
                break;
            case INPUT_CLOSE:
                ae = close();
                break;
            case INPUT_COMPLETE:
                complete();
                break;
            case INPUT_COMPLETE_ABORT:
                completeAbort();
                break;
            case INPUT_CURS_NEXT:
                bufChangePos(1);
                completeReset();
                break;
            case INPUT_CURS_PREV:
                bufChangePos(-1);
                completeReset();
                break;
            case INPUT_CURS_BEGIN:
                _pos = 0;
                completeReset();
                break;
            case INPUT_CURS_END:
                _pos = _buf.size();
                completeReset();
                break;
            case INPUT_HIST_NEXT:
                histNext();
                completeReset();
                break;
            case INPUT_HIST_PREV:
                histPrev();
                completeReset();
                break;
            case INPUT_NO_ACTION:
            default:
                // do nothing, shouldn't happen
                break;
            };
        }

        // something ( most likely ) changed, redraw the window
        if (! ae) {
            bufChanged();
            render();
        }
    }

    return ae;
}

/**
 * Handles ExposeEvent, redraw when ev->count == 0
 */
ActionEvent*
InputDialog::handleExposeEvent(XExposeEvent *ev)
{
    if (ev->count > 0) {
        return 0;
    }
    render();
    return 0;
}

/**
 * Maps the InputDialog center on the PWinObj it executes actions on.
 *
 * @param buf Buffer content.
 * @param focus Give input focus if true.
 * @param wo_ref PWinObj reference, defaults to 0 which does not update.
 */
void
InputDialog::mapCentered(const std::string &buf, bool focus, PWinObj *wo_ref)
{
    // Setup data
    _hist_it = _hist_list.end();

    _buf = Util::to_wide_str(buf);
    _pos = _buf.size();
    bufChanged();

    // Update position
    moveCentered(wo_ref);

    // Map and render
    PDecor::mapWindowRaised();
    render();

    // Give input focus if requested
    if (focus) {
        giveInputFocus();
    }
}

/**
 * Moves to center of wo.
 *
 * @param wo PWinObj to center on.
 */
void
InputDialog::moveCentered(PWinObj *wo)
{
    // Fallback wo on root.
    if (! wo) {
        wo = PWinObj::getRootPWinObj();
    }

    // Make sure position is inside head.
    Geometry head;
    uint head_nr = PScreen::instance()->getNearestHead(wo->getX() + (wo->getWidth() / 2),
                                                     wo->getY() + (wo->getHeight() / 2));
    PScreen::instance()->getHeadInfo(head_nr, head);

    // Make sure X is inside head.
    int new_x = wo->getX() + (static_cast<int>(wo->getWidth()) - static_cast<int>(_gm.width)) / 2;
    if (new_x < head.x) {
        new_x = head.x;
    } else if ((new_x + _gm.width) > (head.x + head.width)) {
        new_x = head.x + head.width - _gm.width;
    }

    // Make sure Y is inside head.
    int new_y = wo->getY() + (static_cast<int>(wo->getHeight()) - static_cast<int>(_gm.height)) / 2;
    if (new_y < head.y) {
        new_y = head.y;
    } else if ((new_y + _gm.height) > (head.y + head.height)) {
        new_y = head.y + head.height - _gm.height;
    }

    // Update position.
    move(new_x, new_y);    
}

/**
 * Sets title of decor
 */
void
InputDialog::setTitle(const std::wstring &title)
{
    _title.setReal(title);
}

/**
 * Maps window, overloaded to refresh content of window after mapping.
 */
void
InputDialog::mapWindow(void)
{
    if (! _mapped) {
        // Correct size for current head before mapping
        updateSize();
    
        PDecor::mapWindow();
        render();
    }
}

/**
 * Sets background and size
 */
void
InputDialog::loadTheme(void)
{
    _data = _theme->getCmdDialogData();
    updateSize();
    updatePixmapSize();
}

/**
 * Frees resources
 */
void
InputDialog::unloadTheme(void)
{
    ScreenResources::instance()->getPixmapHandler()->returnPixmap(_pixmap_bg);
}

/**
 * Renders _buf onto _text_wo
 */
void
InputDialog::render(void)
{
    _text_wo->clear();

    // draw buf content
    _data->getFont()->setColor(_data->getColor());

    _data->getFont()->draw(_text_wo->getWindow(), _data->getPad(PAD_LEFT), _data->getPad(PAD_UP),
                          _buf.c_str() + _buf_off, _buf_chars);

    // draw cursor
    uint pos = _data->getPad(PAD_LEFT);
    if (_pos > 0) {
        pos = _data->getFont()->getWidth(_buf.c_str() + _buf_off,  _pos - _buf_off) + 1;
    }

    _data->getFont()->draw(_text_wo->getWindow(), pos, _data->getPad(PAD_UP), L"|");
}

/**
 * Generates ACTION_CLOSE closing dialog.
 *
 * @return Pointer to ActionEvent.
 */
ActionEvent*
InputDialog::close(void)
{
    _ae.action_list.back().setAction(ACTION_NO);
    return &_ae;
}

/**
 * Default implementation, fills in _buf_on_complete to safely run
 * completeAbort.
 */
void
InputDialog::complete(void)
{
    _buf_on_complete = _buf;
    _pos_on_complete = _pos;
}

/**
 * Restore buffer and clear completion buffers.
 */
void
InputDialog::completeAbort(void)
{
    if (_buf_on_complete.size()) {
        _buf = _buf_on_complete;
        _pos = _pos_on_complete;
    }

    completeReset();
}

/**
 * Clear the completion buffer.
 */
void
InputDialog::completeReset(void)
{
    // Old gcc doesn't know about .clear()
    _buf_on_complete = _buf_on_complete_result = L"";
    _pos_on_complete = 0;
}

/**
 * Adds char to buffer
 */
void
InputDialog::bufAdd(XKeyEvent *ev)
{
    KeySym keysym;
    char c_return[64];
    memset(c_return, '\0', sizeof(c_return));

    XLookupString(ev, c_return, sizeof(c_return), &keysym, 0);
    if (_keysym_map.count(keysym)) {
        _buf.insert(_buf.begin() + _pos++, _keysym_map[keysym]);
    } else {
        // Add wide string to buffer counting position
        wstring buf_ret(Util::to_wide_str(c_return));
        for (unsigned int i = 0; i < buf_ret.size(); ++i) {
            if (iswprint(buf_ret[i])) {
                _buf.insert(_buf.begin() + _pos++, buf_ret[i]);
            }
        }
    }
}

/**
 * Removes char from buffer
 */
void
InputDialog::bufRemove(void)
{
    if ((_pos > _buf.size()) || (_pos == 0) || (_buf.size() == 0)) {
        return;
    }

    _buf.erase(_buf.begin() + --_pos);
}

/**
 * Clears the buffer, resets status
 */
void
InputDialog::bufClear(void)
{
    _buf = _buf_on_complete = _buf_on_complete_result = L""; // old gcc doesn't know about .clear()
    _pos = _pos_on_complete = _buf_off = _buf_chars = 0;
}

/**
 * Removes buffer content after cursor position
 */
void
InputDialog::bufKill(void)
{
    _buf.resize(_pos);
}

/**
 * Moves the marker
 */
void
InputDialog::bufChangePos(int off)
{
    if ((signed(_pos) + off) < 0) {
        _pos = 0;
    } else if (unsigned(_pos + off) > _buf.size()) {
        _pos = _buf.size();
    } else {
        _pos += off;
    }
}

/**
 * Recalculates, _buf_off and _buf_chars
 */
void
InputDialog::bufChanged(void)
{
    PFont *font =  _data->getFont(); // convenience

    // complete string doesn't fit in the window OR
    // we don't fit in the first set
    if ((_pos > 0)
      && (font->getWidth(_buf.c_str()) > _text_wo->getWidth())
      && (font->getWidth(_buf.c_str(), _pos) > _text_wo->getWidth())) {

        // increase position until it all fits
        for (_buf_off = 0; _buf_off < _pos; ++_buf_off) {
            if (font->getWidth(_buf.c_str() + _buf_off, _buf.size() - _buf_off)
                    < _text_wo->getWidth()) {
                break;
            }
        }

        _buf_chars = _buf.size() - _buf_off;
    } else {
        _buf_off = 0;
        _buf_chars = _buf.size();
    }
}

/**
 * Sets the buffer to the next item in the history.
 */
void
InputDialog::histNext(void)
{
    if (_hist_it == _hist_list.end()) {
        return; // nothing to do
    }

    // get next item, if at the end, restore the edit buffer
    ++_hist_it;
    if (_hist_it == _hist_list.end()) {
        _buf = _hist_new;
    } else {
        _buf = *_hist_it;
    }

    // move cursor to the end of line
    _pos = _buf.size();
}

/**
 * Sets the buffer to the previous item in the history.
 */
void
InputDialog::histPrev(void)
{
    if (_hist_it == _hist_list.begin()) {
        return; // nothing to do
    }

    // save item so we can restore the edit buffer later
    if (_hist_it == _hist_list.end()) {
        _hist_new = _buf;
    }

    // get prev item
    _buf = *(--_hist_it);

    // move cursor to the end of line
    _pos = _buf.size();
}

/**
 * Update command dialog size for view on current head.
 */
void
InputDialog::updateSize(void)
{
    // Resize the child window and update the size depending.
    uint old_width = _gm.width;

    unsigned int width, height;
    getInputSize(width, height);
    resizeChild(width, height);

    // If size was updated, replace the texture and recalculate display
    // buffer.
    if (old_width != _gm.width) {
        updatePixmapSize();
        bufChanged();
    }
}

/**
 * Update background pixmap size and redraw.
 */
void
InputDialog::updatePixmapSize(void)
{
    // Get new pixmap and render texture
    PixmapHandler *pm = ScreenResources::instance()->getPixmapHandler();
    pm->returnPixmap(_pixmap_bg);
    _pixmap_bg = pm->getPixmap(_text_wo->getWidth(), _text_wo->getHeight(), PScreen::instance()->getDepth());

    _data->getTexture()->render(_pixmap_bg, 0, 0, _text_wo->getWidth(), _text_wo->getHeight());
    _text_wo->setBackgroundPixmap(_pixmap_bg);
    _text_wo->clear();
}

/**
 * Get size of the text input widget.
 *
 * @param width Fill in width.
 * @param height Fill in height.
 */
void
InputDialog::getInputSize(unsigned int &width, unsigned int &height)
{
    Geometry head;
    PScreen::instance()->getHeadInfo(PScreen::instance()->getNearestHead(_gm.x, _gm.y), head);

    width = head.width / 3;
    height = _data->getFont()->getHeight() + _data->getPad(PAD_UP) + _data->getPad(PAD_DOWN);
}
