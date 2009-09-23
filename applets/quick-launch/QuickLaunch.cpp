#include "Applet.h"
#include <FL/Fl_Input.H>

#include <edelib/Debug.h>
#include <edelib/Nls.h>
#include <edelib/Run.h>

EDELIB_NS_USING(run_async)

static void enter_cb(Fl_Widget*, void *o);

class QuickLaunch : public Fl_Input {
public:
	QuickLaunch() : Fl_Input(0, 0, 100, 25) {
		when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
		callback(enter_cb, this);
		tooltip(_("Enter command to be executed"));
	}
};

static void enter_cb(Fl_Widget*, void *o) {
	QuickLaunch *ql = (QuickLaunch*)o;
	if(ql->value())
		run_async("ede-launch %s", ql->value());
}

EDE_PANEL_APPLET_EXPORT (
 QuickLaunch, 
 EDE_PANEL_APPLET_OPTION_ALIGN_FAR_LEFT,
 "Quick Launch applet",
 "0.1",
 "empty",
 "Sanel Zukan"
)
