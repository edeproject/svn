// generated by Fast Light User Interface Designer (fluid) version 1.0108

#include "SciCalc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"

static double gammaln(double xx) {
  int j;
double x,y,tmp,ser;
static double cof[6] = {76.18009172947146,
						-86.50532032941677,
						24.01409824083091,
						-1.231739572450155,
						0.1208650973866179e-2,
						-0.5395239384953e-5};
	
	y = x = xx;
	tmp = x + 5.5;
	tmp -= (x+0.5)*log(tmp);
	ser = 1.000000000190015;
	for (j=0;j<6;j++) ser += cof[j]/++y;
	return -tmp+log(2.5066282746310005*ser/x);
}

static double truncf(double x) {
  if (x < 0.0)
		x = -floor(-x);
	else
		x = floor(x);
	return x;
}

void SciCalc::cb_radio_2_i(Fl_Button*, void*) {
  change_base(2);
}
void SciCalc::cb_radio_2(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_radio_2_i(o,v);
}

void SciCalc::cb_radio_8_i(Fl_Button*, void*) {
  change_base(8);
}
void SciCalc::cb_radio_8(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_radio_8_i(o,v);
}

void SciCalc::cb_radio_10_i(Fl_Button*, void*) {
  change_base(10);
}
void SciCalc::cb_radio_10(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_radio_10_i(o,v);
}

void SciCalc::cb_radio_16_i(Fl_Button*, void*) {
  change_base(16);
}
void SciCalc::cb_radio_16(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_radio_16_i(o,v);
}

void SciCalc::cb_but_7_i(Fl_Button*, void*) {
  handle_number(7);
}
void SciCalc::cb_but_7(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_7_i(o,v);
}

void SciCalc::cb_but_8_i(Fl_Button*, void*) {
  handle_number(8);
}
void SciCalc::cb_but_8(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_8_i(o,v);
}

void SciCalc::cb_but_9_i(Fl_Button*, void*) {
  handle_number(9);
}
void SciCalc::cb_but_9(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_9_i(o,v);
}

void SciCalc::cb_but_4_i(Fl_Button*, void*) {
  handle_number(4);
}
void SciCalc::cb_but_4(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_4_i(o,v);
}

void SciCalc::cb_but_5_i(Fl_Button*, void*) {
  handle_number(5);
}
void SciCalc::cb_but_5(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_5_i(o,v);
}

void SciCalc::cb_but_6_i(Fl_Button*, void*) {
  handle_number(6);
}
void SciCalc::cb_but_6(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_6_i(o,v);
}

void SciCalc::cb_but_1_i(Fl_Button*, void*) {
  handle_number(1);
}
void SciCalc::cb_but_1(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_1_i(o,v);
}

void SciCalc::cb_but_2_i(Fl_Button*, void*) {
  handle_number(2);
}
void SciCalc::cb_but_2(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_2_i(o,v);
}

void SciCalc::cb_but_3_i(Fl_Button*, void*) {
  handle_number(3);
}
void SciCalc::cb_but_3(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_3_i(o,v);
}

void SciCalc::cb_but_0_i(Fl_Button*, void*) {
  handle_number(0);
}
void SciCalc::cb_but_0(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_0_i(o,v);
}

void SciCalc::cb_but_dot_i(Fl_Button*, void*) {
  handle_number(DOT);
}
void SciCalc::cb_but_dot(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_dot_i(o,v);
}

void SciCalc::cb_but_sign_i(Fl_Button*, void*) {
  if (! emode)
{
	value[top] = -value[top];
	set_display(value[top],NORM);
}
else 
{
	exponent = -exponent;
	value[top] = mantissa*pow(10.0, (double)exponent);
	set_display(mantissa,EXP);
};
}
void SciCalc::cb_but_sign(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->parent()->user_data()))->cb_but_sign_i(o,v);
}

void SciCalc::cb_but_C_i(Fl_Button*, void*) {
  init_value(top); 
set_display(0.0,NORM);
}
void SciCalc::cb_but_C(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_C_i(o,v);
}

void SciCalc::cb_but_AC_i(Fl_Button*, void*) {
  init_value(0);
set_display(0.0,NORM);
currentbrkt = 0;
box_bracket->label("");
box_bracket->redraw();
}
void SciCalc::cb_but_AC(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_AC_i(o,v);
}

void SciCalc::cb_but_X_i(Fl_Button*, void*) {
  handle_operator(MULT);
}
void SciCalc::cb_but_X(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_X_i(o,v);
}

void SciCalc::cb_but_div_i(Fl_Button*, void*) {
  handle_operator(DIV);
}
void SciCalc::cb_but_div(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_div_i(o,v);
}

void SciCalc::cb_but_plus_i(Fl_Button*, void*) {
  handle_operator(PLUS);
}
void SciCalc::cb_but_plus(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_plus_i(o,v);
}

void SciCalc::cb_but_minus_i(Fl_Button*, void*) {
  handle_operator(MINUS);
}
void SciCalc::cb_but_minus(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_minus_i(o,v);
}

void SciCalc::cb_but_pi_i(Fl_Button*, void*) {
  exponent_pi();
}
void SciCalc::cb_but_pi(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_pi_i(o,v);
}

