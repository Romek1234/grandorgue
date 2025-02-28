/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#ifndef GOSOUNDREVERB_H
#define GOSOUNDREVERB_H

#include "ptrvector.h"
class Convproc;
class GOrgueSettings;

class GOSoundReverb
{
private:
	unsigned m_channels;
	ptr_vector<Convproc> m_engine;

	void Cleanup();

public:
	GOSoundReverb(unsigned channels);
	virtual ~GOSoundReverb();

	void Reset();
	void Setup(GOrgueSettings& settings);

	void Process(float *output_buffer, unsigned n_frames);
};

#endif
