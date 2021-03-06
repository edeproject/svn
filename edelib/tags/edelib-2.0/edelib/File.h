/*
 * $Id$
 *
 * File IO stream
 * Copyright (c) 2005-2007 edelib authors
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __EDELIB_FILE_H__
#define __EDELIB_FILE_H__

#include <stdio.h>
#include "String.h"

EDELIB_NS_BEGIN

/**
 * \enum FileErrors
 * \brief Error codes returned by File class
 */
enum FileErrors {
	FILE_SUCCESS = 0,  ///< successful operation
	FILE_EACCESS,      ///< permission denied
	FILE_ENOENT,       ///< no such file
	FILE_EMFILE,       ///< too many opened files
	FILE_ENSPC,        ///< no space left on device
	FILE_FLAG          ///< bad flag
};

/**
 * \enum FileIOMode
 * \brief Open and write flags for File class
 */
enum FileIOMode {
	FIO_READ    = (1<<1),          ///< open file in read-only mode
	FIO_WRITE   = (1<<2),          ///< open file in write mode, and truncate it to zero length
	FIO_APPEND  = (1<<3),          ///< open file in append mode
	FIO_BINARY  = (1<<4),          ///< open file in binary mode
	FIO_TRUNC   = (1<<5)           ///< truncate currently opened file
};

/**
 * \class File
 * \brief A system file io stream.
 *
 * File is portable wrapper for various functions for reading and writing files.
 * Also brings automatic descriptor closing or possible another file openning during
 * object lifetime.
 *
 * Here is a sample of common usage:
 * \code
 *  File f("foo.txt");
 *  if(!f)
 *  	printf("can't open %s\n", f.name());
 *  while(!f.eof())
 *  	printf("%s", f.readline());
 *  f.close(); // optional
 * \endcode
 *
 * \todo error checks during stream read/write (ferror)
 */
class EDELIB_API File {
private:
	FILE* fobj;
	char* fname;
	int fmode;
	int errcode;
	bool opened;
	bool alloc;

	File(const File&);
	File& operator=(File&);

public:
	/**
	 * Creates empty file object.
	 */
	File();

	/**
	 * Open stream for reading or writing
	 *
	 * \param n file name to open
	 * \param m how to open file; default is read-only
	 */
	File(const char* n, int m);

	/**
	 * Cleans reserved data, and closes
	 * all possible opened descriptors.
	 */
	~File();

	/**
	 * Open file in given mode
	 *
	 * \param fname file name to open
	 * \param mode how to open file; default is read-only
	 */
	bool open(const char* fname, int mode = FIO_READ);

	/**
	 * Close current open descriptor, and clean reserved data.
	 * If is not opened, function will do nothing, except to
	 * clean reserved data if needed.
	 */
	void close(void);

	/**
	 * Returns name of curent opened stream. If is called
	 * close() before, it will return NULL.
	 *
	 * \return name of curent opened stream
	 */
	const char* name(void) const;

	/**
	 * Checks if end of file reached.
	 *
	 * \return true if end of file is reached.
	 */
	bool eof(void);

	/**
	 * Read an character from stream.
	 *
	 * \return character or EOF.
	 */
	int getch(void);

	/**
	 * Read data from the stream, with specified size.
	 *
	 * \return number of read data
	 * \param buff where to place data
	 * \param typesz size of each item
	 * \param buffsz size of buffer
	 */
	int read(void* buff, int typesz, int buffsz);

	/**
	 * Read line from stream in specified buffer, with given
	 * size. If buffer size is less than read line, only
	 * given size will be filled. A '\\0' is stored as last
	 * character in buffer. It will return EOF if end of stream
	 * is reached.
	 *
	 * \todo it does not return 1 when line contains only '\\n' char. Should return?
	 *
	 * \return size of readed data or EOF for end.
	 * \param buff where to place content
	 * \param buffsz size of buffer
	 */
	int readline(char* buff, int buffsz);