void SciCalc::cb_but_eval_i(Fl_Button*, void*) {
  handle_operator(EVAL);
}
void SciCalc::cb_but_eval(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_eval_i(o,v);
}

void SciCalc::cb_but_eval_hidden_i(Fl_Button*, void*) {
  handle_operator(EVAL);
}
void SciCalc::cb_but_eval_hidden(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_eval_hidden_i(o,v);
}

void SciCalc::cb_but_eval_hidden2_i(Fl_Button*, void*) {
  handle_operator(EVAL);
}
void SciCalc::cb_but_eval_hidden2(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_eval_hidden2_i(o,v);
}

void SciCalc::cb_but_sqrt_i(Fl_Button*, void*) {
  if (base > 10) handle_number(10.0); 
	else
	if (! inv) 
	{
		value[top] = sqrt(value[top]);
		set_display(value[top],NORM);
		ready = 1;
	}
	else
	{
		value[top] = pow(value[top], 2.0);
		set_display(value[top],NORM);
		ready = 1;
	};
}
void SciCalc::cb_but_sqrt(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_sqrt_i(o,v);
}

void SciCalc::cb_but_pow_i(Fl_Button*, void*) {
  if (base > 10)
		handle_number(11.0);
	else
		handle_operator(check_inv->value()?INVPOW:POW);
}
void SciCalc::cb_but_pow(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_pow_i(o,v);
}

void SciCalc::cb_but_sin_i(Fl_Button*, void*) {
  if (base > 10) handle_number(12.0); 
else
	if (! inv) 
	{
		value[top] = sin(to_drg(value[top]));
		set_display(value[top],NORM);
		ready = 1;
	}
	else
	{
		value[top] = from_drg(asin(value[top]));
		set_display(value[top],NORM);
		ready = 1;
	};
}
void SciCalc::cb_but_sin(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_sin_i(o,v);
}

void SciCalc::cb_but_cos_i(Fl_Button*, void*) {
  if (base > 10) handle_number(13.0);
else
	if (! inv) 
	{
		value[top] = cos(to_drg(value[top]));
		set_display(value[top],NORM);
		ready = 1;
	}
	else
	{
		value[top] = from_drg(acos(value[top]));
		set_display(value[top],NORM);
		ready = 1;
	};
}
void SciCalc::cb_but_cos(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_cos_i(o,v);
}

void SciCalc::cb_but_tan_i(Fl_Button*, void*) {
  if (base > 10) handle_number(14.0);
	else
	if (! inv)
	{
		value[top] = tan(to_drg(value[top]));
		set_display(value[top],NORM);
		ready = 1;
	}
	else
	{
		value[top] = from_drg(atan(value[top]));
		set_display(value[top],NORM);
		ready = 1;
	};
}
void SciCalc::cb_but_tan(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_tan_i(o,v);
}

void SciCalc::cb_but_log_i(Fl_Button*, void*) {
  if (base > 10) handle_number(15.0);
else
	if (! inv)
	{
		value[top] = log10(value[top]);
		set_display(value[top],NORM);
		ready = 1;
	}
	else
	{
		value[top] = pow(10.0, value[top]);
		set_display(value[top],NORM);
		ready = 1;
	};
}
void SciCalc::cb_but_log(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_log_i(o,v);
}

void SciCalc::cb_but_ln_i(Fl_Button*, void*) {
  if (! inv)
	{
		value[top] = log(value[top]);
		set_display(value[top],NORM);
		ready = 1;
	}
	else
	{
		value[top] = exp(value[top]);
		set_display(value[top],NORM);
		ready = 1;
	};
}
void SciCalc::cb_but_ln(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_ln_i(o,v);
}

void SciCalc::cb_but_int_i(Fl_Button*, void*) {
  if (! inv)
	{
		value[top] = truncf(value[top]);
		set_display(value[top],NORM);
		ready = 1;
	}
	else
	{
		value[top] = value[top] - truncf(value[top]);
		set_display(value[top],NORM);
		ready = 1;
	};
}
void SciCalc::cb_but_int(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_int_i(o,v);
}

void SciCalc::cb_but_dr_i(Fl_Button*, void*) {
  if (! inv) 
	{
		value[top] = M_PI*value[top]/180.0;
		set_display(value[top],NORM);
		ready = 1;
	}
	else
	{
		value[top] = 180.0*value[top]/M_PI;
		set_display(value[top],NORM);
		ready = 1;
	};
}
void SciCalc::cb_but_dr(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_dr_i(o,v);
}

void SciCalc::cb_but_drg_i(Fl_Button*, void*) {
  drgmode++;
	drgmode %= 3;
	set_drgdisp();
}
void SciCalc::cb_but_drg(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_drg_i(o,v);
}

void SciCalc::cb_but_leftbr_i(Fl_Button*, void*) {
  add_left_bracket();
}
void SciCalc::cb_but_leftbr(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_leftbr_i(o,v);
}

void SciCalc::cb_but_rightbr_i(Fl_Button*, void*) {
  add_right_bracket();
}
void SciCalc::cb_but_rightbr(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_rightbr_i(o,v);
}

void SciCalc::cb_but_exch_i(Fl_Button*, void*) {
  exchange();
}
void SciCalc::cb_but_exch(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_exch_i(o,v);
}

