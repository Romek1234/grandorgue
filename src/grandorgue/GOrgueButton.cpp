/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#include "GOrgueButton.h"

#include "GOrgueConfigReader.h"
#include "GOrgueDocument.h"
#include "GOrgueSettings.h"
#include "GrandOrgueFile.h"
#include <wx/intl.h>

GOrgueButton::GOrgueButton(GrandOrgueFile* organfile, MIDI_RECEIVER_TYPE midi_type, bool pushbutton) :
	m_organfile(organfile),
	m_midi(organfile, midi_type),
	m_sender(organfile, MIDI_SEND_BUTTON),
	m_shortcut(organfile, KEY_RECV_BUTTON),
	m_Pushbutton(pushbutton),
	m_Displayed(false),
	m_Name(),
	m_Engaged(false),
	m_DisplayInInvertedState(false),
	m_ReadOnly(false)
{
	m_organfile->RegisterEventHandler(this);
	m_organfile->RegisterMidiConfigurator(this);
	m_organfile->RegisterPlaybackStateHandler(this);
}

GOrgueButton::~GOrgueButton()
{
}

void GOrgueButton::Init(GOrgueConfigReader& cfg, wxString group, wxString name)
{
	m_organfile->RegisterSaveableObject(this);
	m_group = group;
	m_Name = name;
	m_Displayed = false;
	m_DisplayInInvertedState = false;
	if (!m_ReadOnly)
	{
		m_midi.Load(cfg, group, m_organfile->GetSettings().GetMidiMap());
		m_shortcut.Load(cfg, group);
	}
	m_sender.Load(cfg, group, m_organfile->GetSettings().GetMidiMap());
}

void GOrgueButton::Load(GOrgueConfigReader& cfg, wxString group)
{
	m_organfile->RegisterSaveableObject(this);
	m_group = group;
	m_Name = cfg.ReadStringNotEmpty(ODFSetting, group, wxT("Name"), true);
	m_Displayed = cfg.ReadBoolean(ODFSetting, group, wxT("Displayed"), false, false);
	m_DisplayInInvertedState = cfg.ReadBoolean(ODFSetting, group, wxT("DisplayInInvertedState"), false, false);
	if (!m_ReadOnly)
	{
		m_midi.Load(cfg, group, m_organfile->GetSettings().GetMidiMap());
		m_shortcut.Load(cfg, group);
	}
	m_sender.Load(cfg, group, m_organfile->GetSettings().GetMidiMap());
}

void GOrgueButton::Save(GOrgueConfigWriter& cfg)
{
	if (!m_ReadOnly)
	{
		m_midi.Save(cfg, m_group, m_organfile->GetSettings().GetMidiMap());
		m_shortcut.Save(cfg, m_group);
	}
	m_sender.Save(cfg, m_group, m_organfile->GetSettings().GetMidiMap());
}

bool GOrgueButton::IsDisplayed()
{
	return m_Displayed;
}

bool GOrgueButton::IsReadOnly()
{
	return m_ReadOnly;
}

const wxString& GOrgueButton::GetName()
{
	return m_Name;
}

void GOrgueButton::HandleKey(int key)
{
	if (m_ReadOnly)
		return;
	switch (m_shortcut.Match(key))
	{
	case KEY_MATCH:
		Push();
		break;

	default:
		break;
	}
}

void GOrgueButton::Push()
{
	if (m_ReadOnly)
		return;
	Set(m_Engaged ^ true);
}

void GOrgueButton::Set(bool on)
{
}

void GOrgueButton::AbortPlayback()
{
	m_sender.SetDisplay(false);
	m_sender.SetName(wxEmptyString);
}

void GOrgueButton::PreparePlayback()
{
	m_midi.PreparePlayback();
	m_sender.SetName(m_Name);
}

void GOrgueButton::StartPlayback()
{
}

void GOrgueButton::PrepareRecording()
{
	m_sender.SetDisplay(m_Engaged);
}

void GOrgueButton::ProcessMidi(const GOrgueMidiEvent& event)
{
	if (m_ReadOnly)
		return;
	switch(m_midi.Match(event))
	{
	case MIDI_MATCH_CHANGE:
		Push();
		break;

	case MIDI_MATCH_ON:
		if (m_Pushbutton)
			Push();
		else
			Set(true);
		break;

	case MIDI_MATCH_OFF:
		if (!m_Pushbutton)
			Set(false);
		break;

	default:
		break;
	}
}

void GOrgueButton::Display(bool onoff)
{
	if (m_Engaged == onoff)
		return;
	m_sender.SetDisplay(onoff);
	m_Engaged = onoff;
	m_organfile->ControlChanged(this);
}

bool GOrgueButton::IsEngaged() const
{
	return m_Engaged;
}

bool GOrgueButton::DisplayInverted() const
{
	return m_DisplayInInvertedState;
}

void GOrgueButton::SetElementID(int id)
{
	if (!m_ReadOnly)
	{
		m_midi.SetElementID(id);
		m_sender.SetElementID(id);
	}
}

void GOrgueButton::SetShortcutKey(unsigned key)
{
	m_shortcut.SetShortcut(key);
}

void GOrgueButton::SetPreconfigIndex(unsigned index)
{
	m_midi.SetIndex(index);
}

wxString GOrgueButton::GetMidiName()
{
	return GetName();
}

void GOrgueButton::ShowConfigDialog()
{
	wxString title = wxString::Format(_("Midi-Settings for %s - %s"), GetMidiType().c_str(), GetMidiName().c_str());

	GOrgueMidiReceiver* midi = &m_midi;
	GOrgueKeyReceiver* key = &m_shortcut;
	if (IsReadOnly())
	{
		midi = NULL;
		key = NULL;
	}

	m_organfile->GetDocument()->ShowMIDIEventDialog(this, title, midi, &m_sender, key);
}

wxString GOrgueButton::GetElementStatus()
{
	return m_Engaged ? _("ON") : _("OFF");
}

std::vector<wxString> GOrgueButton::GetElementActions()
{
	std::vector<wxString> actions;
	actions.push_back(_("trigger"));
	return actions;
}

void GOrgueButton::TriggerElementActions(unsigned no)
{
	if (no == 0)
		Push();
}
