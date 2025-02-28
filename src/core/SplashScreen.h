/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <wx/bitmap.h>
#include <wx/control.h>
#include <wx/dialog.h>
#include <wx/timer.h>

class GOrgueSplashBitmap : public wxControl
{
private:
	wxBitmap m_Bitmap;

	void OnPaint(wxPaintEvent& event);
	void OnClick(wxMouseEvent& event);
	void OnKey(wxKeyEvent& event);

public:
	GOrgueSplashBitmap(wxWindow *parent, wxWindowID id, wxBitmap& bitmap);

	DECLARE_EVENT_TABLE()
};

class GOrgueSplash : private wxDialog
{
private:
	GOrgueSplashBitmap* m_Image;
	wxTimer  m_Timer;
	bool m_hasTimeout;

	GOrgueSplash
		(bool               has_timeout
		,wxWindow          *parent
		,wxWindowID         id
		);
	~GOrgueSplash();
	
	void OnShowWindow(wxShowEvent &event);
	void OnCloseWindow(wxCloseEvent& event);
	void OnNotify(wxTimerEvent& event);

	void DrawText(wxBitmap& bitmap);

public:
	static void DoSplash(bool hasTimeout, wxWindow *parent);
	
	DECLARE_EVENT_TABLE()
};

#endif
