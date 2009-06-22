/*
 * $Id$
 *
 * ede-bug-report, a tool to report bugs on EDE bugzilla instance
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2009 EDE Authors.
 *
 * This program is licensed under terms of the 
 * GNU General Public License version 2 or newer.
 * See COPYING for details.
 */

#ifndef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Button.H>

#include <edelib/Window.h>
#include <edelib/Nls.h>
#include <edelib/MessageBox.h>

#include "Bugzilla.h"
#include "BugImage.h"
#include "icons/bug.xpm"

#define EDE_BUGZILLA_URL   "http://bugs.equinox-project.org/xmlrpc.cgi"

/* must match to existing user */
#define EDE_BUGZILLA_USER  "ede-bugs@lists.sourceforge.net"
#define EDE_BUGZILLA_PASS  "edebugs2709"

EDELIB_NS_USING(String)
EDELIB_NS_USING(alert)
EDELIB_NS_USING(message)

static Fl_Input        *bug_title_input;
static Fl_Input        *email_input;
static Fl_Text_Buffer  *text_buf;

/* check if string has spaces */
static bool empty_entry(const char *en) {
	if(!en)
		return true;

	for(const char *p = en; *p; p++) {
		if(*p != ' ' && *p != '\t' && *p != '\n' && *p != '\r')
			return false;
	}

	return true;
}

static void close_cb(Fl_Widget*, void *w) {
	edelib::Window *win = (edelib::Window *)w;
	win->hide();
}

static void send_cb(Fl_Widget*, void *w) {
	const char   *txt;
	String        title;
	BugzillaData *bdata;

	if(empty_entry(bug_title_input->value())) {
		alert(_("Report title is missing"));
		return;
	}

	if(empty_entry(email_input->value())) {
		alert(_("Email address is missing"));
		return;
	}

	txt = text_buf->text();
	if(empty_entry(txt)) {
		alert(_("Detail problem description is missing"));
		goto cleanup;
	}

	/* 
	 * construct a subject header, so we knows it came from report tool
	 * BRT - Bug Report Tool
	 */
	title = "[BRT] ";
	title += bug_title_input->value();

	free((void *) txt);

	/* Prepend content in the buffer. Due this operation, text is inserted in reversed order of showing */
	text_buf->insert(0, "Detail description:\n");
	text_buf->insert(0, "\n\n");
	text_buf->insert(0, email_input->value());
	text_buf->insert(0, "Reporter:\n");
	text_buf->insert(0, "This issue was reported via EDE Bug Report Tool (BRT).\n\n");

	txt = text_buf->text();

	Fl::check();

	bdata = bugzilla_new(EDE_BUGZILLA_URL);
	if(!bdata) {
		alert(_("Unable to initialize bugzilla interface"));
		goto cleanup;
	}

	if(bugzilla_login(bdata, EDE_BUGZILLA_USER, EDE_BUGZILLA_PASS) < 0) {
		alert(_("Unable to login on EDE Bugzilla. Please check your connection and try again.\n\nIf this problem persists, please manualy report issue on http://bugs.equinox-project.org"));
		goto cleanup;
	}

	/* NOTE: default values must match those values in EDE Bugzilla instance */
	if(bugzilla_submit_bug(bdata, "ede", "general", title.c_str(), "unspecified", txt, "All", "All", "P5", "normal") < 0) {
		alert(_("Unable to properly submit the data. Please try again"));

		/* Must jump to the cleanup since xmlrpc will assert. */
		goto cleanup;
	}

	bugzilla_logout(bdata);

	message(_("Report was sent successfully. Thank you!"));
	/* close the window */
	close_cb(0, w);
cleanup:
	bugzilla_free(bdata);
	free((void *) txt);
}

int main(int argc, char** argv) {
	edelib::Window *win = new edelib::Window(480, 365, _("EDE Bug Reporting Tool"));
	win->begin();
    	Fl_Box *image_box = new Fl_Box(10, 10, 60, 59);
		image_box->image(image_bug);

		Fl_Box *title_box = new Fl_Box(80, 10, 390, 30, _("EDE Bug Reporting Tool"));
		title_box->labelfont(1);
		title_box->labelsize(14);
		title_box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

		Fl_Box *description_box = new Fl_Box(80, 45, 390, 95, _("To help us to improve the future EDE versions, please describe the problem with much details as possible.\n\nNote: valid email address is required, so developers could contact you for more informations if necessary."));
		description_box->align(133|FL_ALIGN_INSIDE);

		bug_title_input = new Fl_Input(10, 165, 225, 25, _("Short and descriptive title:"));
		bug_title_input->align(FL_ALIGN_TOP_LEFT);

		email_input = new Fl_Input(240, 165, 230, 25, _("Your email address:"));
		email_input->align(FL_ALIGN_TOP_LEFT);

		Fl_Text_Editor* te = new Fl_Text_Editor(10, 215, 460, 105, _("Detail description of the problem:"));
		te->align(FL_ALIGN_TOP_LEFT);
		/* wrap length of the text */
		te->wrap_mode(1, 80);

		text_buf = new Fl_Text_Buffer();
		te->buffer(text_buf);

		Fl_Button *send = new Fl_Button(285, 330, 90, 25, _("&Send"));
		send->callback(send_cb, win);

		Fl_Button *cancel = new Fl_Button(380, 330, 90, 25, _("&Cancel"));
		cancel->callback(close_cb, win);

	win->window_icon(bug_xpm);
	win->show(argc, argv);
	return Fl::run();
}
