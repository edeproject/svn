#ifndef FL_LISTITEM_H_
#define FL_LISTITEM_H_

#include "Fl.h"
#include "Fl_Font.h"
#include "Fl_Style.h"
#include "Fl_Callback.h"
#include "Fl_Image.h"
#include "Fl_Image_List.h"
#include "Fl_Int_List.h"
#include "Fl_String_List.h"
#include "Fl_Ptr_List.h"

class Fl_ListView;

class Fl_ListView_Item {
public:
    Fl_ListView_Item(const char *label1=0,
                     const char *label2=0,
                     const char *label3=0,
                     const char *label4=0,
                     const char *label5=0);
    virtual ~Fl_ListView_Item();

    bool selected();

    void *user_data() const { return user_data_; }
    void user_data(void* v) { user_data_ = v; }
    long argument() const   { return (long)user_data_; }
    void argument(long v)   { user_data_ = (void*)v; }

    uchar damage() const { return damage_; }
    void set_damage(uchar c) { damage_ = c; }

    Fl_ListView *parent() { return parent_; }
    void parent(Fl_ListView *l) { parent_ = l; }

    int column_width(int col);

    void check_columns(uint count);
    void columns(uint count);
    uint columns();
    
    virtual void draw_cell(int col, int width, bool selected);
    virtual void layout();

    const char *label();
    const char *label(int col);
    void label(int col, const char *text);
    void copy_label(int col, const char *txt);

    void image(Fl_Image *im) { image_ = im; }
    Fl_Image *image() { return image_; }
    void image(int col, Fl_Image &im) { image(&im); }

    int y() { return y_; }
    void y(int Y) { y_ = Y; }
    int h() { return h_; }
    void h(int H) { h_ = H; }

    void index(int i) { index_ = i; }
    int index() { return index_; }

protected:
    int y_, h_;
    int index_;
    uchar damage_;
    Fl_Image *image_;
    void *user_data_;
    Fl_ListView *parent_;

    Fl_Ptr_List attr_list;
    virtual void add_attr(int col);
};

class Fl_ListView_ItemExt : public Fl_ListView_Item {
public:
    Fl_ListView_ItemExt(const char *label1=0,
                        const char *label2=0,
                        const char *label3=0,
                        const char *label4=0,
                        const char *label5=0);
    virtual ~Fl_ListView_ItemExt();

    virtual void draw_cell(int col, int width, bool selected);
    virtual void layout();

    Fl_Flags flags(int col, int f);
    Fl_Flags flags(int col);
    Fl_Flags set_flag(int col, int f);
    Fl_Flags clear_flag(int col, int f);
    Fl_Flags invert_flag(int col, int f);

    Fl_Font label_font(int col);
    Fl_Labeltype label_type(int col);
    int label_size(int col);
    Fl_Color label_color(int col);

    void label_size(int col, int size);
    void label_font(int col, Fl_Font font);
    void label_color(int col, Fl_Color color);
    void label_type(int col, Fl_Labeltype type);

    void label_font(int col, Fl_Font font, int size) { label_font(col, font); label_size(col, size); }

    void image(int col, Fl_Image *im);
    void image(int col, Fl_Image &im);
    Fl_Image *image(int col);

protected:
    void add_attr(int col);
private:
    void draw_label(const char *label, int X, int Y, int W, int H, Fl_Flags flags, void *a);
};

class Fl_ListItem_List : public Fl_Ptr_List {
public:
    Fl_ListItem_List() : Fl_Ptr_List() { }
    void append(Fl_ListView_Item *item) { Fl_Ptr_List::append((void *)item); }
    void prepend(Fl_ListView_Item *item) { Fl_Ptr_List::prepend((void *)item); }
    void insert(uint pos, Fl_ListView_Item *item) { Fl_Ptr_List::insert(pos, (void *)item); }
    void replace(uint pos, Fl_ListView_Item *item) { Fl_Ptr_List::replace(pos, (void *)item); }
    void remove(uint pos) { Fl_Ptr_List::remove(pos); }
    bool remove(Fl_ListView_Item *item) { return Fl_Ptr_List::remove((void *)item); }
    int index_of(const Fl_ListView_Item *w) const { return Fl_Ptr_List::index_of((void*)w); }
    Fl_ListView_Item *item(uint index) const { return (Fl_ListView_Item*)Fl_Ptr_List::item(index); }
    Fl_ListView_Item **data() { return (Fl_ListView_Item**)items; }
    Fl_ListView_Item *operator [](uint ind) const { return (Fl_ListView_Item *)items[ind]; }
protected:
    void free_item(Fl_Ptr_List_Item item) { delete (Fl_ListView_Item*)(item); }
};

#endif
