//
// "$Id$"
//
// Header file for Fl_Text_Display class.
//
// Copyright Mark Edel.  Permission to distribute under the LGPL for
// the FLTK library granted by Mark Edel.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef FL_TEXT_DISPLAY_H
#define FL_TEXT_DISPLAY_H

#include "fl_draw.h"
#include "Fl_Group.h"
#include "Fl_Widget.h"
#include "Fl_Scrollbar.h"
#include "Fl_Text_Buffer.h"
#include "Fl_Int_List.h"

class FL_API Fl_Text_Display: public Fl_Group {
public:
    enum {
      NORMAL_CURSOR, CARET_CURSOR, DIM_CURSOR,
      BLOCK_CURSOR, HEAVY_CURSOR
    };

    enum {
        CURSOR_POS,
        CHARACTER_POS
    };

    // drag types- they match Fl::event_clicks() so that single clicking to
    // start a collection selects by character, double clicking selects by
    // word and triple clicking selects by line.
    enum {
      DRAG_CHAR = 0, 
	  DRAG_WORD = 1, 
	  DRAG_LINE = 2
    };
    friend void fl_text_drag_me(int pos, Fl_Text_Display* d);

    typedef void (*Unfinished_Style_Cb)(Fl_Text_Display *disp, int pos, void *arg);

    // style attributes
    enum {
      ATTR_NONE = 0,
      ATTR_UNDERLINE = 1,
      ATTR_HIDDEN = 2, //Not implement
	  ATTR_IMAGE = 3 
    };

    struct FL_API Style_Table_Entry {
      Fl_Color color;
      Fl_Font font;
      int size;
      unsigned attr;
	  Fl_Image *image;
    };

    Fl_Text_Display(int X, int Y, int W, int H, const char *l = 0);
    ~Fl_Text_Display();

    virtual void layout();
    virtual void draw();

    virtual int handle(int e);
    void buffer(Fl_Text_Buffer* buf);
    void buffer(Fl_Text_Buffer& buf) { buffer(&buf); }
    
    Fl_Text_Buffer* buffer() { return mBuffer; }
    Fl_Text_Buffer* style_buffer() { return mStyleBuffer; }

    void redisplay_range(int start, int end);
    void scroll(int topLineNum, int horizOffset);
    void insert(const char* text);
    void overstrike(const char* text);
    void insert_position(int newPos);
    int insert_position() { return mCursorPos; }
    int in_selection(int x, int y);
    void show_insert_position();
    int move_right();
    int move_left();
    int move_up(int lines=1);
    int move_down(int lines=1);

    int count_lines(int start, int end, bool start_pos_is_line_start);
    int line_start(int pos);
    int line_end(int pos, bool start_pos_is_line_start);
    int skip_lines(int startPos, int nLines, bool startPosIsLineStart);
    int rewind_lines(int startPos, int nLines);

    void next_word(void);
    void previous_word(void);
    void show_cursor(int b = 1);
    void hide_cursor() { show_cursor(0); }
    void cursor_style(int style);
    int word_start(int pos) { return buffer()->word_start(pos); }
    int word_end(int pos) { return buffer()->word_end(pos); }

    void highlight_data(Fl_Text_Buffer *styleBuffer,
                        Style_Table_Entry *styleTable,
                        int nStyles, char unfinishedStyle,
                        Unfinished_Style_Cb unfinishedHighlightCB,
                        void *cbArg);

    int position_style(int lineStartPos, int lineLen, int lineIndex,
                       int dispIndex);

    int wrapped_column(int row, int column);
    int wrapped_row(int row);

    // Wrap mode, wrap=[enable/disable]
    // wrap_margin=0 = continuos wrap. or character to wrap.
    // NOTE: wrap_margin=0 is EXTREMELY SLOW!
    void wrap_mode(int wrap, int wrap_margin);

    // Sets line number area, width=0 = disable...
    void set_linenumber_area(int left, int width);

    // Calculates fontmetrics
    void set_font();

    Fl_Color cursor_color() { return mCursor_color; }
    void cursor_color(Fl_Color c) { mCursor_color = c; }

protected:
    // Most (all?) of this stuff should only be called from layout() or
    // draw().
    // Anything with "vline" indicates thats it deals with currently
    // visible lines.

    void draw_text(int X, int Y, int W, int H);
    void draw_range(int start, int end);
    void draw_cursor(int, int);

    void draw_string(int style, int x, int y, int toX, const char *string,
                     int nChars);

    void draw_line_numbers(bool clearAll);

    void draw_vline(int visLineNum, int leftClip, int rightClip,
                    int leftCharIndex, int rightCharIndex);

    void clear_rect(int style, int x, int y, int width, int height);
    void display_insert();

    void offset_line_starts(int newTopLineNum);

    void calc_line_starts(int startLine, int endLine);

    void update_line_starts(int pos, int charsInserted, int charsDeleted,
                            int linesInserted, int linesDeleted, int *scrolled);

    void calc_last_char();

    int find_next_char(int pos);
    int find_prev_char(int pos);

    int position_to_line( int pos, int* lineNum );
    int string_width(const char* string, int length, int style);

    static void buffer_predelete_cb(int pos, int nDeleted, void* cbArg);

    static void buffer_modified_cb(int pos, int nInserted, int nDeleted,
                                   int nRestyled, const char* deletedText,
                                   void* cbArg);

