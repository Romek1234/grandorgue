/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#ifndef GORGUEWAVE_H
#define GORGUEWAVE_H

#include "GOrgueBuffer.h"
#include <wx/string.h>
#include <vector>

class GOrgueFile;

typedef struct GO_WAVE_LOOP
{
	unsigned start_sample;
	unsigned end_sample;
} GO_WAVE_LOOP;

class GOrgueWave
{

private:
	GOrgueBuffer <uint8_t> m_SampleData;
	unsigned m_BytesPerSample;
	unsigned m_SampleRate;
	unsigned m_CuePoint;
	unsigned m_Channels;
	unsigned m_MidiNote;
	float m_PitchFract;
	bool m_isPacked;
	bool m_hasRelease;
	std::vector<GO_WAVE_LOOP> m_Loops;

	void SetInvalid();
	void LoadFormatChunk(const uint8_t* ptr, unsigned long length);
	void LoadCueChunk(const uint8_t* ptr, unsigned long length);
	void LoadSamplerChunk(const uint8_t* ptr, unsigned long length);
	template<class T>
	static void writeNext(uint8_t*& output, const T& value);
	template<class T>
	static T readNext(const uint8_t*& input);

public:

	typedef enum
	{
		SF_SIGNEDBYTE_8 = 8,
		SF_SIGNEDSHORT_9 = 9,
		SF_SIGNEDSHORT_10 = 10,
		SF_SIGNEDSHORT_11 = 11,
		SF_SIGNEDSHORT_12 = 12,
		SF_SIGNEDSHORT_13 = 13,
		SF_SIGNEDSHORT_14 = 14,
		SF_SIGNEDSHORT_15 = 15,
		SF_SIGNEDSHORT_16 = 16,
		SF_SIGNEDINT24_17 = 17,
		SF_SIGNEDINT24_18 = 18,
		SF_SIGNEDINT24_19 = 19,
		SF_SIGNEDINT24_20 = 20,
		SF_SIGNEDINT24_21 = 21,
		SF_SIGNEDINT24_22 = 22,
		SF_SIGNEDINT24_23 = 23,
		SF_SIGNEDINT24_24 = 24,
		SF_IEEE_FLOAT
	} SAMPLE_FORMAT;

	GOrgueWave();
	~GOrgueWave();

	void Open(GOrgueFile* file);
	void Open(const GOrgueBuffer <uint8_t>& content);
	bool Save(GOrgueBuffer<uint8_t>& buf);
	void Close();

	/* GetChannels()
	 * Returns the number of channels in the wave stream.
	 */
	unsigned GetChannels() const;

	/* GetReleaseMarkerPosition()
	 * Returns the block where the release marker has been placed at.
	 */
	bool HasReleaseMarker() const;
	unsigned GetReleaseMarkerPosition() const;

	unsigned GetNbLoops() const;
	const GO_WAVE_LOOP& GetLoop(unsigned idx) const;
	const GO_WAVE_LOOP& GetLongestLoop() const;

	/* GetLength()
	 * Returns the number of blocks in the wave file (there are *channel*
	 * samples in a block so for a mono wave, this will simply be the number
	 * of samples)
	 */
	unsigned GetLength() const;

	/* ReadSamples()
	 * Reads all of the samples in the wave file into destBuffer at the
	 * specified read format and sample rate.
	 */
	void ReadSamples(void* dest_buffer, GOrgueWave::SAMPLE_FORMAT read_format, unsigned sample_rate, int channels) const;


	unsigned GetSampleRate() const;
	unsigned GetBitsPerSample() const;
	unsigned GetMidiNote() const;
	float GetPitchFract() const;

	static bool IsWave(const GOrgueBuffer<uint8_t>& data);
	static bool IsWaveFile(const GOrgueBuffer<uint8_t>& data);
};

#endif /* GORGUEWAVE_H */
