#include <efltk/Fl_Window.h>
#include <efltk/x.h>

#include <efltk/Fl.h>
#include <efltk/Fl_WM.h>
#include <efltk/Fl_Box.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Popup_Window.h>

Fl_Popup_Window *pwp;
void hide_cb(Fl_Widget *,void *) {
   Fl::exit_modal();
}

void show_cb(Fl_Widget *,void *) {
   Fl_Popup_Window pw(10,10,200,200,"Popup Window");
   pwp = &pw;
   Fl_Box          bx(5,5,190,50,"Click on the button \nor outside this window");
   bx.align(FL_ALIGN_INSIDE|FL_ALIGN_CENTER);
   Fl_Button       bt(10,60,100,20,"Click me");
   bt.callback(hide_cb);
   pw.end();
   
   pw.show_popup();
}

int main(int argc, char **argv)
{
    Fl_Window window(400,280);

    //Set type to DIALOG
    //window->window_type(Fl_WM::DIALOG);

    //window->begin();
    Fl_Button bt(100,100,100,50,"Click me");
    bt.callback(show_cb);
    window.end();
    window.show(argc, argv);

    return Fl::run();
}

//
// End of "$Id$".
//
