/*
 * $Id$
 *
 * Elauncher, tool for running apps
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include "elauncher.h"
#include <edeconf.h>

// globals used in forking
int fds_[3];
Fl_String cmd_;
int pid_;

// command-line parameters
bool param_root = false;
bool param_secure = false;
bool param_term = false;


// Shows a generic window for displaying output stream
void output_window_close(Fl_Widget *w)
{
    w->window()->hide();
}

void output_window(char *title, char *content)
{
    int height=0;
    Fl_Text_Buffer buffer;
    buffer.text(content);
    
    for (unsigned i=0;i<strlen(content);i++) 
        if (content[i] == '\n') height+=10;
    height+=63;
    if (height>550) height=550;
    if (height<100) height=100;
    
    Fl_Window window(500, height);
    window.label(title);
        
    Fl_Text_Display message(0, 0, 500, height-23, content);
    window.resizable(message);
    message.buffer(buffer);
    
    Fl_Button* button;
    button = new Fl_Return_Button(410, height-23, 80, 23, _("&Ok"));
    button->callback(output_window_close);
    window.hotspot(button);
    window.focus(button);
    window.end();
    window.exec();
}

// Error message window
static Fl_Image error_pix((const char **)error_xpm);

void cb_errOk(Fl_Widget *w) {
  w->window()->hide();
}

void errormessage(char *part1, char *part2, char *part3)
{
	Fl_Window* w;
	{
		Fl_Window* o = new Fl_Window(350, 100, _("Error"));
		w = o;
		o->shortcut(0xff1b);
		{
			Fl_Button* o = new Fl_Button(260, 65, 80, 22, _("&OK"));
			o->callback((Fl_Callback*)cb_errOk); 
		}
		{
			Fl_Box* o = new Fl_Box(60, 5, 125, 16, part1);
			o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
		}
		{
			Fl_Box* o = new Fl_Box(90, 20, 240, 16, part2);
			o->label_font(fl_fonts+1);
			o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
		}
		{
			Fl_Box* o = new Fl_Box(60, 35, 280, 30, part3);
			o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
		}
		{
			Fl_Box* o = new Fl_Box(15, 15, 35, 35, "");
			o->image(error_pix);
		}
		o->end();
	}
	w->exec();
	return;
}


// Depending on exit status, show some nice dialogs
void process_output_status(int child_val)
{
	Fl_String messages1[256], messages2[256];

	// FIXME: do we still need this init?
	for (int i=0;i<256;i++) { messages1[i] = ""; messages2[i] = ""; }

	messages1[199] = _("Program not found:");
	messages2[199] = _("Perhaps it is not installed properly. Check your $PATH value.");
//	messages1[14] = _("Segmentation fault in child process:");
//	messages2[14] = _("");
	messages1[13] = _("File is not executable:");
	messages1[13] = _("");

	if (!(messages1[child_val] == "")) {
		// we have special message for this status
		errormessage(messages1[child_val],cmd_,messages2[child_val]);
        } else {
		fprintf(stderr, _("Elauncher: child's exited normally with status %d\n"), child_val);

		if (child_val>0) {
			// unknown status, display stdout & stderr
			char buffer[255];
			char output[65535];
			int bytes;

			bool changed=false;
			strcpy(output,"");
			while ((bytes = read(fds_[1], buffer, sizeof(buffer)))>0) {
				buffer[bytes]='\0';
				strcat(output, buffer);
				changed=true;
			}
			if (changed) output_window(_("Standard output"),output);
            
			changed=false;
			strcpy(output,"");
			while ((bytes = read(fds_[2], buffer, sizeof(buffer)))>0) {
				buffer[bytes]='\0';
				strcat(output, buffer);
				changed=true;
			}
			if (changed) output_window(_("Standard error output"),output);
		} // if (child_val>0)
	} // if (!messages...)
}

void notify_wm(void)
{
	fl_open_display();
	Atom _XA_EDE_WM_STARTUP_NOTIFY   = XInternAtom(fl_display, "_EDE_WM_STARTUP_NOTIFY", False);
	Atom _XA_EDE_WM_APP_STARTING     = XInternAtom(fl_display, "_EDE_WM_APP_STARTING", False);

	XEvent xevent;
	xevent.xclient.type = ClientMessage;
	xevent.xclient.message_type = _XA_EDE_WM_STARTUP_NOTIFY;
	xevent.xclient.window = RootWindow(fl_display, fl_screen);
	xevent.xclient.format = 32;
	xevent.xclient.data.l[0] = _XA_EDE_WM_APP_STARTING;

	XSendEvent(fl_display, RootWindow(fl_display, fl_screen), False,
			PropertyChangeMask, &xevent);

	XFlush(fl_display);
}

// unlike fl_start... this checks on the status of child
// and displays some nice dialogs
int ede_start_child_process(char *cmd) 
{
#ifndef _WIN32
    int	inpipe[2],				// Pipes for stdin
	outpipe[2],				// Pipes for stdout
	errpipe[2];				// Pipes for stderr

    pipe(inpipe);
    pipe(outpipe);
    pipe(errpipe);
    
    // convert cmd to null-terminated array of strings
    char **params = fl_split(cmd, " ", 255);
    cmd_ = params[0];
    
    // make sure this is not set wrongly by some parent
    // (e.g. eworkpanel)
    signal (SIGCHLD, SIG_DFL);


	// let wm know we started application
  	notify_wm(); 

    switch(pid_=fork()) {
    case 0:
        // The child process goes here...  Setup stdin, stdout, and stderr
        close(0);
        dup(inpipe[0]);
        close(inpipe[0]);
        close(inpipe[1]);

        close(1);
        dup(outpipe[1]);
        close(outpipe[0]);
        close(outpipe[1]);

        close(2);
        dup(errpipe[1]);
        close(errpipe[0]);
        close(errpipe[1]);

        // Execute the program
        execvp(cmd_,params);
       
        // Return if we can't execute
        if (errno == 2)
            _exit(199);   // Some programs use value 2 (tar)
        else
            _exit(errno);
    case -1:
        fprintf(stderr, _("Elauncher: fork() failed!"));

        // Close the pipes...
        close(inpipe[0]);
        close(inpipe[1]);

        close(outpipe[0]);
        close(outpipe[1]);

        close(errpipe[0]);
        close(errpipe[1]);
        break;
    default:
        // The successful parent comes here...  Save the parent-side pipe
        // file descriptors for later use, and close the child-side pipe fds...
        fds_[0] = inpipe[1];
        fds_[1] = outpipe[0];
        fds_[2] = errpipe[0];

        close(inpipe[0]);
        close(outpipe[1]);
        close(errpipe[1]);

	// FIXME: unfinished code
	// Add callback for attempt to read from STDIN
//	Fl::add_fd(fds_[0], FL_READ, (void (*)(int, void*)) stdin_cb, (void *)fds_[0]);

	// Wait for PID
        int status, child_val;
	if (waitpid(pid_, &status, 0) < 0) 
	{
		fprintf(stderr, _("Elauncher: waitpid failed\n"));
		return 1;
	}
	
	if (WIFEXITED(status))                /* did child exit normally? */
	{
		child_val = WEXITSTATUS(status); /* get child's exit status */
		process_output_status(child_val);
	} else {
		// Should we show a message box?
		fprintf (stderr, _("Elauncher: %s: child died unexpectedly!\n"),(char *)cmd_);
	}
	
	// Now is safe to close all fd's
	close(fds_[0]);
	close(fds_[1]);
	close(fds_[2]);
    } // switch
    
