/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#ifndef GORUGEARCHIVEENTRYFILE_H
#define GORUGEARCHIVEENTRYFILE_H

#include "GOrgueFile.h"

class GOrgueArchive;

class GOrgueArchiveEntryFile : public GOrgueFile
{
private:
	GOrgueArchive* m_archiv;
	wxString m_Name;
	size_t m_Offset;
	size_t m_Length;
	size_t m_Pos;

public:
	GOrgueArchiveEntryFile(GOrgueArchive* archive, const wxString& name, size_t offset, size_t len);

	size_t GetSize();
	const wxString GetName();
	const wxString GetPath();

	bool Open();
	void Close();
	size_t Read(void * buffer, size_t len);
};

#endif
