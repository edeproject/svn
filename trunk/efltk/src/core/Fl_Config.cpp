// Fl_Config.cpp: implementation of the Fl_Config class.
//
//////////////////////////////////////////////////////////////////////

#include <efltk/vsnprintf.h>
#include <efltk/Fl_Config.h>
#include <efltk/filename.h>
#include <efltk/fl_utf8.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include <config.h>

#ifdef _WIN32

# include <io.h>
# include <direct.h>
# include <windows.h>
# define access(a,b) _access(a,b)
# define mkdir(a,b) _mkdir(a)
# define R_OK 4

#else

# include <unistd.h>

#endif /* _WIN32 */

static int is_path_rooted(const char *fn)
{
    /* see if an absolute name was given: */
#ifdef _WIN32
    if (fn[0] == '/' || fn[0] == '.' || fn[0] == '\\' || fn[1]==':')
#else
    if (fn[0] == '/' || fn[0] == '.')
#endif
        return 1;
    return 0;
}

// recursively create a path in the file system
static bool makePath( const char *path ) {
    if(access(path, 0)) {
        const char *s = strrchr( path, slash );
        if ( !s ) return 0;
        int len = s-path;
        char *p = (char*)malloc( len+1 );
        memcpy( p, path, len );
        p[len] = 0;
        makePath( p );
        free( p );
        return ( mkdir( path, 0777 ) == 0 );
    }
    return true;
}

// strip the filename and create a path
static bool makePathForFile( const char *path )
{
    const char *s = strrchr( path, slash );
    if ( !s ) return false;
    int len = s-path;
    char *p = (char*)malloc( len+1 );
    memcpy( p, path, len );
    p[len] = 0;
    bool ret=makePath( p );
    free( p );
    return ret;
}