#endif
    return 0;
}


// Code copied over from former eRun
// Analyze command and, if URL, call appropriate application
// Otherwise just run the process
void run_resource(char *cmd) {
	char pRun[256];
	char browser[256];

	// look up default browser in config
	Fl_Config pGlobalConfig(fl_find_config_file("ede.conf", 0));
	pGlobalConfig.get("Web", "Browser", browser, 0, sizeof(browser));
	if(pGlobalConfig.error() && !browser) {
		strncpy(browser, "mozilla", sizeof(browser));
	}

	// split cmd to protocol and location
	char *protocol = strdup(cmd);
	char *location = strchr(protocol, ':');
	if (location) *(location++) = '\0';	// cut off at ':'

	// is cmd a proper URL?
	if((location) && (strchr(protocol, ' ') == NULL))
	{

		if (!strcasecmp(protocol,"file"))
		// use mimetypes
		{
			char *m_program;

			Fl_Config m_mimetypes(fl_find_config_file("mime.conf", 0));
			const char *m_ext = fl_file_getext(location);
			m_mimetypes.get(m_ext, "Exec", m_program);

			if (m_program)
				snprintf(pRun, sizeof(pRun)-1, "%s %s", m_program, location);
			else 
			{	// unknown extension
				char m_printout[256];
				snprintf(m_printout, sizeof(m_printout)-1, _("Unknown file type:\n\t%s\nTo open this file in 'appname' please use\n 'appname %s'"), location, location);
				fl_alert(m_printout);
				return;
			}
		}
		
		else if (!strcasecmp(protocol, "http") || !strcasecmp(protocol, "ftp"))
		{
			snprintf(pRun, sizeof(pRun)-1, "%s %s", browser, cmd);
		}
		
		// search engine urls
		else if (!strcasecmp(protocol, "gg"))
		{
			snprintf(pRun, sizeof(pRun)-1, "%s http://www.google.com/search?q=\"%s\"", browser, location);
		}
		
		else if (!strcasecmp(protocol, "leo"))
		{
			snprintf(pRun, sizeof(pRun)-1, "%s http://dict.leo.org/?search=\"%s\"", browser, location);
		}
		
		else if (!strcasecmp(protocol, "av"))
		{
			snprintf(pRun, sizeof(pRun)-1, "%s http://www.altavista.com/sites/search/web?q=\"%s\"", browser, location);
		}
		
		else	// Unkown URL type - let browser deal with it
		{
			snprintf(pRun, sizeof(pRun)-1, "%s %s", browser, cmd);
		}
	}
	else 
	// local executable 
	// TODO: parse the standard specification parameters to the executable if any exists in the *.desktop file.
	{
		if (param_secure) {
			char message[256];
			snprintf (message, sizeof(message)-1, _("You have requested to execute program %s via Elauncher. However, secure mode was enabled. Execution has been prevented."), cmd);
			fl_alert (message);
			exit(1);
		} else {
			snprintf(pRun, sizeof(pRun)-1, "%s", cmd);
		}
	}
	delete [] protocol;
	
	// Additional parameters
	if (param_term) {
		char termapp[256];
		pGlobalConfig.get("Terminal", "Terminal", termapp, 0, sizeof(termapp));
		char tmp[256];
		snprintf (tmp, sizeof(pRun)-1, "%s -e %s",termapp,pRun);
		strcpy (pRun, tmp);
	}
	if (param_root) {
		// not implemented yet
	}

	// continue with execution
	ede_start_child_process(pRun);
}


