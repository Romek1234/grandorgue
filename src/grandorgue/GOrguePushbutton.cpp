/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#include "GOrguePushbutton.h"

GOrguePushbutton::GOrguePushbutton(GrandOrgueFile* organfile) :
	GOrgueButton(organfile, MIDI_RECV_BUTTON, true)
{
}
