#include <efltk/Fl.h>
#include <efltk/Fl_Window.h>
#include <efltk/Fl_Button.h>
#include <efltk/Fl_Date_Time.h>
#include <efltk/Fl_Date_Time_Input.h>

void callback(Fl_Widget *l, void *) {
}

int main() {
    Fl_Window   window(400,300,"Date and Time widgets test");
    window.layout_spacing(4);

    Fl_Date_Input   dateInput(0,0,10,10,"Date Input:");
    dateInput.layout_align(FL_ALIGN_TOP);
    dateInput.label_width(100);

    Fl_Date_Time_Input dateTimeInput(0,0,10,10,"Date Time Input:");
    dateTimeInput.layout_align(FL_ALIGN_TOP);
    dateTimeInput.label_width(100);


    Fl_Date_Input   dateInput2(100,100,150,150,"Date Input:");

    window.end();
    window.show();
    return Fl::run();
}
