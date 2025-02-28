/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#include "GOrgueLabel.h"

#include "GOrgueDocument.h"
#include "GOrgueSettings.h"
#include "GrandOrgueFile.h"
#include <wx/intl.h>

GOrgueLabel::GOrgueLabel(GrandOrgueFile* organfile) :
	m_Name(),
	m_Content(),
	m_organfile(organfile),
	m_sender(organfile, MIDI_SEND_LABEL)
{
	m_organfile->RegisterMidiConfigurator(this);
	m_organfile->RegisterPlaybackStateHandler(this);
}

GOrgueLabel::~GOrgueLabel()
{
}

void GOrgueLabel::Init(GOrgueConfigReader& cfg, wxString group, wxString name)
{
	m_organfile->RegisterSaveableObject(this);
	m_group = group;
	m_Name = name;
	m_sender.Load(cfg, m_group, m_organfile->GetSettings().GetMidiMap());
}

void GOrgueLabel::Load(GOrgueConfigReader& cfg, wxString group, wxString name)
{
	m_organfile->RegisterSaveableObject(this);
	m_group = group;
	m_Name = name;
	m_sender.Load(cfg, m_group, m_organfile->GetSettings().GetMidiMap());
}

void GOrgueLabel::Save(GOrgueConfigWriter& cfg)
{
	m_sender.Save(cfg, m_group, m_organfile->GetSettings().GetMidiMap());
}

const wxString& GOrgueLabel::GetName()
{
	return m_Name;
}

const wxString& GOrgueLabel::GetContent()
{
	return m_Content;
}

void GOrgueLabel::SetContent(wxString name)
{
	m_Content = name;
	m_sender.SetLabel(m_Content);
	m_organfile->ControlChanged(this);
}

void GOrgueLabel::AbortPlayback()
{
	m_sender.SetLabel(wxEmptyString);
	m_sender.SetName(wxEmptyString);
}

void GOrgueLabel::PreparePlayback()
{
	m_sender.SetName(m_Name);
}

void GOrgueLabel::StartPlayback()
{
}

void GOrgueLabel::PrepareRecording()
{
	m_sender.SetLabel(m_Content);
}

wxString GOrgueLabel::GetMidiType()
{
	return _("Label");
}

wxString GOrgueLabel::GetMidiName()
{
	return GetName();
}

void GOrgueLabel::ShowConfigDialog()
{
	wxString title = wxString::Format(_("Midi-Settings for %s - %s"), GetMidiType().c_str(), GetMidiName().c_str());

	m_organfile->GetDocument()->ShowMIDIEventDialog(this, title, NULL, &m_sender, NULL);
}

wxString GOrgueLabel::GetElementStatus()
{
	return m_Content;
}

std::vector<wxString> GOrgueLabel::GetElementActions()
{
	std::vector<wxString> actions;
	return actions;
}

void GOrgueLabel::TriggerElementActions(unsigned no)
{
	// Never called
}
