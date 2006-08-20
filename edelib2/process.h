/*
 * $Id$
 *
 * edelib::PtyProcess - This class enables us to "chat" with terminal programs synchronously
 * Adapted from KDE (kdelibs/kdesu/process.h) - original copyright message below
 *
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */


/* vi: ts=8 sts=4 sw=4
 *
 * Id: process.h 439322 2005-07-27 18:49:23Z coolo 
 *
 * This file is part of the KDE project, module kdesu.
 * Copyright (C) 1999,2000 Geert Jansen <jansen@kde.org>
 * 
 * This is free software; you can use this library under the GNU Library 
 * General Public License, version 2. See the file "COPYING.LIB" for the 
 * exact licensing terms.
 */

#ifndef _edelib_Process_h_
#define _edelib_Process_h_

#include <sys/types.h>

//#include <qbytearray.h>
//#include <qstring.h>
//#include <qstringlist.h>
//#include <qlist.h>

//#include <kdelibs_export.h>

namespace edelib {

class PTY;

/**
 * Synchronous communication with tty programs.
 *
 * PtyProcess provides synchronous communication with tty based programs.
 * The communications channel used is a pseudo tty (as opposed to a pipe)
 * This means that programs which require a terminal will work.
 */

class PtyProcess
{
public:
    PtyProcess();
    virtual ~PtyProcess();

    /**
     * Forks off and execute a command. The command's standard in and output
     * are connected to the pseudo tty. They are accessible with readLine
     * and writeLine.
     * @param command The command to execute.
     * @param args The arguments to the command.
     */
    int exec(const char *command, const char **args);

    /**
     * Reads a line from the program's standard out. Depending on the @em block
     * parameter, this call blocks until a single, full line is read.
     * @param block Block until a full line is read?
     * @return The output string.
     */
    char *readLine(bool block=true);

    /**
     * Writes a line of text to the program's standard in.
     * @param line The text to write.
     * @param addNewline Adds a '\n' to the line.
     */
    void writeLine(const char *line, bool addNewline=true);

    /**
     * Puts back a line of input.
     * @param line The line to put back.
     * @param addNewline Adds a '\n' to the line.
     */
    void unreadLine(const char *line, bool addNewline=true);

    /**
     * Sets the exit string. If a line of program output matches this,
     * waitForChild() will terminate the program and return.
     */
    void setExitString(char *exit) { m_Exit = exit; }

    /**
     * Waits for the child to exit, capturing all output.
     */
    int runChild();

    /**
     * Waits for the child to exit. See also setExitString.
     */
    int waitForChild();

    /**
     * Waits until the pty has cleared the ECHO flag. This is useful
     * when programs write a password prompt before they disable ECHO.
     * Disabling it might flush any input that was written.
     */
    int WaitSlave();

    /**
     * Enables/disables local echo on the pseudo tty.
     */
    int enableLocalEcho(bool enable=true);

    /**
     * Enables/disables terminal output. Relevant only to some subclasses.
     */
    void setTerminal(bool terminal) { m_bTerminal = terminal; }

    /**
     * Overwrites the password as soon as it is used. Relevant only to
     * some subclasses.
     */
    void setErase(bool erase) { m_bErase = erase; }

    /**
     * Set additinal environment variables.
     */
    void setEnvironment( const char **env );

    /**
     * Returns the filedescriptor of the process.
     */
    inline int fd() const {return m_Fd;};

    /**
     * Returns the pid of the process.
     */
    inline int pid() const {return m_Pid;};

public /* static */:
    /*
    ** This is a collection of static functions that can be
    ** used for process control inside kdesu. I'd suggest
    ** against using this publicly. There are probably
    ** nicer Qt based ways to do what you want.
    */

    /*
    ** Wait @p ms miliseconds (ie. 1/10th of a second is 100ms),
    ** using @p fd as a filedescriptor to wait on. Returns
    ** select(2)'s result, which is -1 on error, 0 on timeout,
    ** or positive if there is data on one of the selected fd's.
    **
    ** @p ms must be in the range 0..999 (ie. the maximum wait
    ** duration is 999ms, almost one second).
    */
    static int waitMS(int fd,int ms);


    /*
    ** Basic check for the existence of @p pid.
    ** Returns true iff @p pid is an extant process,
    ** (one you could kill - see man kill(2) for signal 0).
    */
    static bool checkPid(pid_t pid);

    /*
    ** Check process exit status for process @p pid.
    ** On error (no child, no exit), return -1.
    ** If child @p pid has exited, return its exit status,
    ** (which may be zero).
    ** If child @p has not exited, return -2.
    */
    enum checkPidStatus { Error=-1, NotExited=-2, Killed=-3, Crashed=-4 } ;
    static int checkPidExited(pid_t pid);


protected:
    char **environment() const;

    bool m_bErase, m_bTerminal;
    int m_Pid, m_Fd;
    char *m_Command, *m_Exit;

private:
    int init();
    int SetupTTY(int fd);

    PTY *m_pPTY;
    char *m_Inbuf, *m_TTY;

protected:
    virtual void virtual_hook( int id, void* data );
private:
    class PtyProcessPrivate;
    PtyProcessPrivate *d;
};


}

#endif
