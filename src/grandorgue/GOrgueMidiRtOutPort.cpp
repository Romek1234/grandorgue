/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#include "GOrgueMidiRtOutPort.h"
#include <wx/intl.h>
#include <wx/log.h>

GOrgueMidiRtOutPort::GOrgueMidiRtOutPort(GOrgueMidi* midi, wxString prefix, wxString name, RtMidi::Api api) :
	GOrgueMidiOutPort(midi, prefix, name),
	m_api(api),
	m_port(NULL)
{
}

GOrgueMidiRtOutPort::~GOrgueMidiRtOutPort()
{
  Close(true);
}

bool GOrgueMidiRtOutPort::Open()
{
  Close(false);
  if (!m_port)
    try
    {
      m_port = new RtMidiOut(m_api, (const char*)GetClientName().fn_str());
    }
    catch (RtMidiError &e)
    {
      wxString error = wxString::FromAscii(e.getMessage().c_str());
      wxLogError(_("RtMidi error: %s"), error.c_str());
    }
  if (!m_port)
    return false;
  try
  {
    for (unsigned i = 0; i <  m_port->getPortCount(); i++)
    {
	    if (m_Name == m_Prefix + wxString::FromAscii(m_port->getPortName(i).c_str()))
	    {
		    m_port->openPort(i, (const char*)GetPortName().fn_str());
		    m_IsActive = true;
		    break;
	    }
    }
  }
  catch (RtMidiError &e)
  {
    wxString error = wxString::FromAscii(e.getMessage().c_str());
    wxLogError(_("RtMidi error: %s"), error.c_str());
  }
  return GOrgueMidiOutPort::Open();
}

void GOrgueMidiRtOutPort::Close(bool isToFreePort)
{
  if (m_IsActive)
  {
    if (m_port)
      try
      {
	m_port->closePort();
      }
      catch (RtMidiError &e)
      {
	wxString error = wxString::FromAscii(e.getMessage().c_str());
	wxLogError(_("RtMidi error: %s"), error.c_str());
      }
    m_IsActive = false;
  }

  if (isToFreePort)
  {
    RtMidiOut* const port = m_port;

    if (port) {
      m_port = NULL;
      try
      {
	delete port;
      }
      catch (RtMidiError &e)
      {
	wxString error = wxString::FromAscii(e.getMessage().c_str());
	wxLogError(_("RtMidi error: %s"), error.c_str());
      }
    }
  }
}

void GOrgueMidiRtOutPort::SendData(std::vector<unsigned char>& msg)
{
  if (m_port)
    try
    {
	    m_port->sendMessage(&msg);
    }
    catch (RtMidiError &e)
    {
	    wxString error = wxString::FromAscii(e.getMessage().c_str());
	    wxLogError(_("RtMidi error: %s"), error.c_str());
    }
}
