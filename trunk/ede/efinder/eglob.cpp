/*
 * $Id$
 *
 * Efinder, file search tool
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */


#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <efltk/Fl.h>
#include <efltk/fl_ask.h>
#include <efltk/Fl_Item.h>
#include <efltk/Fl_Locale.h>
#include <efltk/Fl_Image.h>
#include <efltk/Fl_ListView_Item.h>

#include <edeconf.h>

#include "efinder.h"
#include "eglob.h"

#include "icons/block_dev.xpm"
#include "icons/char_dev.xpm"
#include "icons/dir_close.xpm"
#include "icons/exe.xpm"
#include "icons/sexe.xpm"
#include "icons/fifo.xpm"
#include "icons/page.xpm"
#include "icons/page_lnk.xpm"
#include "icons/socket.xpm"

#define GLOB "glob"

#define SHELL "/bin/sh"
// TODO: ensure that find and grep are GNU
#define GREP "grep"
#define FIND "find"


#define TRUE 1
#define FALSE 0
#define MAX_ARG 100

static int cancelled = FALSE;
static int pfd[2];               /* the pipe */
static pid_t Gpid;               /* glob pid, to be able to cancel search */
static short int findCount;      /* how many files found */
static short int fileLimit = 100;
static int type=0x0;

static Fl_Image block_dev_pix = *Fl_Image::read_xpm(0, (const char **)block_dev_xpm);
static Fl_Image char_dev_pix  = *Fl_Image::read_xpm(0, (const char **)char_dev_xpm);
static Fl_Image dir_close_pix = *Fl_Image::read_xpm(0, (const char **)dir_close_xpm);
static Fl_Image exe_pix       = *Fl_Image::read_xpm(0, (const char **)exe_xpm);
static Fl_Image sexe_pix      = *Fl_Image::read_xpm(0, (const char **)sexe_xpm);
static Fl_Image fifo_pix      = *Fl_Image::read_xpm(0, (const char **)fifo_xpm);
static Fl_Image page_pix      = *Fl_Image::read_xpm(0, (const char **)page_xpm);
static Fl_Image page_lnk_pix  = *Fl_Image::read_xpm(0, (const char **)page_lnk_xpm);
static Fl_Image socket_pix    = *Fl_Image::read_xpm(0, (const char **)socket_xpm);



static char *ftypes_find[FILE_TYPES_NR] =
{
    "bcdpfls",
    "f",
    "d",
    "l",
    "s",
    "b",
    "c",
    "p",
    NULL
};

void
jam(char *file, Fl_Menu_ *optmenu)
{
    FILE *archie;
    char line[256];
    char *s,*r,*t = "Anyone";

    archie=fopen(file,"r");
    if (archie==NULL) return;

    optmenu->add("Anyone");

    while (!feof(archie) && (fgets(line,255,archie)))
    {
        if (feof(archie)) break;
        line[255]=0;
        if ((line[0]=='#')||(strchr(line,':')==NULL)) continue;
        r=strtok(line,":"); if (!r)  continue;
        s=strchr(r+strlen(r)+1,':')+1;if (!s)  continue;
        s=strtok(s,":");if (!s)  continue;
        t=(char *)malloc(strlen(s)+1);
        strcpy(t,s);
        optmenu->add(r);
    }
    fclose(archie);
    return;
}


void
toggle_permission(long data)
{
    int flag;
    flag = (int ) ((long)data);
    type ^= (flag&07777);
}


static void
abort_glob()
{
    if (Gpid)
    {
        kill (Gpid, SIGKILL);    //agressive
    }
    // TODO: flush buffer (so we don't get continuation of previous results)
}


static void
abort_glob1()
{
    if (Gpid)
    {
        kill (Gpid, SIGTERM);    // nonagressive
    }
}


void
GlobWait(void *data)
{
    int status;
    int childPID;
    childPID = (int) ((long)data);
    //fprintf(stderr,"waiting\n");
    waitpid (childPID, &status, WNOHANG);
    if (WIFEXITED (status))
    {
        //fprintf(stderr,"waiting done\n");
        char result[128];
        snprintf(result,127,"Finished search. %d results found.",findCount);
        statusLine->copy_label(result);
        statusLine->redraw();
        searchButton->activate();
        stopButton->deactivate();
        return;
    }
    Fl::add_timeout(2, GlobWait, (void*)childPID);
    return;
}


