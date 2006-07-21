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

#include "Sound.h"
#include "debug.h"

#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>

#include <assert.h>
#include <stdio.h>

SoundSystem::SoundSystem() : device(NULL)
{
}

SoundSystem::~SoundSystem()
{
}

void SoundSystem::init(void)
{
	ao_initialize();
	default_driver = ao_default_driver_id();
}

void SoundSystem::shutdown(void)
{
	ao_shutdown();
}

int SoundSystem::play(SoundEvent e)
{
	// not implemented
	return 1;
}

int SoundSystem::play(const char* fname)
{
	assert(fname != NULL);

	FILE* f = fopen(fname, "rb");
	if(f == NULL)
	{
		ELOG("Can't open %s\n", fname);
		return 0;
	}

	OggVorbis_File vf;
	vorbis_info* vi;

	if(ov_open(f, &vf, NULL, 0) < 0)
	{
		ELOG("%s does not appear to be ogg file\n");
		fclose(f);
		return 0;
	}

	// read and print comments
	char** comm = ov_comment(&vf, -1)->user_comments;
	vi = ov_info(&vf, -1);

	while(*comm)
	{
		ELOG("%s", *comm);
		comm++;
	}

	assert(vi != NULL);

	format.bits = 4 * 8;	// TODO: should be word_size * 8
	format.channels = vi->channels;
	format.rate = vi->rate;
	format.byte_format = AO_FMT_NATIVE;

	// now open device
	device = ao_open_live(default_driver, &format, NULL);
	if(device == NULL)
	{
		ELOG("Can't open device");
		ov_clear(&vf);
		return 0;
	}

	int current_section;
	while(1)
	{
		long ret = ov_read(&vf, pcm_out, sizeof(pcm_out), 0, 2, 1, &current_section);
		if(ret == 0)
			break;
		else if(ret < 0)
			ELOG("Error in the stream, continuing...");
		else
			ao_play(device, pcm_out, ret);
	}

	ao_close(device);
	device = NULL;

	// NOTE: fclose(f) is not needed, since ov_clear() will close file
	ov_clear(&vf);

	return 1;
}
