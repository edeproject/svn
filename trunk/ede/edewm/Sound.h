/*
 * $Id$
 *
 * Edewm, window manager
 * Part of Equinox Desktop Environment (EDE).
 * Copyright (c) 2000-2006 EDE Authors.
 *
 * This program is licenced under terms of the
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef __SOUND_H__
#define __SOUND_H__

#include <ao/ao.h>

/* Sound player based on libao and libvorbis.
 * Possible changes in stream decoder.
 */

enum SoundEvent
{
	SOUND_MINIMIZE = 0,
	SOUND_MAXIMIZE,
	SOUND_CLOSE,
	SOUND_SHADE
};

/*
struct StreamFormat
{
	int bits;
	int channels;
	int rate;
	int byte_format;        // accepts as ao_sample_format::byte_format 
};
*/
#define PCM_BUF_SIZE 4096

class SoundSystem
{
	private:
		ao_device* device;
		ao_sample_format format;
		int default_driver;
		char pcm_out[PCM_BUF_SIZE];

	public:
		SoundSystem();
		~SoundSystem();
		void init(void);
		void shutdown(void);
		int play(const char* fname);
		int play(SoundEvent e);
};

#endif