    static void h_scrollbar_cb(Fl_Scrollbar* w, Fl_Text_Display* d);
    static void v_scrollbar_cb( Fl_Scrollbar* w, Fl_Text_Display* d);
    void update_v_scrollbar();
    void update_h_scrollbar();
    int measure_vline(int visLineNum);
    int longest_vline();
    int empty_vlines();
    int vline_length(int visLineNum);
    int xy_to_position(int x, int y, int PosType = CHARACTER_POS);

    void xy_to_rowcol(int x, int y, int* row, int* column,
                      int PosType = CHARACTER_POS);

    int position_to_xy(int pos, int* x, int* y);

    void maintain_absolute_top_line_number(int state);
    int get_absolute_top_line_number();
    void absolute_top_line_number(int oldFirstChar);
    int maintaining_absolute_top_line_number();
    void reset_absolute_top_line_number();

    int position_to_linecol(int pos, int* lineNum, int* column);
    void scroll_(int topLineNum, int horizOffset);

    void extend_range_for_styles(int* start, int* end);

    void find_wrap_range(const char *deletedText, int pos, int nInserted,
                         int nDeleted, int *modRangeStart, int *modRangeEnd,
                         int *linesInserted, int *linesDeleted);
    void measure_deleted_lines(int pos, int nDeleted);
    void wrapped_line_counter(Fl_Text_Buffer *buf, int startPos, int maxPos,
                              int maxLines, bool startPosIsLineStart,
                              int styleBufOffset, int *retPos, int *retLines,
                              int *retLineStart, int *retLineEnd,
                              bool countLastLineMissingNewLine = true);
    void find_line_end(int pos, bool start_pos_is_line_start, int *lineEnd,
                       int *nextLineStart);
    int measure_proportional_character(char c, int colNum, int pos);
    int wrap_uses_character(int lineEndPos);
    int range_touches_selection(Fl_Text_Selection *sel, int rangeStart,
                                int rangeEnd);

    int damage_range1_start, damage_range1_end;
    int damage_range2_start, damage_range2_end;
    int mCursorPos;
    int mCursorOn;
    int mCursorOldY;		/* Y pos. of cursor for blanking */
    int mCursorToHint;		/* Tells the buffer modified callback
                                   where to move the cursor, to reduce
                                   the number of redraw calls */
    int mCursorStyle;           /* One of enum cursorStyles above */
    int mCursorPreferredCol;    /* Column for vert. cursor movement */
    int mNVisibleLines;         /* # of visible (displayed) lines */
    int mNBufferLines;          /* # of newlines in the buffer */
    Fl_Text_Buffer* mBuffer;    /* Contains text to be displayed */
    Fl_Text_Buffer* mStyleBuffer; /* Optional parallel buffer containing
                                     color and font information */
    int mFirstChar, mLastChar;  /* Buffer positions of first and last
                                   displayed character (lastChar points
                                   either to a newline or one character
                                   beyond the end of the buffer) */
    int mContinuousWrap;        /* Wrap long lines when displaying */
    int mWrapMargin;            /* Margin in # of char positions for
                                   wrapping in continuousWrap mode */
    Fl_Int_List mLineStarts;
    int mTopLineNum;            /* Line number of top displayed line
                                   of file (first line of file is 1) */
    int mAbsTopLineNum;         /* In continuous wrap mode, the line
                                   number of the top line if the text
                                   were not wrapped (note that this is
                                   only maintained as needed). */
    int mNeedAbsTopLineNum;     /* Externally settable flag to continue
                                   maintaining absTopLineNum even if
                                   it isn't needed for line # display */

    int mHorizOffset;           /* Horizontal scroll pos. in pixels */
    int mTopLineNumHint;        /* Line number of top displayed line
                                   of file (first line of file is 1) */
    int mHorizOffsetHint;       /* Horizontal scroll pos. in pixels */
    int mVisibility;            /* Window visibility (see XVisibility event) */
    int mNStyles;               /* Number of entries in styleTable */
    Style_Table_Entry *mStyleTable; /* Table of fonts and colors for
                                       coloring/syntax-highlighting */
    char mUnfinishedStyle;      /* Style buffer entry which triggers
                                   on-the-fly reparsing of region */
    Unfinished_Style_Cb mUnfinishedHighlightCB; /* Callback to parse "unfinished"
                                                   regions */
    void* mHighlightCBArg;      /* Arg to unfinishedHighlightCB */

    int mMaxsize;

    int mFixedFontWidth;        /* Font width if all current fonts are
                                   fixed and match in width, else -1 */

    int mSuppressResync;        /* Suppress resynchronization of line
                                   starts during buffer updates */
    int mNLinesDeleted;         /* Number of lines deleted during
                                   buffer modification (only used
                                          when resynchronization is suppressed) */
    int mModifyingTabDistance;  /* Whether tab distance is being
                                   modified */

    int mMaxFontBound, mMinFontBound; //Min and Max bound of font (non-styled)

    int mOldWidth;

    Fl_Color mCursor_color;

    Fl_Scrollbar* mHScrollBar;
    Fl_Scrollbar* mVScrollBar;
    int dragPos, dragType, dragging;
    int display_insert_position_hint;
    struct { int x, y, w, h; } text_area;

    int mLineNumLeft, mLineNumWidth;  /* Line number margin and width */
};

#endif

//
// End of "$Id$".
//