void SciCalc::cb_but_invx_i(Fl_Button*, void*) {
  value[top] = 1.0/value[top];
	set_display(value[top],NORM);
	ready = 1;
}
void SciCalc::cb_but_invx(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_invx_i(o,v);
}

void SciCalc::cb_but_fact_i(Fl_Button*, void*) {
  factorial();
}
void SciCalc::cb_but_fact(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_fact_i(o,v);
}

void SciCalc::cb_but_Mplus_i(Fl_Button*, void*) {
  if (! inv) mem += value[top]; else mem -= value[top];
	set_display(value[top],NORM);
	ready = 1;
	set_memdisp();
}
void SciCalc::cb_but_Mplus(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_Mplus_i(o,v);
}

void SciCalc::cb_but_Mmult_i(Fl_Button*, void*) {
  if (! inv) mem *= value[top]; else mem /= value[top];
	set_display(value[top],NORM);
	ready = 1;
	set_memdisp();
}
void SciCalc::cb_but_Mmult(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_Mmult_i(o,v);
}

void SciCalc::cb_but_Mclear_i(Fl_Button*, void*) {
  if (! inv)
	{
		mem = 0.0;
		set_display(value[top],NORM);
		ready = 1;
		set_memdisp();
	}
	else
		memexch();
}
void SciCalc::cb_but_Mclear(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_Mclear_i(o,v);
}

void SciCalc::cb_but_Mst_i(Fl_Button*, void*) {
  mem = value[top];
	set_display(value[top],NORM);
	ready = 1;
	set_memdisp();
}
void SciCalc::cb_but_Mst(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_Mst_i(o,v);
}

void SciCalc::cb_but_Mrc_i(Fl_Button*, void*) {
  value[top] = mem;
	set_display(value[top],NORM);
	ready = 1;
}
void SciCalc::cb_but_Mrc(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_Mrc_i(o,v);
}

void SciCalc::cb_check_inv_i(Fl_Button*, void*) {
  if (inv) 
	{
		inv = 0;
		setnormlabels();
	}
	else
	{
		inv = 1;
		setinvlabels();
	};
}
void SciCalc::cb_check_inv(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_check_inv_i(o,v);
}

void SciCalc::cb_but_quit_i(Fl_Button*, void*) {
  win->hide();
delete win;
}
void SciCalc::cb_but_quit(Fl_Button* o, void* v) {
  ((SciCalc*)(o->parent()->user_data()))->cb_but_quit_i(o,v);
}
/* members */
/********/

