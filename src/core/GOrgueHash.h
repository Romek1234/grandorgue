/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#ifndef GORGUEHASH_H
#define GORGUEHASH_H

#include "contrib/sha1.h"
#include <wx/string.h>

typedef struct _GOrgueHashType
{
	uint8_t hash[20];
} GOrgueHashType;

class GOrgueHash
{
private:
	SHA_CTX m_ctx;
	GOrgueHashType m_Hash;
	bool m_Done;

public:
	GOrgueHash();
	~GOrgueHash();

	const GOrgueHashType& getHash();
	wxString getStringHash();

	void Update(const void* data, unsigned len);
	void Update(const wxString& str);
	void Update(signed int value);
	void Update(unsigned int value);
	void Update(signed long value);
	void Update(unsigned long value);
	void Update(signed long long value);
	void Update(unsigned long long value);
};

#endif
