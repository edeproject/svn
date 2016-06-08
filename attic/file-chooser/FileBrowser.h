#ifndef __FILEBROWSER_H__
#define __FILEBROWSER_H__

#include <edelib/TableBase.h>
#include <edelib/Missing.h>
#include <edelib/List.h>

EDELIB_NS_BEGIN

typedef int (FileBrowserFileSort)(struct dirent**, struct dirent**);
struct FileBrowserItem;

class FileBrowser : public TableBase {
	private:
		FileBrowserItem* files;
		int              nfiles;
		list<int>        sel_rows;

		int  row_selected(int R);
		void add_selected(int R);
	protected:
		void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H);
		int handle(int event);

	public:
		FileBrowser(int X, int Y, int W, int H, const char *l = 0);
		~FileBrowser();
		void resize(int X, int Y, int W, int H);
		int load(const char *directory, FileBrowserFileSort *sort = edelib_alphasort);
		void clear(void);
};

EDELIB_NS_END

#endif