// Draw GUI run dialog. This is shown when no parameters are given
// Copied over from eRun
static Fl_Image run_pix((const char **)run_xpm);
Fl_Window* windowRunDialog;
Fl_Input* inputRunDialog;


static void cb_inputRunDialog(Fl_Input*, void*) {
  char *cmd = (char*)inputRunDialog->value();
  windowRunDialog->hide();
  Fl::flush();
  run_resource(cmd);
}

static void cb_OK(Fl_Button*, void*) {
  char *cmd = (char*)inputRunDialog->value();
  windowRunDialog->hide();
  Fl::flush();
  run_resource(cmd);
}

static void cb_Cancel(Fl_Button*, void*) {
  windowRunDialog->hide();
}

static void cb_Browse(Fl_Button*, void*) {
	char *file_types = _("Executables (*.*), *, All files (*.*), *");
	char *fileName = fl_select_file(inputRunDialog->value(), file_types, _("File selection..."));          
	if (fileName) 
	{ 
		inputRunDialog->value(fileName);
	}
}

void run_dialog() {
	Fl_Window* w = windowRunDialog = new Fl_Window(350, 160, _("Open..."));
	w->when(FL_WHEN_ENTER_KEY);

	{ Fl_Input* o = inputRunDialog = new Fl_Input(60, 77, 200, 23, _("Open:"));
	o->callback((Fl_Callback*)cb_inputRunDialog);
	o->align(132);
	o->when(FL_WHEN_ENTER_KEY); }
     
	{ Fl_Box* o = new Fl_Box(5, 5, 55, 70);
	o->image(run_pix);
	o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE); }
     
	{ Fl_Box* o = new Fl_Box(60, 5, 285, 70, _("Type the location you want to open or the name of the program you want to run\
. (Possible prefixes are: http:, ftp:, gg:, av:, leo:)"));
	o->align(132|FL_ALIGN_INSIDE); }

	{ Fl_Button* o = new Fl_Button(180, 127, 80, 25, _("&OK"));
	o->callback((Fl_Callback*)cb_OK); }
     
	{ Fl_Button* o = new Fl_Button(265, 127, 80, 25, _("&Cancel"));
	o->callback((Fl_Callback*)cb_Cancel); }
     
	{ Fl_Button* o = new Fl_Button(265, 77, 80, 25, _("&Browse..."));
	o->callback((Fl_Callback*)cb_Browse); }

	w->x(Fl::info().w/2 - (w->w()/2));
	w->y( (Fl::info().h/2) - (w->h()/2));
	w->end();
  
	w->show();
	Fl::run();
}