SciCalc::SciCalc() {
  { win = new Fl_Double_Window(181, 262, "ecalc");
    win->user_data((void*)(this));
    { leddisplay = new Fl_Box(5, 3, 172, 24, "0 ");
      leddisplay->box(FL_DOWN_BOX);
      leddisplay->color((Fl_Color)207);
      leddisplay->labelfont(1);
      leddisplay->labelsize(16);
      leddisplay->labelcolor((Fl_Color)59);
      leddisplay->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
    } // Fl_Box* leddisplay
    { box_DEGRAD = new Fl_Box(24, 27, 35, 15, " ");
      box_DEGRAD->box(FL_ENGRAVED_BOX);
      box_DEGRAD->labelsize(9);
      box_DEGRAD->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    } // Fl_Box* box_DEGRAD
    { box_bracket = new Fl_Box(59, 27, 65, 15);
      box_bracket->box(FL_ENGRAVED_BOX);
      box_bracket->labelsize(9);
      box_bracket->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    } // Fl_Box* box_bracket
    { box_M = new Fl_Box(124, 27, 35, 15, "M");
      box_M->box(FL_ENGRAVED_BOX);
      box_M->labelsize(9);
      box_M->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    } // Fl_Box* box_M
    { Fl_Group* o = new Fl_Group(46, 44, 93, 22);
      o->color((Fl_Color)46);
      { radio_2 = new Fl_Button(49, 48, 20, 15, "2");
        radio_2->type(102);
        radio_2->labelsize(10);
        radio_2->labelcolor((Fl_Color)1);
        radio_2->callback((Fl_Callback*)cb_radio_2);
      } // Fl_Button* radio_2
      { radio_8 = new Fl_Button(70, 48, 21, 15, "8");
        radio_8->type(102);
        radio_8->labelsize(10);
        radio_8->labelcolor((Fl_Color)1);
        radio_8->callback((Fl_Callback*)cb_radio_8);
      } // Fl_Button* radio_8
      { radio_10 = new Fl_Button(92, 48, 21, 15, "10");
        radio_10->type(102);
        radio_10->value(1);
        radio_10->labelsize(10);
        radio_10->labelcolor((Fl_Color)1);
        radio_10->callback((Fl_Callback*)cb_radio_10);
      } // Fl_Button* radio_10
      { radio_16 = new Fl_Button(114, 48, 21, 15, "16");
        radio_16->type(102);
        radio_16->labelsize(10);
        radio_16->labelcolor((Fl_Color)1);
        radio_16->callback((Fl_Callback*)cb_radio_16);
      } // Fl_Button* radio_16
      o->end();
    } // Fl_Group* o
    { Fl_Group* o = new Fl_Group(3, 163, 107, 94);
      o->box(FL_FLAT_BOX);
      o->color((Fl_Color)43);
      { but_7 = new Fl_Button(6, 167, 32, 20, "7");
        but_7->shortcut(0x37);
        but_7->labelfont(1);
        but_7->labelsize(16);
        but_7->callback((Fl_Callback*)cb_but_7);
      } // Fl_Button* but_7
      { but_8 = new Fl_Button(41, 167, 32, 20, "8");
        but_8->shortcut(0x38);
        but_8->labelfont(1);
        but_8->labelsize(16);
        but_8->callback((Fl_Callback*)cb_but_8);
      } // Fl_Button* but_8
      { but_9 = new Fl_Button(75, 167, 32, 20, "9");
        but_9->shortcut(0x39);
        but_9->labelfont(1);
        but_9->labelsize(16);
        but_9->callback((Fl_Callback*)cb_but_9);
      } // Fl_Button* but_9
      { but_4 = new Fl_Button(6, 189, 32, 20, "4");
        but_4->shortcut(0x34);
        but_4->labelfont(1);
        but_4->labelsize(16);
        but_4->callback((Fl_Callback*)cb_but_4);
      } // Fl_Button* but_4
      { but_5 = new Fl_Button(41, 189, 32, 20, "5");
        but_5->shortcut(0x35);
        but_5->labelfont(1);
        but_5->labelsize(16);
        but_5->callback((Fl_Callback*)cb_but_5);
      } // Fl_Button* but_5
      { but_6 = new Fl_Button(75, 189, 32, 20, "6");
        but_6->shortcut(0x36);
        but_6->labelfont(1);
        but_6->labelsize(16);
        but_6->callback((Fl_Callback*)cb_but_6);
      } // Fl_Button* but_6
      { but_1 = new Fl_Button(6, 211, 32, 20, "1");
        but_1->shortcut(0x31);
        but_1->labelfont(1);
        but_1->labelsize(16);
        but_1->callback((Fl_Callback*)cb_but_1);
      } // Fl_Button* but_1
      { but_2 = new Fl_Button(41, 211, 32, 20, "2");
        but_2->shortcut(0x32);
        but_2->labelfont(1);
        but_2->labelsize(16);
        but_2->callback((Fl_Callback*)cb_but_2);
      } // Fl_Button* but_2
      { but_3 = new Fl_Button(75, 211, 32, 20, "3");
        but_3->shortcut(0x33);
        but_3->labelfont(1);
        but_3->labelsize(16);
        but_3->callback((Fl_Callback*)cb_but_3);
      } // Fl_Button* but_3
      { but_0 = new Fl_Button(6, 233, 32, 20, "0");
        but_0->shortcut(0x30);
        but_0->labelfont(1);
        but_0->labelsize(16);
        but_0->callback((Fl_Callback*)cb_but_0);
      } // Fl_Button* but_0
      { but_dot = new Fl_Button(41, 233, 32, 20, ".");
        but_dot->shortcut(0x2e);
        but_dot->labelfont(1);
        but_dot->labelsize(16);
        but_dot->callback((Fl_Callback*)cb_but_dot);
      } // Fl_Button* but_dot
      { but_sign = new Fl_Button(75, 233, 32, 20, "+/-");
        but_sign->labelfont(1);
        but_sign->labelsize(16);
        but_sign->callback((Fl_Callback*)cb_but_sign);
      } // Fl_Button* but_sign
      o->end();
    } // Fl_Group* o
    { but_C = new Fl_Button(112, 167, 31, 20, "C");
      but_C->labelfont(1);
      but_C->labelsize(16);
      but_C->callback((Fl_Callback*)cb_but_C);
    } // Fl_Button* but_C
    { but_AC = new Fl_Button(146, 167, 30, 20, "AC");
      but_AC->labelfont(1);
      but_AC->labelsize(16);
      but_AC->callback((Fl_Callback*)cb_but_AC);
    } // Fl_Button* but_AC
    { but_X = new Fl_Button(112, 189, 31, 20, "x");
      but_X->shortcut(0x2a);
      but_X->labelfont(1);
      but_X->labelsize(16);
      but_X->callback((Fl_Callback*)cb_but_X);
    } // Fl_Button* but_X
    { but_div = new Fl_Button(146, 189, 30, 20, "/");
      but_div->shortcut(0x2f);
      but_div->labelfont(1);
      but_div->labelsize(16);
      but_div->callback((Fl_Callback*)cb_but_div);
    } // Fl_Button* but_div
    { but_plus = new Fl_Button(112, 211, 31, 20, "+");
      but_plus->shortcut(0x2b);
      but_plus->labelfont(1);
      but_plus->labelsize(16);
      but_plus->callback((Fl_Callback*)cb_but_plus);
    } // Fl_Button* but_plus
    { but_minus = new Fl_Button(146, 211, 30, 20, "-");
      but_minus->shortcut(0x2d);
      but_minus->labelfont(1);
      but_minus->labelsize(16);
      but_minus->callback((Fl_Callback*)cb_but_minus);
    } // Fl_Button* but_minus
    { but_pi = new Fl_Button(112, 233, 31, 20, "e/p");
      but_pi->labelfont(12);
      but_pi->labelsize(17);
      but_pi->callback((Fl_Callback*)cb_but_pi);
    } // Fl_Button* but_pi
    { but_eval = new Fl_Button(146, 233, 30, 20, "=");
      but_eval->shortcut(0x3d);
      but_eval->labelfont(1);
      but_eval->labelsize(16);
      but_eval->callback((Fl_Callback*)cb_but_eval);
    } // Fl_Button* but_eval
    { but_eval_hidden = new Fl_Button(147, 253, 6, 7);
      but_eval_hidden->box(FL_NO_BOX);
      but_eval_hidden->shortcut(0xff0d);
      but_eval_hidden->labelfont(1);
      but_eval_hidden->labelsize(16);
      but_eval_hidden->callback((Fl_Callback*)cb_but_eval_hidden);
    } // Fl_Button* but_eval_hidden
    { but_eval_hidden2 = new Fl_Button(157, 263, 6, 7);
      but_eval_hidden2->box(FL_NO_BOX);
      but_eval_hidden2->shortcut(0xff8d);
      but_eval_hidden2->labelfont(1);
      but_eval_hidden2->labelsize(16);
      but_eval_hidden2->callback((Fl_Callback*)cb_but_eval_hidden2);
    } // Fl_Button* but_eval_hidden2
    { but_sqrt = new Fl_Button(6, 70, 32, 21, "sqrt");
      but_sqrt->labelsize(11);
      but_sqrt->labelcolor((Fl_Color)4);
      but_sqrt->callback((Fl_Callback*)cb_but_sqrt);
    } // Fl_Button* but_sqrt
    { but_pow = new Fl_Button(41, 70, 32, 21, "x^y");
      but_pow->labelsize(11);
      but_pow->labelcolor((Fl_Color)4);
      but_pow->callback((Fl_Callback*)cb_but_pow);
    } // Fl_Button* but_pow
    { but_sin = new Fl_Button(76, 70, 31, 21, "sin");
      but_sin->labelsize(11);
      but_sin->labelcolor((Fl_Color)4);
      but_sin->callback((Fl_Callback*)cb_but_sin);
    } // Fl_Button* but_sin
    { but_cos = new Fl_Button(110, 70, 31, 21, "cos");
      but_cos->labelsize(11);
      but_cos->labelcolor((Fl_Color)4);
      but_cos->callback((Fl_Callback*)cb_but_cos);
    } // Fl_Button* but_cos
    { but_tan = new Fl_Button(144, 70, 30, 21, "tan");
      but_tan->labelsize(11);
      but_tan->labelcolor((Fl_Color)4);
      but_tan->callback((Fl_Callback*)cb_but_tan);
    } // Fl_Button* but_tan
    { but_log = new Fl_Button(6, 93, 32, 21, "log");
      but_log->labelsize(11);
      but_log->labelcolor((Fl_Color)4);
      but_log->callback((Fl_Callback*)cb_but_log);
    } // Fl_Button* but_log
    { but_ln = new Fl_Button(41, 93, 32, 21, "ln");
      but_ln->labelsize(11);
      but_ln->labelcolor((Fl_Color)4);
      but_ln->callback((Fl_Callback*)cb_but_ln);
    } // Fl_Button* but_ln
    { but_int = new Fl_Button(76, 93, 31, 21, "int");
      but_int->labelsize(11);
      but_int->labelcolor((Fl_Color)4);
      but_int->callback((Fl_Callback*)cb_but_int);
    } // Fl_Button* but_int
    { but_dr = new Fl_Button(110, 93, 31, 21, "d->r");
      but_dr->labelsize(10);
      but_dr->labelcolor((Fl_Color)4);
      but_dr->callback((Fl_Callback*)cb_but_dr);
    } // Fl_Button* but_dr
    { but_drg = new Fl_Button(144, 93, 30, 21, "d-r-g");
      but_drg->labelsize(9);
      but_drg->callback((Fl_Callback*)cb_but_drg);
    } // Fl_Button* but_drg
    { but_leftbr = new Fl_Button(6, 116, 32, 21, "[");
      but_leftbr->shortcut(0x28);
      but_leftbr->labelsize(11);
      but_leftbr->callback((Fl_Callback*)cb_but_leftbr);
    } // Fl_Button* but_leftbr
    { but_rightbr = new Fl_Button(41, 116, 32, 21, "]");
      but_rightbr->shortcut(0x29);
      but_rightbr->labelsize(11);
      but_rightbr->callback((Fl_Callback*)cb_but_rightbr);
    } // Fl_Button* but_rightbr
    { but_exch = new Fl_Button(76, 116, 31, 21, "exch");
      but_exch->labelsize(11);
      but_exch->callback((Fl_Callback*)cb_but_exch);
    } // Fl_Button* but_exch
    { but_invx = new Fl_Button(110, 116, 31, 21, "1/x");
      but_invx->labelsize(11);
      but_invx->callback((Fl_Callback*)cb_but_invx);
    } // Fl_Button* but_invx
    { but_fact = new Fl_Button(144, 116, 30, 21, "x!");
      but_fact->labelsize(11);
      but_fact->callback((Fl_Callback*)cb_but_fact);
    } // Fl_Button* but_fact
    { but_Mplus = new Fl_Button(6, 139, 32, 21, "M+");
      but_Mplus->color((Fl_Color)93);
      but_Mplus->labelcolor((Fl_Color)4);
      but_Mplus->callback((Fl_Callback*)cb_but_Mplus);
    } // Fl_Button* but_Mplus
    { but_Mmult = new Fl_Button(41, 139, 32, 21, "M*");
      but_Mmult->color((Fl_Color)93);
      but_Mmult->labelcolor((Fl_Color)4);
      but_Mmult->callback((Fl_Callback*)cb_but_Mmult);
    } // Fl_Button* but_Mmult
    { but_Mclear = new Fl_Button(76, 139, 31, 21, "MC");
      but_Mclear->color((Fl_Color)93);
      but_Mclear->labelcolor((Fl_Color)4);
      but_Mclear->callback((Fl_Callback*)cb_but_Mclear);
    } // Fl_Button* but_Mclear
    { but_Mst = new Fl_Button(110, 139, 31, 21, "Mst");
      but_Mst->color((Fl_Color)93);
      but_Mst->callback((Fl_Callback*)cb_but_Mst);
    } // Fl_Button* but_Mst
    { but_Mrc = new Fl_Button(144, 139, 30, 21, "Mrc");
      but_Mrc->color((Fl_Color)93);
      but_Mrc->callback((Fl_Callback*)cb_but_Mrc);
    } // Fl_Button* but_Mrc
    { check_inv = new Fl_Button(6, 44, 32, 21, "inv");
      check_inv->type(1);
      check_inv->labelsize(11);
      check_inv->labelcolor((Fl_Color)4);
      check_inv->callback((Fl_Callback*)cb_check_inv);
    } // Fl_Button* check_inv
    { but_quit = new Fl_Button(145, 44, 29, 21, "Exit");
      but_quit->labelfont(1);
      but_quit->labelcolor((Fl_Color)33);
      but_quit->callback((Fl_Callback*)cb_but_quit);
    } // Fl_Button* but_quit
    win->end();
  } // Fl_Double_Window* win
  init_value(0);
	drgmode = 1;
	base = 10;
	currentbrkt = 0;
	startbrkt[0] = 0;

	set_memdisp();
	set_brktdisp();
	radio_10->value(1);
	set_drgdisp();
	set_display(0.0,NONE);
}