char *get_sys_dir() {
#ifndef _WIN32
	return CONFIGDIR;
#else
	static char path[FL_PATH_MAX];
    HKEY hKey;
    if(RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion", 0, KEY_READ, &hKey)==ERROR_SUCCESS)
    {
        DWORD size=4096;
        LONG result=RegQueryValueExW(hKey, L"CommonFilesDir", NULL, NULL, (LPBYTE)path, &size);		
        RegCloseKey(hKey);
        return path;
    }
	return "C:\\EFLTK\\";
#endif
}

char *Fl_Config::find_config_file(const char *filename, bool create, ConfMode mode)
{
    static char path[4096];

    if(is_path_rooted(filename)) {
        strncpy(path, filename, sizeof(path));
        return (create || !access(path, R_OK)) ? path : 0;
    }
    if(mode==USER) {
        char *cptr = fl_get_homedir();
        char *ret=0;
        if(cptr) {
            snprintf(path, sizeof(path)-1, "%s%c%s%c%s", cptr, slash, ".ede", slash, filename);
            if(create || !access(path, R_OK)) {
                ret = path;
            }
            delete []cptr;
            return ret;
        }
		return 0;
    } else {
        snprintf(path, sizeof(path)-1, "%s%c%s", get_sys_dir(), slash, filename);
        return (create || !access(path, R_OK)) ? path : 0;
    }
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define L(item) ((Line*)item)
#define S(item) ((Section*)item)

Fl_Config::Fl_Config(const char *vendor, const char *application, ConfMode mode)
{
    cur_sec = 0;
    changed=false;
    _error = 0;

    vendor_ = vendor?strdup(vendor):0;
    app_    = application?strdup(application):0;
	filename_ = 0;

    if(app_) {
        const char *file=0;
        char tmp[FL_PATH_MAX];
#ifdef _WIN32
		if(mode==SYSTEM) snprintf(tmp, sizeof(tmp)-1, "%s%c%s.conf", app_, slash, app_);
		else
#endif
        snprintf(tmp, sizeof(tmp)-1, "apps%c%s%c%s.conf", slash, app_, slash, app_);
        file = find_config_file(tmp, true, mode);
        if(file) {
            bool ret = makePathForFile(file);
            if(ret) {
                filename_ = strdup(file);
                read_file(true);
            } else
                _error = CONF_ERR_FILE;
        } else
            _error = CONF_ERR_FILE;
    } else
        _error = CONF_ERR_FILE;
}

Fl_Config::Fl_Config(const char *filename, bool read, bool create)
{
    vendor_ = 0;
    app_    = 0;

    _error = 0;
    filename_ = filename?strdup(filename):0;
    cur_sec = 0;
    changed=false;

    if(create && filename_) {
        makePathForFile(filename_);
    }

    if(read) read_file(create);
}

Fl_Config::~Fl_Config()
{
    flush();
    uint n;
    for(n=0; n<lines.size(); n++)
        delete (Line*)lines[n];
    for(n=0; n<sections.size(); n++)
        delete (Section*)sections[n];
    if(filename_) delete []filename_;
}

/* get error string associated with error number */
const char *Fl_Config::strerror(int error)
{
    switch(error)
    {
    case CONF_SUCCESS: return "successful completion";
    case CONF_ERR_FILE: return "could not access config file";
    case CONF_ERR_SECTION: return "config file section not found";
    case CONF_ERR_KEY: return "key not found in section";
    case CONF_ERR_MEMORY: return "could not allocate memory";
    case CONF_ERR_NOVALUE: return "invalid value associated with key";
    default: return "unknown error";
    }
}

bool Fl_Config::read_file(bool create)
{
    bool error = false;
    if(filename_) {
        if(create && !fl_file_exists(filename_)) {
            FILE *f = fl_fopen(filename_, "w+");
            if(f) {
                fputs("\n",f);
                fclose(f);
            } else error=true;
        }
    } else error=true;

    if(error) {
        _error = CONF_ERR_FILE;
        return false;
    }

    // If somebody calls this function two times, we
    // need to clean earlier section list...
    uint n;
    for(n=0; n<lines.size(); n++)
        delete (Line*)lines[n];
    for(n=0; n<sections.size(); n++)
        delete (Section*)sections[n];
    lines.clear();
    sections.clear();

    /////

    struct stat fileStat;
    stat(filename_, &fileStat);
    unsigned int size = fileStat.st_size;
    if(size == 0) {
        _error = 0;
        return true;
    }

    FILE *fp = fl_fopen(filename_, "r");
    if(!fp) {
        //fprintf(stderr, "fp == 0: %s\n", filename_);
        _error = CONF_ERR_FILE;
        return false;
    }

    char *buffer, *lines, *tmp;
    if( (buffer = new char[size]) == 0) {
        _error = CONF_ERR_MEMORY;
        return false;
    }

    unsigned int readed = fread(buffer, 1, size, fp);
    if(readed <= 0) {
        delete []buffer;
        fclose(fp);
        _error = CONF_ERR_FILE;
        return false;
    }
    fclose(fp);

    Section *section = 0;
    char *ptr = buffer;
    lines = strtok(ptr, "\n");
    while(lines != 0)
    {
        if(lines)
        {
            while(*lines && (*lines==' '||*lines=='\t')) lines++;

            if (lines[0] == '[')
            {
                if ((tmp = strchr(lines, ']')))
                {
                    *tmp = '\0';
                    section = create_section(&lines[1]);
                }
            }
            else if (lines[0] != '#')
            {
                tmp = strchr(lines, '=');
                if(!tmp) tmp = strchr(lines, ':');
                if(tmp) {
                    *tmp = '\0';
                    tmp++;
                    create_string(section, lines, tmp);
                }
            }
        }

        lines = strtok(NULL,"\n");
    }

    if(buffer)
        delete []buffer;

    _error = 0;
    changed=false;
    return true;
}

void Fl_Config::write_section(int indent, FILE *fp, Section *sec)
{
    Section *child;
    uint n;
    for(int a=0; a<indent; a++) fprintf(fp, " ");

    fprintf(fp, "[%s%s]\n", sec->path, sec->name);

    for(n=0; n<sec->lines.size(); n++) {
        Line *l = L(sec->lines[n]);
        if(l && l->key) {
            for(int a=0; a<indent; a++) fprintf(fp, " ");
            fprintf(fp, "  %s=%s\n", l->key, l->value?l->value:"");
        }
    }

    fprintf(fp, "\n");

    for(n=0; n<sec->sections.size(); n++) {
        child = S(sec->sections[n]);
        write_section(indent+2, fp, child);
    }
}

bool Fl_Config::flush()
{
    if(!changed) return true;

    if(!filename_) {
        _error = CONF_ERR_FILE;
        return false;
    }
    FILE *file = fl_fopen(filename_, "w+");
    if(!file) {
        _error = CONF_ERR_FILE;
        return false;
    }

    fprintf( file, "# EFLTK Configuration - Version %f\n", FL_VERSION);
    if(vendor_) fprintf( file, "# Vendor: %s\n", vendor_ );
    if(app_)    fprintf( file, "# Application: %s\n", app_ );
    fprintf( file, "\n");

    uint n;
    for(n=0; n<lines.size(); n++) {
        Line *l = L(lines[n]);
        if(l && l->key) {
            fprintf(file, "%s=%s\n", l->key, l->value?l->value:"");
        }
    }
    fprintf(file, "\n");

    for(n=0; n<sections.size(); n++) {
        Section *s = S(sections[n]);
        write_section(0, file, s);
    }

    fclose(file);
    _error = 0;

    changed=false;

    return true;
}

Line *Fl_Config::create_string(Section *section, const char * key, const char * value)
{
    if(!key || !*key) return 0;

    Line *line = new Line(key, value);

    line->key = fl_trim(line->key);
    if(line->value) {
        line->value = fl_trim(line->value);
    }
    _error=0;

    if(section) section->lines.append(line);
    else lines.append(line);
    return line;
}

Section *Fl_Config::create_section(const char *name)
{
    if(!name) return 0;
    Section *section = find_section(name, true);
    if(section) return section;

    Section *parent=0;
    char secpath[1024]={0}, secname[256]={0};
    int namelen=strlen(name);
    const char *ptr=name+namelen-1;
    char ch;
    bool gotname=false;
    int namebegin=namelen;
    while((ch=*ptr--)) {
        namebegin--;
        if(ch=='/') {
            if(!gotname) {
                strncpy(secname, name+(namebegin+1), namelen-namebegin-1);
                gotname = true;
                break;
            }
        }
    }

    if(!gotname) strncpy(secname, name, sizeof(secname)-1);
    if(gotname) strncpy(secpath, name, namebegin+1);

    SectionList *list;

    if(*secpath) {
        parent = find_section(secpath, false);

        if(!parent) {
            // Add all parent sections!

            char tmp[256]={0}, *tmpptr = tmp;
            char tmppath[1024]={0};
            char *ptr = secpath; int pos=0;
            while( (ch=*ptr++) ) {
                if(ch=='/') {
                    if(parent) list = &parent->sections;
                    else list = &sections;

                    parent = new Section(tmp, tmppath, parent);
                    list->append(parent);

                    memset(tmp, 0, sizeof(tmp));
                    tmpptr = tmp;

                    strncpy(tmppath+pos, tmpptr, strlen(tmpptr));
                    pos+=strlen(tmpptr);
                    strncpy(tmppath+pos, "/", 1);
                    pos++;

                    continue;
                }
                *tmpptr++ = ch;
            }
        }

    }

    if(parent) list = &parent->sections;
    else list = &sections;

    section = new Section(secname, secpath, parent);
    list->append(section);

    _error = 0;

    return section;
}

Section *Fl_Config::find_section(const char *path, bool perfect_match)
{
    if(!path || !*path) return 0;

    int last_slash = -1, pos=0;

    Section *section=0;
    char sec[256]={0};
    const char *pathptr = path;
    char *secptr = sec;
    char ch;
    while((ch=*pathptr++)) {
        pos++;
        if(ch=='/')
        {
            last_slash=pos;
            Section *tmp = find_section(section, sec, false);
            if(!tmp) if(perfect_match) return 0; else break;
            section = tmp;

            memset(sec, 0, sizeof(sec));
            secptr = sec;
        } else
            *secptr++ = ch;
    }

    if(last_slash==-1) {
        return find_section(0, path, false);
    } else
    if(last_slash<pos) {
        Section *tmp = find_section(section, sec, false);
        if(!tmp&&perfect_match) return 0;
        else if(tmp) section = tmp;
    }

    return section;
}

Section *Fl_Config::find_section(Section *sec, const char *name, bool recursive)
{
    SectionList *list = sec?&sec->sections:&sections;
    Section *s;

    for(uint n=0; n<list->size(); n++) {
        s = S(list->item(n));
        if(!strcmp(s->name, name)) {
            _error = 0;
            return s;
        }
        if(recursive) {
            s = find_section(s, name, recursive);
            if(s) return s;
        }
    }

    _error = CONF_ERR_SECTION;
    return 0;
}

Line *Fl_Config::find_string(Section *section, const char *key)
{
    if(key) {
        for(uint n=0; n<section->lines.size(); n++) {
            Line *line = L(section->lines[n]);
            if( line->key && !strcmp(line->key, key) ) {
                _error = 0;
                return line;
            }
        }
    }

    _error = CONF_ERR_KEY;
    return 0;
}

void Fl_Config::remove_key(const char *section, const char *key)
{
    if(key) {
        Section *sect;
        Line *line;

        if((sect = find_section(section, true)) != 0) {
            if ((line = find_string(sect, key)) != 0) {
                sect->lines.remove(line);
                delete line;
                _error = 0;
                changed = true;
                return;
            }
        }
    }
    _error = CONF_ERR_KEY;
}

void Fl_Config::remove_sec(const char *section)
{
    if(section) {
        Section *sect;
        if((sect = find_section(section, true)) != 0) {
            if(sect->parent) {
                sect->parent->sections.remove(sect);
            } else {
                sections.remove(sect);
            }
            delete sect;
            _error = 0;
            changed = true;
            return;
        }
    }

    _error = CONF_ERR_SECTION;
}

#include "fl_internal.h"

// Converts locale decimal to '.' e.g. "1,5" to "1.5"
char *double_to_str(double v)
{
    static char ret[128];
    snprintf(ret, sizeof(ret)-1, "%g", v);

    lconv *locale_conv = localeconv();
    char decimal = locale_conv->decimal_point[0];
    if(decimal=='.') return ret;

    char *ptr = ret;
    while(*ptr++) if(*ptr==decimal) *ptr = '.';
    return ret;
}

// Reads double with '.' as decimal pointer
double str_to_double(const char *v)
{
    lconv *locale_conv = localeconv();
    char decimal = locale_conv->decimal_point[0];
    if(decimal=='.') return atof(v);

    static char ret[128];
    double value=0;
    const char *ptr = v;
    char *ret_ptr = ret;

    while(*ptr) {
        char c = *ptr++;
        if(c=='.') {
            break;
        }
        *ret_ptr++ = c;
    }
    *ret_ptr = '\0';
    value=double(atoi(ret));

    ret_ptr = ret;
    while(*ptr) {
        char c = *ptr++;
        *ret_ptr++ = c;
    }
    *ret_ptr = '\0';

    return value+double(atoi(ret))/10;
}

/*
 *  Read functions
 */

int Fl_Config::_read_string(Section *s, const char *key, char *ret, const char *def_value, int size)
{
    if(!key || !s) {
        strncpy(ret, def_value?def_value:"\0", size);
        _error = !key ? CONF_ERR_KEY : CONF_ERR_SECTION;
        return _error;
    }

    Line *l = find_string(s, key);
    if(l&&l->value)
    {
        char *tmp = strchr(l->value, '#');
        if(tmp) *tmp = '\0';
        strncpy(ret, l->value, size);
        return (_error = CONF_SUCCESS);
    }

    strncpy(ret, def_value?def_value:"\0", size);
    return (_error = CONF_ERR_KEY);
}

int Fl_Config::_read_string(Section *s, const char *key, char *&ret, const char *def_value)
{
    if(!key || !s) {
        ret = def_value?strdup(def_value):0;
        _error = !key ? CONF_ERR_KEY : CONF_ERR_SECTION;
        return _error;
    }

    Line *l = find_string(s, key);
    if(l&&l->value)
    {
        char *tmp = strchr(l->value, '#');
        if(tmp) *tmp = '\0';
        ret = strdup(l->value);
        return (_error = CONF_SUCCESS);
    }

    ret = def_value?strdup(def_value):0;
    return (_error = CONF_ERR_KEY);
}

int Fl_Config::_read_long(Section *s, const char *key, long &ret, long def_value)
{
    char tmp[128];
    if(!_read_string(s, key, tmp, 0, sizeof(tmp)-1)) {
        ret = *tmp?atol(tmp):def_value;
    } else
        ret = def_value;
    return _error;
}

int Fl_Config::_read_int(Section *s, const char *key, int &ret, int def_value)
{
    char tmp[128];
    if(!_read_string(s, key, tmp, 0, sizeof(tmp)-1)) {
        ret = *tmp?atoi(tmp):def_value;
    } else
        ret = def_value;
    return _error;
}

int Fl_Config::_read_float (Section *s, const char *key, float &ret, float def_value)
{
    char tmp[128];
    if(!_read_string(s, key, tmp, 0, sizeof(tmp)-1)) {
        ret = (float)str_to_double(tmp);
    } else
        ret = def_value;
    return _error;
}

int Fl_Config::_read_double(Section *s, const char *key, double &ret, double def_value)
{
    char tmp[128];
    if(!_read_string(s, key, tmp, 0, sizeof(tmp)-1)) {
        ret = str_to_double(tmp);
    } else
        ret = def_value;
    return _error;
}

int Fl_Config::_read_bool(Section *s, const char *key, bool &ret, bool def_value)
{
    char tmp[128];
    if(_read_string(s, key, tmp, 0, sizeof(tmp)-1)) {
        ret = def_value;
        return _error;
    }

    if( !strcasecmp(tmp, "TRUE") ||
       !strcasecmp(tmp, "YES") ||
       !strcasecmp(tmp, "ON") ||
       !strcasecmp(tmp, "1") ) {
        ret = true;
    } else if( !strcasecmp(tmp, "FALSE") ||
              !strcasecmp(tmp, "NO") ||
              !strcasecmp(tmp, "OFF") ||
              !strcasecmp(tmp, "0") ) {
        ret = false;
    } else {
        _error = CONF_ERR_NOVALUE;
        ret = def_value;
    }
    return _error;
}

int Fl_Config::_read_color(Section *s, const char *key, Fl_Color &ret, Fl_Color def_value)
{
    char tmp[128];
    if(_read_string(s, key, tmp, 0, sizeof(tmp)-1)) {
        ret = def_value;
        return _error;
    }

    int r=0,g=0,b=0;
    if(sscanf(tmp, "RGB(%d,%d,%d)", &r, &g, &b)!=3) {
        ret = def_value;
        return (_error = CONF_ERR_NOVALUE);
    }
    ret = fl_rgb(r,g,b);
    return _error;
}

/*
 *  Write functions
 */

int Fl_Config::_write_string(Section *s, const char *key, const char *value)
{
    if(!s) return (_error = CONF_ERR_SECTION);
    if(!key) return (_error = CONF_ERR_KEY);

    Line *line;
    if((line = find_string(s, key)))
    {
        if(line->value) delete []line->value;
        line->value = value?strdup(value):0;
    }
    else
        create_string(s, key, value);

    changed=true;

    return (_error=CONF_SUCCESS);
}

int Fl_Config::_write_long(Section *s, const char *key, const long value)
{
    char tmp[128];
    snprintf(tmp, sizeof(tmp)-1, "%ld", value);
    return _write_string(s, key, tmp);
}

int Fl_Config::_write_int(Section *s, const char *key, const int value)
{
    char tmp[128];
    snprintf(tmp, sizeof(tmp)-1, "%d", value);
    return _write_string(s, key, tmp);
}

int Fl_Config::_write_float(Section *s, const char *key, const float value)
{
    return _write_string(s, key, double_to_str(value));
}

int Fl_Config::_write_double(Section *s, const char *key, const double value)
{
    return _write_string(s, key, double_to_str(value));
}

int Fl_Config::_write_bool(Section *s, const char *key, const bool value)
{
    if(value) return _write_string(s, key, "1");
    return _write_string(s, key, "0");
}

int Fl_Config::_write_color(Section *s, const char *key, const Fl_Color value)
{
    uint8 r,g,b;
    fl_get_color(value, r,g,b);
    char tmp[32];
    snprintf(tmp, sizeof(tmp)-1, "RGB(%d,%d,%d)", r,g,b);
    return _write_string(s, key, tmp);
}
