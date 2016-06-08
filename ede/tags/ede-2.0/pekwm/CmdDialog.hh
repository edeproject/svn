//
// CmdDialog.hh for pekwm
// Copyright © 2004-2009 Claes Nästén <me{@}pekdon{.}net>
//
// This program is licensed under the GNU GPL.
// See the LICENSE file for more information.
//

#ifndef _CMD_DIALOG_HH_
#define _CMD_DIALOG_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "Action.hh"
#include "Completer.hh"
#include "InputDialog.hh"
#include "Theme.hh"

#include <list>
#include <string>

//! @brief CmdDialog presenting the user with an pekwm action command line.
class CmdDialog : public InputDialog
{
public:
    CmdDialog(Theme *theme);
    virtual ~CmdDialog(void);

    void unmapWindow(void);

    virtual void mapCentered(const std::string &buf, bool focus, PWinObj *wo_ref);

private:
    void render(void);

    virtual ActionEvent *exec(void);
    virtual void complete(void);

private:
    Completer _completer; /**< Completer used completing actions. */
    complete_list _complete_list; /**< List of completions found by completer. */
    complete_it _complete_it; /**< Iterator used to step between completions. */

    int _exec_count; /**< Number of CmdDialog has run exec since last history save. */ 
};

#endif // _CMD_DIALOG_HH_