SciCalc::~SciCalc() {
}

void SciCalc::run(int px,int py) {
  win->show();
Fl::run();
}

void SciCalc::handle_number(double numb) {
  int first;
double sign;

	if (ready) init_value(top);

	if (numb == -1.0) 
	{
		if (dot) /* check whether we already have a dot */
			return;
		else 
		{
			dot = 1;
			set_display(value[top],DOT);
			return;
		}
	}

	if (emode) 
	{
		sign = copysign(1.0, (double)exponent);
		if (abs(exponent)*10 + numb > 999) 
		{ /* cycle if exponent has > 3 digits */
			first = (int)floor((double)abs(exponent)/100.0);
			exponent = abs(exponent) - 100*first;
			exponent *= (int)sign;
		}
		exponent = exponent*10 + (int) (sign*numb);
		value[top] = mantissa*pow(10.0, (double)exponent);
		set_display(mantissa, EXP);
	}
	else if (numb < base)
	{ /* both decimal and non decimal number entry */
		sign = copysign(1.0, value[top]);
		if (dot && behind < 9) 
		{
			behind++;
			diver = diver/(double)base;
			value[top] += sign*diver*numb;
		}
		else 
		if ((! dot) && (value[top] < 1.0e10))
			value[top] = (double)base*value[top] + sign*numb;

		set_display(value[top],(mode)behind);
	}
}