void findCB()
{
    char *argument[MAX_ARG];
    char *filter, *s;
    int sizeG, sizeM;
    int childPID;

    cancelled = FALSE;

    // Kill any existing search process
    if (Gpid)
    {
        kill (Gpid, SIGHUP);
        Gpid = 0;
    }

    // Clear the results pane
    searchList->clear();
    statusLine->label(_("Searching..."));
    statusLine->redraw();

    // Disable the start button
    searchButton->deactivate();
    // Enable the stop button
    stopButton->activate();

    findCount = 0;
    fileLimit = (int) fileLimitValue->value();	// Max number of results


    // The following code prepares parameters for find and grep
    int i = 0;

    // FIND comes first
    argument[i++] = FIND;

    // Path is always the first parameter
    char* path = (char*) pathInput->value();		// Start directory for search
    // This should only happen if user manually edits path field:
    if (strlen(path)==0)
        path = "/"; 
    else if (path[0]!='/')
        path = strcat("/",path); // blah...

    argument[i++] = path;


    // The following parameters must come before others

    // Search subdirectories?
    if (!recursiveCheck->value()) {
        argument[i++] = "-maxdepth";
	argument[i++] = "1";
    }

    // Remain on same file system
    if (stayOnSingleCheck->value())
        argument[i++] = "-mount";


    // Filename and other parameters

    filter = (char*) filterInput->value();	// File name definition
    if (strlen(filter)!=0) {
        argument[i++] = "-name";
        char tmp[256];
        snprintf(tmp,255,"*%s*",filter);
        argument[i++] = tmp;
    }

    // Show hidden files
    if (!findHidden->value()) {
        argument[i++] = "-not";
        argument[i++] = "-path";
        argument[i++] = "*/.*";
    }

    // File type match
    s = (char*) fileTypeBrowser->value();
    // if j==0 (Any type) we don't need to do anything
    for (int j = 1; j<FILE_TYPES_NR-1; j++)
    {
        if (strcmp (s, ftypes[j]) == 0)
        {
            argument[i++] = "-type";
            argument[i++] = ftypes_find[j];
            break;
        }
    }

    // Permissions
    if (considerPermValue->value()) {
        // TODO: Permission handling in find is very weird so I'll skip it here
    }

    // File ownership
    if (considerUserValue->value()) {
        if (userIdChoice->value() && (strcmp(userIdChoice->value(),"Anyone") != 0))
        {
            argument[i++] = "-user";
            //argument[i++] = (char*)userIdChoice->text(userIdChoice->value());
            argument[i++] = (char*)userIdChoice->value();
        }
        if (groupIdChoice->value() && (strcmp(groupIdChoice->value(),"Anyone") != 0))
        {
            argument[i++] = "-group";
            //argument[i++] = (char*)groupIdChoice->text(groupIdChoice->value());
            argument[i++] = (char*)groupIdChoice->value();
        }
    }


    // File size
    if (considerSizeValue->value())
    {
        sizeG = (int)sizeGValue->value();
        sizeM = (int)sizeMValue->value();

        if ((sizeM <= sizeG)&&(sizeM > 0))
        {
            fl_alert(_("Size limits are impossible."));
            return;
        }

        // This doesn't work with find (need to read man page)

        /*char sizeG_s[64], sizeM_s[64];
        if (sizeG > 0)
        {
            argument[i++] = "-size";
            snprintf (sizeG_s, sizeof(sizeG_s)-1, "%d", sizeG);
            argument[i++] = sizeG_s;
        }
        if (sizeM > 0)
        {
            argument[i++] = "-size";
            snprintf (sizeM_s, sizeof(sizeM_s)-1,  "%d", sizeM);
            argument[i++] = sizeM_s;
        }*/
    }


    // File time (access, creation, change...)
    if (considerTimeValue->value())
    {
        // find only works with days... need to fix this manually

/*        if (modifiedRadio->value()) argument[i++] = "-M";
        if (accessedRadio->value()) argument[i++] = "-A";
        if (changedRadio->value()) argument[i++] = "-C";
        if (hours > 0)
        {
            if (minutesRadio->value()) argument[i++] = "-k";
            if (hoursRadio->value()) argument[i++] = "-h";
            if (daysRadio->value()) argument[i++] = "-d";
            if (mounthsRadio->value()) argument[i++] = "-m";

            snprintf (hours_s, sizeof(hours_s)-1, "%d", hours);
            argument[i++] = hours_s;
        }*/
    }


    // We are now handling output control and counting
        /*if (outputCountCheck->value())
            argument[i++] = "-c";*/



    // now GREP - first lets see if its needed
    char* token = (char*) containsInput->value();

    if (strlen(token) > 0) {
        argument[i++] = "-exec";
        argument[i++] = GREP;

        // All output is handled by find
        argument[i++] = "-s";
        argument[i++] = "-q";

        // This we do if silly user wants to search directories or devices
        argument[i++] = "-d";
        argument[i++] = "skip";
        argument[i++] = "-D";
        argument[i++] = "skip";

        // Should we search inside binary files? (usually not)
        if (doNotLookIntoBinaryCheck->value())
            argument[i++] = "-I";
        else
            argument[i++] = "-a";

        // Use Perl compatible regexp's
        // TODO: check for proper escaping
        if (useRegexpCheck->value())
            argument[i++] = "-P";

        // Case sensitive search
        if (caseSensitiveCheck->value())
            argument[i++] = "-i";

        // Match only whole words
        if (wholeWordsOnly->value())
            argument[i++] = "-w";

        // What we search
        argument[i++] = token;

        // This will be replaced with filenames
        argument[i++] = "{}";

        // This ends grep params list
        argument[i++] = ";";
    }



    // Print the filename in format understood by process_find_messages()
    argument[i++] = "-printf";
    argument[i++] = "\%p:1\\n";

    // End of arguments list... must be null
    argument[i] = (char *) 0;



    //DEBUG 
    for (int j=0;j<i;j++) fprintf (stderr, "%s ",argument[j]);fprintf (stderr,"\n");



    // Great! Now lets fork the search process

    Gpid = 0;
    childPID=fork ();

    if (!childPID)
    {
        dup2 (pfd[1], 1);        /* assign child stdout to pipe */
        close (pfd[0]);          /* not used by child */
        execvp (FIND, argument);
        perror ("exec");
        _exit (127);             /* child never get here */
    }
    Gpid = childPID;
    Fl::add_timeout(2, GlobWait, (void*)childPID);

// This apparently do nothing!?
/*    char command[128];
    char *textos[6];
    strcpy (command, argument[0]);
    for (int j = 1; j < i; j++)
    {
        strcat (command, " ");
        strcat (command, argument[j]);
    }

    if (strlen(token)) textos[0] = token;  else textos[0] = "";
    if (strlen(filter)) textos[1] = filter;  else textos[1] = "";
    if (strlen(path)) textos[2] = path;  else textos[2] = "";
    textos[3] = textos[4] =  textos[5] = "";*/

    int *data;
    data=(int *)malloc(3*sizeof(int));
    data[0]=data[1]=data[2]=0;
}