// Show console help on parameters
void showHelp() {
	printf ("ELauncher - ");
	printf (_("program and URL opener for EDE.\n"));
	printf ("Copyright (c) 2004. EDE authors\n");
	printf (_("Licenced under terms of GNU General Public Licence v2.0 or newer.\n\n"));
	printf (_("Usage:\n"));
	printf ("\telauncher [OPTIONS] [URL]\n");
	printf ("\telauncher [OPTIONS] [PROGRAM]\n\n");
	printf ("elauncher URL -\n");
	printf (_("\tParse URL in form protocol:address and open in appropriate program.\n\tURLs with protocol 'file' are opened based on their MIME type.\n"));
	printf ("elauncher PROGRAM -\n");
	printf (_("\tRun the program. If no path is given, look in $PATH. To give parameters\n\tto program, use quotes e.g.:\n"));
	printf ("\t\telauncher --term \"rm -rf /\"\n\n");
	printf (_("Options:\n"));
	printf ("  -h, --help\t- ");
	printf (_("This help screen.\n"));
	printf ("  --root\t- ");
	printf (_("Run as root. Dialog is opened to enter password.\n"));
	printf ("  --secure\t- ");
	printf (_("Prevent running programs. Only URLs are allowed.\n"));
	printf ("  --term\t- ");
	printf (_("Open in default terminal app.\n\n"));
}


// parse command line parameters
int main (int argc, char **argv) {
	char url[255];
	url[0] = '\0';

	fl_init_locale_support("elauncher", PREFIX"/share/locale");

	for (int i=1; i<argc; i++) {
		// params
		if ((strcmp(argv[i],"--help") == 0) || (strcmp(argv[i],"-h") == 0)) {
			showHelp();
			exit(0);
		}
		else if (strcmp(argv[i],"--root") == 0) {
			param_root = true;
		}
		else if (strcmp(argv[i],"--secure") == 0) {
			param_secure = true;
		}
		else if (strcmp(argv[i],"--term") == 0) {
			param_term = true;
		}
		
		// there shouldn't be multiple url
		else if (url[0] == '\0') {
			strcpy (url, argv[i]);
		} else {
			fprintf (stderr, _("Elauncher: Wrong number of parameters...\n"));
			exit (1);
		}
	}
	
	if (url[0] == '\0') {
		run_dialog();
	} else {
		run_resource(url);
	}
}
