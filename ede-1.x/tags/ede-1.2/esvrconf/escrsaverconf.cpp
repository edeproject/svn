/*
 * $Id$
 *
 * Esvrconf, screensaver configuration tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include "escrsaverconf.h"
#include "escreensaver.h"
#include <edeconf.h>

Fl_Window* mainWindow;

static void cb_mainWindow(Fl_Window*, void*) {
  clearOnExit();
}

Fl_Input_Browser* saversList;

static void cb_saversList(Fl_Input_Browser*, void*) {
  startSaverPreview();
}

Fl_Value_Input* timeoutSlider;

static void cb_OK(Fl_Button*, void*) {
  writeConfiguration(); clearOnExit();
}

static void cb_Cancel(Fl_Button*, void*) {
  clearOnExit();
}

Fl_Group* dpmsGroup;

Fl_Value_Input* standbySlider;

Fl_Value_Input* suspendSlider;

Fl_Value_Input* offSlider;

Fl_Check_Button* enableDPMSCheck;

static void cb_enableDPMSCheck(Fl_Check_Button*, void*) {
  if (enableDPMSCheck->value()) dpmsGroup->activate(); else  dpmsGroup->deactivate();
}

#include <efltk/Fl_Image.h>

#include <efltk/Fl_Images.h>
static const char *datas_energy[] = {
  "48 48 256 2",
  "  	c None",
  ". 	c #000000",
  "+ 	c #4271D0",
  "@ 	c #4777D0",
  "# 	c #4D81BE",
  "$ 	c #3C6FB7",
  "% 	c #4977D8",
  "& 	c #5CA24B",
  "* 	c #5A9A7E",
  "= 	c #61AC4A",
  "- 	c #65B33B",
  "; 	c #4E86A9",
  "> 	c #4579C2",
  ", 	c #3966C4",
  "' 	c #4170DC",
  ") 	c #5BA52C",
  "! 	c #549C48",
  "~ 	c #65BC0E",
  "{ 	c #67BF00",
  "] 	c #559953",
  "^ 	c #3A63E4",
  "/ 	c #58A723",
  "( 	c #55A42B",
  "_ 	c #56A71A",
  ": 	c #64B713",
  "< 	c #3F7695",
  "[ 	c #5EAB26",
  "} 	c #519362",
  "| 	c #58A63A",
  "1 	c #4A6FB3",
  "2 	c #55A226",
  "3 	c #539D3D",
  "4 	c #559D33",
  "5 	c #4B8B5E",
  "6 	c #519533",
  "7 	c #498A75",
  "8 	c #3559F9",
  "9 	c #529E27",
  "0 	c #40A01E",
  "a 	c #57A823",
  "b 	c #599F3A",
  "c 	c #35699B",
  "d 	c #3B6CAB",
  "e 	c #53954A",
  "f 	c #3F77A5",
  "g 	c #3C6AA6",
  "h 	c #4E914F",
  "i 	c #539A43",
  "j 	c #2E8947",
  "k 	c #9CB232",
  "l 	c #74AF1B",
  "m 	c #35718B",
  "n 	c #458760",
  "o 	c #2F5DBB",
  "p 	c #3864B4",
  "q 	c #4D9347",
  "r 	c #509739",
  "s 	c #4D9847",
  "t 	c #4B8C49",
  "u 	c #448F4C",
  "v 	c #2B8336",
  "w 	c #FFFE5D",
  "x 	c #BBB859",
  "y 	c #2B60AB",
  "z 	c #4D923A",
  "A 	c #498F42",
  "B 	c #497870",
  "C 	c #2850B7",
  "D 	c #2F6094",
  "E 	c #46895A",
  "F 	c #448546",
  "G 	c #326C73",
  "H 	c #498F3D",
  "I 	c #91AF4F",
  "J 	c #FFFF68",
  "K 	c #F8D645",
  "L 	c #257433",
  "M 	c #3B727C",
  "N 	c #3260AC",
  "O 	c #325DA6",
  "P 	c #2E5AA5",
  "Q 	c #42874F",
  "R 	c #33795E",
  "S 	c #2A52C4",
  "T 	c #448954",
  "U 	c #487F56",
  "V 	c #488950",
  "W 	c #4B8A52",
  "X 	c #196C58",
  "Y 	c #FFF253",
  "Z 	c #6E6C32",
  "` 	c #E2D957",
  " .	c #6B9142",
  "..	c #236C8B",
  "+.	c #3D7B5C",
  "@.	c #3164A7",
  "#.	c #325DAA",
  "$.	c #2B7A5B",
  "%.	c #206B5F",
  "&.	c #FFF43E",
  "*.	c #003A2E",
  "=.	c #AEC865",
  "-.	c #DAC32C",
  ";.	c #012592",
  ">.	c #073CCA",
  ",.	c #0E4BC2",
  "'.	c #104BB7",
  ").	c #2E58A7",
  "!.	c #1241B3",
  "~.	c #2F6E5F",
  "{.	c #2B55B4",
  "].	c #2D57AF",
  "^.	c #31765A",
  "/.	c #5F8D4E",
  "(.	c #7A916B",
  "_.	c #91A480",
  ":.	c #FFFF44",
  "<.	c #AE9B4E",
  "[.	c #004778",
  "}.	c #FFFF4E",
  "|.	c #C1BC6B",
  "1.	c #89968A",
  "2.	c #708287",
  "3.	c #4A5F88",
  "4.	c #395F9C",
  "5.	c #2A51A2",
  "6.	c #0032B2",
  "7.	c #4163AB",
  "8.	c #25686B",
  "9.	c #2B4D9B",
  "0.	c #2B599B",
  "a.	c #234BA7",
  "b.	c #2E5DA2",
  "c.	c #337765",
  "d.	c #136168",
  "e.	c #FEED65",
  "f.	c #D0BB4D",
  "g.	c #F5D457",
  "h.	c #EACC4D",
  "i.	c #AEA94B",
  "j.	c #022686",
  "k.	c #558C3C",
  "l.	c #E3D160",
  "m.	c #FEE336",
  "n.	c #FDE85B",
  "o.	c #91894A",
  "p.	c #24499B",
  "q.	c #002BB5",
  "r.	c #F8D52B",
  "s.	c #3C7C53",
  "t.	c #175687",
  "u.	c #0637A7",
  "v.	c #022CBF",
  "w.	c #08438D",
  "x.	c #095460",
  "y.	c #013162",
  "z.	c #163E97",
  "A.	c #0006C2",
  "B.	c #61746F",
  "C.	c #CDB22B",
  "D.	c #586A55",
  "E.	c #0023B4",
  "F.	c #013991",
  "G.	c #00239C",
  "H.	c #977D1A",
  "I.	c #0023A3",
  "J.	c #3C5CA7",
  "K.	c #E8CB38",
  "L.	c #000C90",
  "M.	c #D4C853",
  "N.	c #07296E",
  "O.	c #185876",
  "P.	c #FEE24A",
  "Q.	c #08329E",
  "R.	c #68788A",
  "S.	c #ADAD7E",
  "T.	c #305E56",
  "U.	c #787850",
  "V.	c #2C4887",
  "W.	c #024C6E",
  "X.	c #556D96",
  "Y.	c #0011AA",
  "Z.	c #24479E",
  "`.	c #1B45A3",
  " +	c #1D449C",
  ".+	c #0018A8",
  "++	c #1D387A",
  "@+	c #000F6F",
  "#+	c #FFFD37",
  "$+	c #A99935",
  "%+	c #FFFE55",
  "&+	c #FFFF76",
  "*+	c #4A514C",
  "=+	c #EDD74C",
  "-+	c #948C30",
  ";+	c #1439A2",
  ">+	c #294B95",
  ",+	c #003897",
  "'+	c #FFFE27",
  ")+	c #232E47",
  "!+	c #013485",
  "~+	c #204692",
  "{+	c #0C3598",
  "]+	c #0026A8",
  "^+	c #0007B4",
  "/+	c #929768",
  "(+	c #000397",
  "_+	c #000B59",
  ":+	c #0F2F8E",
  "<+	c #E8E45D",
  "[+	c #C9C662",
  "}+	c #1F4292",
  "|+	c #EECF1F",
  "1+	c #0006AB",
  "2+	c #617CA2",
  "3+	c #0014A4",
  "4+	c #001AB5",
  "5+	c #3D5789",
  "6+	c #001CA5",
  "7+	c #264E80",
  "8+	c #19409B",
  "9+	c #0F3B98",
  "0+	c #0D2E93",
  "a+	c #385170",
  "b+	c #012CAB",
  "c+	c #012AA4",
  "d+	c #DEC21D",
  "e+	c #0000B3",
  "f+	c #1B4A86",
  "g+	c #163A9C",
  "h+	c #183C9F",
  "i+	c #193D97",
  "j+	c #143A98",
  "k+	c #0D349D",
  "l+	c #000EBD",
  "m+	c #163793",
  "n+	c #103198",
  "o+	c #163696",
  "p+	c #123A9A",
  "q+	c #11349B",
  "r+	c #153E90",
  "s+	c #102E9E",
  "t+	c #00158D",
  "u+	c #062E98",
  "v+	c #123796",
  "w+	c #113A9D",
  "x+	c #00169B",
  "y+	c #022A97",
  "z+	c #0E3491",
  "A+	c #0D3294",
  "B+	c #113091",
  "C+	c #0E3191",
  "D+	c #113295",
  "E+	c #113592",
  "F+	c #12358D",
  "G+	c #344F9F",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                          . . . . . .                                           ",
  "                                . . . . . + @ # # # $ . . . . .                                 ",
  "                            . . $ + % & * = - ; > # # # # @ % > . .                             ",
  "                          . , > ' * ) ! ~ { { @ ' @ @ ' @ ] & > @ , .                           ",
  "                      . . ^ ; / ( _ : , & { < ^ [ & ; } ~ : : ~ [ | 1 . .                       ",
  "                    . 2 ) 3 _ / 4 5 6 7 5 $ 8 ] 9 2 _ _ 4 4 | 4 ) 0 _ a b .                     ",
  "                  . c d e ) 3 ( < f a g ^ $ 3 h 5 3 b ! 3 3 i | j k l m < n .                   ",
  "                . d o p ( q i e r _ 9 m o , n _ 4 s t s q e s u v w x v h < o .                 ",
  "              . 1 y o h z t q 6 A B C , p p z D E F G H q t t j I J K L s M N p .               ",
  "            . O P o c z Q A H R S S p O N o T U D c B t V V W X Y Z `  ...+.O N P .             ",
  "            . N @.o c H n M G S N @.#.N o M A V t T $.$.%.%.X X &.*.=.-.;.>.,.'.'..             ",
  "          . P ).O ).!.%.~.{.S {.#.).P ].).F +.R R ^./.(.I (._.:.<.[.X }.|.1.1.2.3.4..           ",
  "          . 5.).P 6.7.1.8.M 9.0.C 5.).a.b.Q c.d.d.~.&.e.f.g.h.i.j.+ k.(.l.m.f.n.J o..           ",
  "        . p.5.9.q._.r.f.o.q.+.s.t.6.a.u.v.w.x.` e.y.x.y.i.z.A.B.Y C.D.E.F.A.G.:.H.I.a..         ",
  "        . a.5.!.J.K.L.M.N.0.%.O.U P.Q.R.:.3.S.r.C.I n.T.J U.x i.V.G.t.G W.X.:.U.Y.C Z..         ",
  "      . `. +p..+f.++}.@+S.#+$+@+%+r.$+&+*+e.K &+f.f.Z =+%+-+++E.;+a.>+,+(.'+)+!+0.Z.~+p..       ",
  "      . {+]+^+2.#+i.N./+J K.U.(.w -+:.=+(+1.m.n._+B C.w *+^+'+:+a.u.`.E.S.i.;.9. + + + +.       ",
  "      . |.<+[+w *+!+n.C.%+L.i.o.-+D.j.j.v.}+#+U..+O.n.|+1+2+K.3+4+5+Q.6+B P.[.7+`.8+8+9+.       ",
  "    . 9.0+B.a+h./+=+*+Q.a+b+6+!+N.F.!.u.c+=+d+++4+M.$+o.A.|.U.e+|.'+:.a+L.&.y.f+g+h+i+j+.       ",
  "    . 1 ;.b+q.G.*+G.E.k+b+8+h+7+7+;+i+I.2.r.$+1+X.o.x ++l+M.U.w '+_+3.:./+M.Z q.h+f+8+{+7..     ",
  "    . Z.0+m+m+n+b+n+o+p+o+o+q+f+r+s+m+G.3.f.j.^+/+m.$+1+;.w #+D.(+E.3+t+f.w l.L.r+r+q+0+ +.     ",
  "    . u+p+v+w+{+v+w+n+w+v+{+w+z.r+n+q+z.c+x+u.6+R.C.(+b+V.r.@+^+;+o+z.]+e+(.-.G.k+s+j+y+J..     ",
  "    . :+z+0+:+A+0+B+A+B+z+0+B+0+A+z+C+C+:+D+B+:+y+3+D+E+u+(+c+F+E+0+:+E+D+x+x+D+z+C+B+j.G+.     ",
  "    . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .     ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                ",
  "                                                                                                "};

static void cb_Apply(Fl_Button*, void*) {
  writeConfiguration();
}

Fl_Window* saverWindow;

int main(int argc, char **argv) {
  Fl_Window* w;
  fl_init_locale_support("esvrconf", PREFIX"/share/locale");
   {Fl_Window* o = mainWindow = new Fl_Window(285, 374, _("Screensaver settings"));
    w = o;
    o->callback((Fl_Callback*)cb_mainWindow);
     {Fl_Group* o = new Fl_Group(5, 185, 275, 55, _("Screensaver"));
      o->box(FL_ENGRAVED_BOX);
      o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
       {Fl_Input_Browser* o = saversList = new Fl_Input_Browser(7, 15, 158, 23); o->begin();
        o->callback((Fl_Callback*)cb_saversList);
        o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
        o->type(1); getScreenhacks();
        fillSaversList(o);
        o->end();
      }
       {Fl_Group* o = new Fl_Group(175, 5, 95, 45);
         {Fl_Value_Input* o = timeoutSlider = new Fl_Value_Input(55, 9, 40, 25, _("Timeout:"));
          o->maximum(60);
          o->step(1);
          o->value(1);
          o->align(196);
          o->label(_(o->label()));
        }
        o->end();
      }
      o->label(_(o->label()));
      o->end();
    }
     {Fl_Button* o = new Fl_Button(30, 345, 80, 25, _("&OK"));
      o->callback((Fl_Callback*)cb_OK);
      o->label(_(o->label()));
    }
     {Fl_Button* o = new Fl_Button(200, 345, 80, 25, _("&Cancel"));
      o->callback((Fl_Callback*)cb_Cancel);
      o->label(_(o->label()));
    }
     {Fl_Group* o = new Fl_Group(5, 260, 275, 80, _("DPMS"));
      o->box(FL_ENGRAVED_BOX);
      o->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
       {Fl_Group* o = dpmsGroup = new Fl_Group(130, 2, 140, 74);
        o->deactivate();
         {Fl_Value_Input* o = standbySlider = new Fl_Value_Input(100, 2, 40, 20, _("Standby:"));
          o->maximum(60);
          o->step(1);
          o->value(10);
          o->align(132);
          o->label(_(o->label()));
        }
         {Fl_Value_Input* o = suspendSlider = new Fl_Value_Input(100, 27, 40, 20, _("Suspend:"));
          o->maximum(60);
          o->step(1);
          o->value(15);
          o->align(132);
          o->label(_(o->label()));
        }
         {Fl_Value_Input* o = offSlider = new Fl_Value_Input(100, 52, 40, 20, _("Off:"));
          o->maximum(60);
          o->step(1);
          o->value(20);
          o->align(132);
          o->label(_(o->label()));
        }
        o->end();
      }
       {Fl_Check_Button* o = enableDPMSCheck = new Fl_Check_Button(60, 25, 105, 25, _("Enabled"));
        o->callback((Fl_Callback*)cb_enableDPMSCheck);
        o->align(132|FL_ALIGN_INSIDE);
        o->label(_(o->label()));
      }
       {Fl_Box* o = new Fl_Box(5, 15, 55, 35);
        o->image(Fl_Image::read_xpm("icons/energy.xpm", (const char**)datas_energy));
      }
      o->label(_(o->label()));
      o->end();
    }
     {Fl_Button* o = new Fl_Button(115, 345, 80, 25, _("&Apply"));
      o->callback((Fl_Callback*)cb_Apply);
      o->label(_(o->label()));
    }
     {Fl_Group* o = new Fl_Group(40, 5, 205, 165);
       {Fl_Box* o = new Fl_Box(10, 6, 180, 131);
        o->box(FL_UP_BOX);
      }
       {Fl_Box* o = new Fl_Box(20, 15, 160, 110);
        o->box(FL_DOWN_BOX);
      }
       {Fl_Box* o = new Fl_Box(70, 137, 59, 3);
        o->box(FL_THIN_UP_BOX);
      }
       {Fl_Box* o = new Fl_Box(52, 140, 95, 12);
        o->box(FL_UP_BOX);
      }
       {Fl_Box* o = new Fl_Box(164, 127, 15, 6);
        o->box(FL_THIN_UP_BOX);
      }
       {Fl_Box* o = new Fl_Box(157, 128, 2, 4);
        o->box(FL_FLAT_BOX);
        o->color((Fl_Color)2);
      }
       {Fl_Window* o = saverWindow = new Fl_Window(22, 17, 156, 106);
        o->box(FL_FLAT_BOX);
        o->color((Fl_Color)32);
        o->end();
      }
      o->end();
    }
    o->label(_(o->label()));
    o->end();
  }
  readConfiguration(); 
  mainWindow->end(); 
  mainWindow->show(); 
  startSaverPreview(); //preview active saver
  return Fl::run();
}