void stopSearch()
{
    cancelled = TRUE;
    char result[128];
    snprintf(result,127,_("Search stopped. %d results found"),findCount);
    statusLine->copy_label(result);
    statusLine->redraw();
    searchButton->activate();
    stopButton->deactivate();
    abort_glob1();
}

void pauseSearch()
{
    // unimplemented
}


void
process_find_messages(int, void*)
{
    static char *buffer, line[256];
    static int nothing_found;
    char *filename;

    buffer = line;

    while (1)
    {
        if (!read (pfd[0], buffer, 1))
            return;
        if (cancelled)
            return;
        if (buffer[0] == '\n')
        {
		// TODO: this part of code isn't accessed anymore
            buffer[1] = (char) 0;
            if (strncmp(line, "GLOB DONE=", strlen ("GLOB DONE=")) == 0)
            {
                fl_message(_("Search finished."));
                Gpid = 0;
                if (nothing_found)
                    fl_message(_("Nothing found."));
                if (findCount)
                {
                    char mess[128];
                    snprintf(mess, 127, _("Found %d files."), findCount);
                    if (findCount >= fileLimit)
                        fl_message(_("Interrupted because maximum limit exceded."));
                    fl_alert(mess);
                }
                return;
            }

            if ((strncmp (line, "PID=", 4) == 0))
            {
                Gpid = atoi (line + 4);
                //printf("Glob PID=%d\n",Gpid);
                //	      fflush(NULL);
                nothing_found = TRUE;
                return;
            }
		// --------- until here

            if (line[0] == '/')  /* strstr for : and strtok and send to cuenta */
            {
                if (fileLimit>0 && findCount >= fileLimit) {
                    char result[128];
                    snprintf(result,127,_("Finished search. More than %d results found (use Preferences to change this limit)."),fileLimit);
                    statusLine->copy_label(result);
                    statusLine->redraw();
                    searchButton->activate();
                    stopButton->deactivate();
                    cancelled = TRUE;
                    abort_glob1();
                } else
                {
                    char *path, *linecount = NULL, *textos[6], cuenta[32],
                        sizeF[64], permF[16];
                    struct stat st;
                    int *data;

                    path = line;
                    char *ptr = path;
                    while(*ptr) { if(*ptr=='\n') *ptr='\0'; ptr++; }
//                    statusLine->copy_label(fl_trim(path));
                    char result[128];
                    snprintf(result,127,_("Searching... %d results found."),findCount+1);
                    statusLine->copy_label(result);
                    statusLine->redraw();

                    if (strstr(path, ":"))
                    {
                        path = strtok(path, ":");
                        linecount = strtok (NULL, ":");
                        if (strcmp(linecount, "0") == 0)
                        {
                            linecount = NULL;
                            return;
                        }
                    }

                    findCount++;
                    data=(int *)malloc(3*sizeof(int));
                    data[0]=findCount;
                    data[1]=data[2]=0;

                    if (linecount)
                        snprintf(cuenta, sizeof(cuenta)-1, "%d (%s %s)", findCount, linecount, "lines");
                    else
                        snprintf (cuenta, sizeof(cuenta)-1, "%d", findCount);

                    textos[0] = cuenta;
                    textos[1] = filename = (char*)fl_file_filename(path);
                    textos[2] = path;

                    Fl_Image *resultImage=0;

                    if (lstat (path, &st) == 0)
                    {
                        data[1]=st.st_size;
                        data[2]=st.st_ctime;

                        snprintf (sizeF, sizeof(sizeF)-1,"%ld", st.st_size);
                        snprintf (permF, sizeof(permF)-1,"0%o", st.st_mode & 07777);
                        textos[3] = sizeF;
                        textos[4] = ctime (&(st.st_ctime));
                        textos[4] = strtok(textos[4],"\n");	// chop()
                        textos[5] = permF;

                        if (S_ISREG (st.st_mode))
                        {
                            resultImage = &page_pix;
                        }
                        if ((st.st_mode & 0100) || (st.st_mode & 010)
                            || (st.st_mode & 01))
                        {
                            resultImage = &exe_pix;
                        }
                        if (st.st_mode & 04000)
                        {
                            resultImage = &sexe_pix;
                        }
                        if (S_ISDIR (st.st_mode))
                        {
                            resultImage = &dir_close_pix;
                        }
                        if (S_ISCHR (st.st_mode))
                        {
                            resultImage = &char_dev_pix;
                        }
                        if (S_ISBLK (st.st_mode))
                        {
                            resultImage = &block_dev_pix;
                        }
                        if (S_ISFIFO (st.st_mode))
                        {
                            resultImage = &fifo_pix;
                        }
                        if (S_ISLNK (st.st_mode))
                        {
                            resultImage = &page_lnk_pix;
                        }
                        if (S_ISSOCK (st.st_mode))
                        {
                            resultImage = &socket_pix;
                        }
                    }
                    else
                    {
                        textos[2] = textos[3] = textos[4] = "-";
                    }
                    
                                 // leave just directory
                        *(strrchr(path,'/'))=0;
                        if (!strlen(path))
                            textos[2]="/";
//                        char output[FL_PATH_MAX];
//                        snprintf(output, sizeof(output)-1, "%s/%s", textos[2], textos[1]);
                        searchList->begin();
                        Fl_ListView_Item *resultItem = new Fl_ListView_Item();

                        // Copy labels, so item destructor knows to de-allocate them
                        resultItem->copy_label(0, textos[1]);
                        resultItem->copy_label(1, textos[2]);
                        resultItem->copy_label(2, textos[3]);
                        resultItem->copy_label(3, textos[4]);
                        resultItem->copy_label(4, textos[5]);

                        resultItem->image(resultImage);

                        searchList->end();
                        searchList->relayout();
                        searchList->redraw();
                    
                }
            }
            //else {}
            nothing_found = FALSE;
            buffer = line;
            return;  ;           /* continue here causes main loop blocking */
        }
        buffer++;
    }
    return;
}

int main (int argc, char **argv)
{
    fl_init_locale_support("efinder", PREFIX"/share/locale");

    if (pipe (pfd) < 0)
    {
        perror ("pipe");
        return 1;
    }

    createFindWindow();

    Fl::add_fd(pfd[0], FL_READ, process_find_messages, (void*)pfd[0]);

    Fl::run();

    close(pfd[0]);
    close(pfd[1]);

    return 0;
}
