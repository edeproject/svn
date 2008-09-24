#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include <FL/fl_draw.H>

#include <edelib/Missing.h>
#include <edelib/MimeType.h>
#include <edelib/List.h>

#include "FileBrowser.h"

EDELIB_NS_BEGIN

static char some_buf[1024];

static char* header_long[] = {
	"Name",
	"Size",
	"Type",
	"Modified"
};

static char* header_short[] = {
	"Name",
	"Modified"
};

struct FileBrowserItem {
	String        filename;
	String        mime_type;
	unsigned long size;
	time_t        mtime;
	bool          is_file;
};

static const char* to_nice_size(long sz) {
	static char szbuff[256];
	if(sz < 1024)
		snprintf(szbuff, sizeof(szbuff), "%.0f bytes", (double)sz);
	else if(sz < 10240) // 1024 * 10
		snprintf(szbuff, sizeof(szbuff), "%.0f KB", (double)(sz / 1024));
	else if(sz < 102400) // 1024 * 100
		snprintf(szbuff, sizeof(szbuff), "%.0f KB", (double)(sz / 1024));
	else if(sz < 1048576) // 1024 * 1024
		snprintf(szbuff, sizeof(szbuff), "%.0f KB", (double)(sz / 1024));
	else if(sz < 10485760) // 1024 * 1024 * 10
		snprintf(szbuff, sizeof(szbuff), "%.0f MB", (double)(sz / 1048576));
	else if(sz < 1073741824) // 1024 * 1024 * 1024
		snprintf(szbuff, sizeof(szbuff), "%.0f MB", (double)(sz / 1048576));
	else
		snprintf(szbuff, sizeof(szbuff), "%.0f GB", (double)(sz / 1073741824));

	return (const char*)szbuff;
}

// anyhow this can be replaced with something better?
static void order_dirs_first(const char *dir, dirent*** files, int n) {
	int                     i;
	struct stat             st;
	list<dirent*>           dlist, flist;
	list<dirent*>::iterator it, it_end;

	for(i = 0; i < n; i++) {
		snprintf(some_buf, sizeof(some_buf), "%s/%s", dir, (*files)[i]->d_name);

		if(stat(some_buf, &st) == 0 && S_ISDIR(st.st_mode))
			dlist.push_back((*files)[i]);
		else
			flist.push_back((*files)[i]);
	}

	i = 0;
	it = dlist.begin(), it_end = dlist.end();
	for(; it != it_end && i < n; ++it, i++)
		(*files)[i] = *it;

	it = flist.begin(), it_end = flist.end();
	for(; it != it_end && i < n; ++it, i++)
		(*files)[i] = *it;
}

FileBrowser::FileBrowser(int X, int Y, int W, int H, const char *l) : TableBase(X, Y, W, H, l), files(0) {
}

FileBrowser::~FileBrowser() {
	clear();
}

int FileBrowser::load(const char *directory, FileBrowserFileSort *sort) {
	clear();

	char full_name[4096];
	int  n;

	if(!directory)
		return 0;

	dirent** df;

	n = edelib_scandir(directory, &df, 0, sort);
	if(n < 0)
		return 0;

	order_dirs_first(directory, &df, n);

	MimeType    mt;
	struct stat st;

	nfiles = n;
	files = new FileBrowserItem[nfiles];

	for(int i = 0; i < n; i++) {
		files[i].filename = df[i]->d_name;

		snprintf(full_name, sizeof(full_name), "%s/%s", directory, df[i]->d_name);

		if(mt.set(full_name))
			files[i].mime_type = mt.comment();

		if(stat(full_name, &st) == 0) {
			int sz = 0;
			bool f = false;

			files[i].mtime = st.st_mtime;

			if(S_ISREG(st.st_mode)) {
				sz  = st.st_size;
				f = true;
			} else {
				sz  = 0; // don't bother
				f = false;
			}

			files[i].size = sz;
			files[i].is_file = f;
		} else {
			files[i].mtime = 0; // ???
			files[i].size  = 0;
		}

		free(df[i]);
	}
	free(df);

	rows(nfiles);
	return nfiles;
}

void FileBrowser::clear(void) {
	delete [] files;
	nfiles = 0;
}

void FileBrowser::resize(int X, int Y, int W, int H) {
	TableBase::resize(X, Y, W, H);
}

void FileBrowser::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) {
	const char *l;

	switch(context) {
		case CONTEXT_ROW_HEADER:
		case CONTEXT_RC_RESIZE:
			break;

		case CONTEXT_COL_HEADER:
			fl_font(FL_HELVETICA, 12);
			fl_push_clip(X, Y, W, H);
			fl_draw_box(FL_UP_BOX, X, Y, W, H, row_header_color());

			fl_color(FL_BLACK);
			if(cols() == 4)
				l = header_long[C];
			else
				l = header_short[C];
			fl_draw(l, X + 5, Y, W, H, FL_ALIGN_LEFT);
			fl_pop_clip();
			break;
			
		case CONTEXT_CELL:
			fl_push_clip(X, Y, W, H);

			if(R > 0 && R % 2)
				fl_draw_box(FL_FLAT_BOX, X, Y, W, H, FL_LIGHT2);
			else
				fl_draw_box(FL_FLAT_BOX, X, Y, W, H, FL_WHITE);

			fl_pop_clip();

			if(C == 0)
				l = files[R].filename.c_str();
			else if(C == 1) {
				if(files[R].is_file)
					l = to_nice_size(files[R].size);
				else
					l = "";
			} else if(C == 2) {
				l = files[R].mime_type.c_str();
			} else if(C == 3) {
				struct tm* t = localtime(&files[R].mtime);
				strftime(some_buf, sizeof(some_buf), "%D", t);
				l = some_buf;
			}

			// text
			fl_font(FL_HELVETICA, 12);
			fl_push_clip(X + 5, Y + 3, W - 6, H - 6);
			fl_color(FL_BLACK);
			fl_draw(l, X + 5, Y + 3, W - 6, H - 6, FL_ALIGN_LEFT);
			fl_pop_clip();

		default:
			break;
	}
}

EDELIB_NS_END
