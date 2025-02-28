/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#include "GOrgueCompress.h"

#include "GOrgueBuffer.h"
#include "GOrgueGZipFormat.h"
#include <wx/mstream.h>
#include <wx/zstream.h>

bool compressBuffer(GOrgueBuffer<uint8_t>& buffer)
{
	wxMemoryOutputStream stream;
	wxZlibOutputStream zstream(stream, -1, wxZLIB_GZIP);
	zstream.Write(buffer.get(), buffer.GetSize());
	if (!zstream.IsOk() || !zstream.Close())
		return false;
	if (!stream.IsOk() || !stream.Close())
		return false;
	GOrgueBuffer<uint8_t> buf(stream.GetLength());
	if (stream.CopyTo(buf.get(), buf.GetSize()) != buf.GetSize())
		return false;
	buffer = std::move(buf);
	return true;
}


bool isBufferCompressed(const GOrgueBuffer<uint8_t>& buffer)
{
	if (buffer.GetSize() < sizeof(GOGZipHeader) + sizeof(GOGZipTrailer))
		return false;
	const GOGZipHeader* header = reinterpret_cast<const GOGZipHeader*>(buffer.get());
	if (header->signature == GZIP_SIGNATURE)
		return true;
	return false;
}

bool uncompressBuffer(GOrgueBuffer<uint8_t>& buffer)
{
	wxMemoryOutputStream mstream;
	{
		wxMemoryInputStream stream(buffer.get(), buffer.GetSize());
		wxZlibInputStream zstream(stream, wxZLIB_GZIP);
		zstream.Read(mstream);
		if (!zstream.Eof())
			return false;
	}
	if (!mstream.IsOk() || !mstream.Close())
		return false;
	GOrgueBuffer<uint8_t> buf(mstream.GetLength());
	if (mstream.CopyTo(buf.get(), buf.GetSize()) != buf.GetSize())
		return false;
	buffer = std::move(buf);
	return true;
}