	/**
	 * Write character to the stream.
	 *
	 * \return a character written, or EOF in case of error.
	 */
	int putch(int c);

	/**
	 * Write data to the stream, with specified size.
	 *
	 * \return number of written data
	 * \param buff data to write
	 * \param typesz size of each item
	 * \param buffsz size of buffer
	 */
	int write(const void* buff, int typesz, int buffsz);

	/**
	 * Write char data to the stream.
	 *
	 * \return number of written data
	 * \param buff data to write
	 * \param buffsz size of buffer
	 */
	int write(const char* buff, unsigned int buffsz);

	/**
	 * Same as write(buff, strlen(buff))
	 */
	int write(const char* buff);

	/**
	 * printf function on the stream.
	 *
	 * \return size of writen data
	 */
	int printf(const char* fmt, ...);
};

/** 
 * Check if file exists and is regular file 
 * \related File
 * \deprecated with file_test()
 */
EDELIB_API bool file_exists(const char* name) EDELIB_DEPRECATED;

/** 
 * Check if file is readable 
 * \related File
 * \deprecated with file_test()
 */
EDELIB_API bool file_readable(const char* name) EDELIB_DEPRECATED;

/** 
 * Check if file is writeable 
 * \related File
 * \deprecated with file_test()
 */
EDELIB_API bool file_writeable(const char* name) EDELIB_DEPRECATED;


/**
 * Check if file is executable
 * \related File
 * \deprecated with file_test()
 */
EDELIB_API bool file_executable(const char* name) EDELIB_DEPRECATED;

/**
 * Remove file at given path. It will call system's unlink()
 * instead remove() since remove() is not compatible between
 * latest versions of libc and libc4 or libc5 (see <em>man remove</em>).
 *
 * \related File
 * \return true if operation was succesfull, or false if is unable to proceed
 * \param name is absolute (or relative) path to the file
 */
EDELIB_API bool file_remove(const char* name);

/**
 * Copy file from src to dest. This function is implemented via getc()
 * (not like standard <em>cp</em> command) due it's simplicity. 
 *
 * \note src <b>must</b> exists and checking if dest exists <b>is not done</b>.
 *       This means if dest exists, it will <b>be overwritten</b>. Use file_exists() to
 *       check does it really exists.
 *
 * If <em>exact</em> parameter is set to true, it will set the same permissions and access/change time
 * to the dest as src. On other hand, if any of stages (changing permissions or setting time) fails
 * file_copy() will return false.
 *
 * \related File
 * \return true if is able to open src and able to write in dest, or false if failed in both (or setting
 *  permissions/time if <em>exact</em> paramter is used)
 * \param src is source file to be copied
 * \param dest is destination
 * \param exact will try to set exact src info (timestamp,owner,group,...) to dest
 */
EDELIB_API bool file_copy(const char* src, const char* dest, bool exact = false);

/**
 * Rename file. Assumed is that name to be renamed to does not exists (file, directory, link and etc.)
 * \related File
 * \return true if succeded or false if not
 * \param from is what file to rename
 * \param to is new name
 */
EDELIB_API bool file_rename(const char* from, const char* to);

/**
 * Return full path of given argument, looking at $PATH environment variable.
 * If is unable to read $PATH or file does not exists or resolved path is not file,
 * it will return empty string.
 *
 * Returned path can be either link to binary or actual binary (depends on how
 * entries in $PATH are placed). For example <em>mv</em> command is often in <em>/bin</em>, and
 * it's link is (often) in <em>/usr/bin</em>. If PATH contains something like 
 * <em>/usr/local/bin:/usr/bin:/bin</em>, <em>/usr/bin/mv</em> will be returned.
 *
 * For this casses, setting <em>skip_link</em> to true will return <em>/bin/mv</em>.
 *
 * \related File
 * \return full path if file exists or empty string as noted above
 * \param fname file to look for
 * \param skip_link if true symbolic links are ignored
 */
EDELIB_API String file_path(const char* fname, bool skip_link = false);

EDELIB_NS_END
#endif