void SciCalc::handle_operator(Operator op) {
  int prevop, i, finished;

	switch (op)
	{
		case PLUS:
		case MINUS:
		case MULT:
		case DIV:
		case POW:
		case INVPOW:
			finished = 0;
			do 
			{
				if (top == startbrkt[currentbrkt]) finished = 1; /* is 1st operator */
				if (! finished) 
				{ /* compare priority of previous operators with current op */
					prevop = oper[top-1];
					if (priority[prevop] < priority[op])
						finished = 1;
					else 
					{ /* last op can be calculated */
						top--;
    					calc(top);
					}
				}
			} while (! finished);

			oper[top] = op;
			init_value(top+1);

			set_display(value[top-1],NORM);
			break;

		case EVAL:
			while (currentbrkt > 0) add_right_bracket();
			for (i = top; i > 0; i--) calc(i-1);
			top = 0;
			ready = 1;
			set_display(value[top],NORM);
			break;
	}
}

void SciCalc::change_base(int newbase) {
  int oldbase;

	oldbase = base;
	base = newbase;

	set_display(value[top], NORM);
	ready = 1;
	if ((oldbase == 16) || (base == 16)) setnormlabels();
}

void SciCalc::set_display(double val,mode behind) {
  int i;
char dispstr[40], expstr[10], str2[10];

	/* number or operator handled to get here so reset inv stuff */
/*
	if (inv) 
	{
		inv = 0;
		check_inv->value(0);
		setnormlabels();
	}*/
	if (behind >= 0) 
	{ /* format with appropriate number of decimal places */
		if (base == 10) 
		{
			emode = 0;
			strcpy(str2,"%.1f");
			str2[2] = behind + '0';
			sprintf(dispstr,str2,val);
		}
		else /* non base 10 display */
			cvttobase(val, base, behind, dispstr);
	}
	else 
	if (behind == DOT) 
	{ /* display the . at the right */
		if (base == 10) 
		{
			emode = 0;
			sprintf(dispstr,"%.1f",val);
			dispstr[strlen(dispstr)-1] = 0;
		}
		else
			cvttobase(val, base, behind, dispstr);
	}
	else if (behind == NORM) 
	{ /* normal display */
		if (base == 10) 
		{
			emode = 0;
			sprintf(dispstr,"%.9g",val);
		}
		else /* non base 10 display */
			cvttobase(val, base, behind, dispstr);
	}
	else 
	{ /* exponent entering display */
		sprintf(dispstr,"%.8f",val);
		for (i = strlen(dispstr); dispstr[i-1] == '0'; i--);
		dispstr[i] =0;
		strcat(dispstr, "e");
		sprintf(expstr,"%d",exponent);
		strcat(dispstr, expstr);
	}
	strcat(dispstr," ");
	dispstr[17] = 0;
	leddisplay->copy_label(dispstr);
	leddisplay->redraw();
}

void SciCalc::set_memdisp() {
  if (mem)
	box_M->label("M");
else
	box_M->label("");
box_M->redraw();
}

void SciCalc::set_drgdisp() {
  if (drgmode == 0)
	box_DEGRAD->label("DEG");
else 
{
	if (drgmode == 1)
		box_DEGRAD->label("RAD");
	else
		box_DEGRAD->label("GRAD");
}
box_DEGRAD->redraw();
}

void SciCalc::set_brktdisp() {
  char dispstr[40];

	if (currentbrkt > 0)
	{
		sprintf(dispstr, "%d [ max %d", currentbrkt, MaxNumBrkts);
		box_bracket->copy_label(dispstr);
	}
	else
		box_bracket->label("");
box_bracket->redraw();
}

void SciCalc::add_left_bracket() {
  if (currentbrkt < MaxNumBrkts) 
	{
		currentbrkt++;
		startbrkt[currentbrkt] = top;
		ready = 1;
		set_brktdisp();
	}
}

void SciCalc::add_right_bracket() {
  int i;

	if (currentbrkt > 0) 
	{
		for (i = top; i > startbrkt[currentbrkt]; i--) calc(i-1);
		top = startbrkt[currentbrkt];
		currentbrkt--;
		ready = 1;
	}
	set_display(value[top],NORM);
	set_brktdisp();
}

void SciCalc::factorial() {
  double lg, alpha;

  /* uses gamma functions to get result for non-integer values */

	alpha = value[top] + 1.0;
	if ((floor(alpha) == alpha)&&(alpha <= 0.0))
	{
		init_value(0);
		leddisplay->label("Error: -ve integer ");
		leddisplay->redraw();
	}
	else
	if (alpha > 32)
	 {
		lg = exp(gammaln(alpha));
    		value[top] = lg;
   		 set_display(value[top],NORM);
		ready = 1;
	}
	else
	if (alpha > 1.0)
	{
		int n = (int)truncf(alpha);
		lg = 1.0;
		for (int i = 1; i <n; i++) lg *= i;
		value[top] = lg;
		set_display(value[top],NORM);
		ready = 1;
	}
}

void SciCalc::exchange() {
  double temp;

  /* check if we have 2 values to exchange */
  if (top > startbrkt[currentbrkt]) {
    temp = value[top];
    value[top] = value[top-1];
    value[top-1] = temp;

    set_display(value[top],NORM);
    ready = 1;
  }
}

void SciCalc::exponent_pi() {
  if ((value[top] == 0.0) || (ready)) {
    value[top] = M_PI;
    set_display(value[top],NORM);
    ready = 1;
  }
  else if ((! emode) && (base == 10)) {
    emode = 1;
    exponent = 0;
    mantissa = value[top];
    set_display(mantissa,EXP);
  }
}

void SciCalc::calc(int i) {
  switch (oper[i])
	 {
		case PLUS:	value[i] += value[i+1]; break;
		case MINUS:	value[i] -= value[i+1]; break;
		case MULT:	value[i] *= value[i+1]; break;
		case DIV:	value[i] /= value[i+1]; break;
		case POW:	value[i] = pow(value[i], value[i+1]); break;
		case INVPOW: value[i] = pow(value[i], 1.0/value[i+1]); break;
	}
}

void SciCalc::init_value(int lev) {
  top = lev;
	value[top] = 0.0;
	ready = 0;
	emode = 0;
	dot = 0;
	diver = 1.0;
	behind = 0;
	if (inv) 
	{
		inv = 0;
		check_inv->value(0);
		setnormlabels();
	}
}

void SciCalc::cvttobase(double num,int base,mode behind,char *str) {
  double sign, div;
int place, digit, i;
char digstr[2];

	sign = copysign(1.0, num);
	num *= sign;
	if (sign == -1.0)
		sprintf(str, "-");
	else
		str[0] = 0;

	if (num == 0.0) 
	{
		sprintf(str, "0");
		if (behind > 0) 
		{
			strcat(str, ".");
			for(i = 0; i < behind; i++) strcat(str, "0");
		}
		return;
	}
	place = (int)( log(num)/log((double)base) );
	if (place < 0) place = 0;
	do 
	{
		div = pow((double)base, (double)place);
		digit = (int)(num/div);
		num -= (double)digit*div;
		if (place == -1) strcat(str, ".");
		place--;
		sprintf(digstr, "%x", digit);
		strcat(str, digstr);
		if (strlen(str) > 18) 
		{
			sprintf(str, "can't display");
			return;
		}
	} while ((place >= 0) || ((place >= -9) && (num != 0.0)));

	if ((place == -1) && ((behind == DOT) || (behind > 0)))
		strcat(str, ".");
	while ((behind > 0) && (behind >= -place)) 
	{
		strcat(str, "0");
		place--;
	}
}

void SciCalc::setnormlabels() {
  if (base <= 10)
	{
		but_sqrt->label("sqrt");  but_sqrt->shortcut(0);
		but_pow->label("x^y"); but_pow->shortcut(0);
		but_sin->label("sin");   but_sin->shortcut(0);
		but_cos->label("cos"); but_cos->shortcut(0);
		but_tan->label("tan");   but_tan->shortcut(0);
		but_log->label("log");  but_log->shortcut(0);
		but_sqrt->labelcolor(FL_BLUE);
		but_pow->labelcolor(FL_BLUE);
		but_sin->labelcolor(FL_BLUE);
		but_cos->labelcolor(FL_BLUE);
		but_tan->labelcolor(FL_BLUE);
		but_log->labelcolor(FL_BLUE);
	}
	else
	{
		but_sqrt->label("a");  but_sqrt->shortcut('a');
		but_pow->label("b");  but_pow->shortcut('b');
		but_sin->label("c");   but_sin->shortcut('c');
		but_cos->label("d");  but_cos->shortcut('d');
		but_tan->label("e");  but_tan->shortcut('e');
		but_log->label("f");   but_log->shortcut('f');
		but_sqrt->labelcolor(FL_BLACK);
		but_pow->labelcolor(FL_BLACK);
		but_sin->labelcolor(FL_BLACK);
		but_cos->labelcolor(FL_BLACK);
		but_tan->labelcolor(FL_BLACK);
		but_log->labelcolor(FL_BLACK);
	}
	but_ln->label("ln");
	but_int->label("int");
	but_dr->label("d->r");
	but_Mplus->label("M+");
	but_Mmult->label("M*");
	but_Mclear->label("MC");
	but_sqrt->redraw();
	but_pow->redraw();
	but_sin->redraw();
	but_cos->redraw();
	but_tan->redraw();
	but_log->redraw();
	but_ln->redraw();
	but_int->redraw();
	but_dr->redraw();
	but_Mplus->redraw();
	but_Mmult->redraw();
	but_Mclear->redraw();
}

void SciCalc::setinvlabels() {
  if (base <= 10)
	{
		but_sqrt->label("x^2");
		but_pow->label("x^1/y");
		but_sin->label("asin");
		but_cos->label("acos");
		but_tan->label("atan");
		but_log->label("10^x");
		but_sqrt->labelcolor(FL_BLUE);
		but_pow->labelcolor(FL_BLUE);
		but_sin->labelcolor(FL_BLUE);
		but_cos->labelcolor(FL_BLUE);
		but_tan->labelcolor(FL_BLUE);
		but_log->labelcolor(FL_BLUE);
	}
	else
	{
		but_sqrt->label("a");
		but_pow->label("b");
		but_sin->label("c");
		but_cos->label("d");
		but_tan->label("e");
		but_log->label("f");
		but_sqrt->labelcolor(FL_BLACK);
		but_pow->labelcolor(FL_BLACK);
		but_sin->labelcolor(FL_BLACK);
		but_cos->labelcolor(FL_BLACK);
		but_tan->labelcolor(FL_BLACK);
		but_log->labelcolor(FL_BLACK);
	}
	but_ln->label("e^x");
	but_int->label("frac");
	but_dr->label("r->d");
	but_Mplus->label("M-");
	but_Mmult->label("M/");
	but_Mclear->label("Mex");
	but_sqrt->redraw();
	but_pow->redraw();
	but_sin->redraw();
	but_cos->redraw();
	but_tan->redraw();
	but_log->redraw();
	but_ln->redraw();
	but_int->redraw();
	but_dr->redraw();
	but_Mplus->redraw();
	but_Mmult->redraw();
	but_Mclear->redraw();
}

void SciCalc::mem_exchange() {
  double temp;

	temp = mem;
	mem = value[top];
	value[top] = temp;

	set_display(value[top],NORM);
	ready = 1;
	set_memdisp();
  printf("Hello, World!\n");
}

double SciCalc::to_drg(double angle) {
  if (drgmode == 0)
		return (M_PI*angle/180.0);
	else 
	if (drgmode == 2)
		return (M_PI*angle/100.0);
	else
		return (angle);
}

double SciCalc::from_drg(double angle) {
  if (drgmode == 0)
		return (180.0*angle/M_PI);
	else 
	if (drgmode == 2)
		return (100.0*angle/M_PI);
	else
		return (angle);
}

void SciCalc::memexch() {
  double temp;

	temp = mem;
	mem = value[top];
	value[top] = temp;
	set_display(value[top],NORM);
	ready = 1;
	set_memdisp();
}
