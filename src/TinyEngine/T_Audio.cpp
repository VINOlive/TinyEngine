//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#include "T_Audio.h"

AudioWave::AudioWave():hmmio(0), wfex(0)
{	}

AudioWave::~AudioWave()
{
	Close();
}

bool AudioWave::Open(const wchar_t *path)
{
	hmmio =  mmioOpenW((wchar_t *)path, NULL, MMIO_ALLOCBUF | MMIO_READ);
	if(hmmio == NULL)	return false;
	if(mmioDescend(hmmio, &ckInRIFF, NULL, 0) != 0) return false;
	if((ckInRIFF.ckid != FOURCC_RIFF)|| (ckInRIFF.fccType != mmioFOURCC('W', 'A', 'V', 'E'))) return false;
	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if(mmioDescend(hmmio, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0) return false;
	if(ckIn.cksize < sizeof(PCMWAVEFORMAT)) return false;
	wfex = (WAVEFORMATEX *)new char[ckIn.cksize];
	if(wfex == 0)  return false;
	if(mmioRead(hmmio, (HPSTR)wfex, ckIn.cksize) != (LONG)ckIn.cksize) return false;
	if(mmioAscend(hmmio, &ckIn, 0) != 0) return false;
	return true;
}

bool AudioWave::StartRead()
{
	if(mmioSeek(hmmio, ckInRIFF.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1) return false;
	ckIn.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if(mmioDescend(hmmio, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0) return false;			
	return true;
}

bool AudioWave::Read(unsigned long size, void *data, unsigned long *nread)
{
	*nread = 0;
	int result = mmioRead(hmmio, (HPSTR)data, size);
	if (result == -1) return false;
	*nread = (unsigned)result;
	return true;
}

bool AudioWave::Close()
{
	delete[] (char *)wfex;
	wfex = 0;
	if(hmmio != NULL) return mmioClose(hmmio, 0) == 0;
	return true;
}

AudioDXBuffer::AudioDXBuffer():SndBuf(NULL),Volume(0),Pan(0),Freq(0)
{

}

AudioDXBuffer::~AudioDXBuffer()
{
	if(SndBuf) SndBuf->Release();
}

bool AudioDXBuffer::CreateBuffer(AudioDX &ds, WAVEFORMATEX *format, int size)
{
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags =	DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS| 
					DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLVOLUME| 
					DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = size;
	dsbd.lpwfxFormat = format;
	HRESULT result = ds->CreateSoundBuffer(&dsbd, &SndBuf, NULL);
	if (FAILED(result)) return false;
	SndBuf->GetVolume(&Volume);
	SndBuf->GetPan(&Pan);
	SndBuf->GetFrequency(&Freq);
	return true;
}

bool AudioDXBuffer::CreateMainBuffer(AudioDX &ds, int freq)
{
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd); 
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;
	HRESULT result = ds->CreateSoundBuffer(&dsbd, &SndBuf, NULL);
	if(FAILED(result)) return false;
	WAVEFORMATEX wfex;
	ZeroMemory(&wfex, sizeof(wfex));
	wfex.wFormatTag = WAVE_FORMAT_PCM;
	wfex.nChannels = 2;
	wfex.nSamplesPerSec = freq;
	wfex.nBlockAlign = 4;
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;
	wfex.wBitsPerSample = 16;
	result = SndBuf->SetFormat(&wfex);
	if(FAILED(result)) return false;
	SndBuf->GetVolume(&Volume);
	SndBuf->GetPan(&Pan);
	SndBuf->GetFrequency(&Freq);
	return true;
}

bool AudioDXBuffer::LoadWave(AudioDX &ds, const wchar_t *path)
{
	AudioWave file;
	if(!file.Open(path) || !file.StartRead()) return false;
	Release();
	if(!CreateBuffer(ds, file.GetFormat(), file.CkSize())) return false;
	void *ptr1, *ptr2;
	DWORD bytes1, bytes2;
	HRESULT result = SndBuf->Lock( 0, file.CkSize(), &ptr1, &bytes1, &ptr2, &bytes2, 0);
	if(result == DSERR_BUFFERLOST) 
	{
		Restore();
		result = SndBuf->Lock( 0, file.CkSize(), &ptr1, &bytes1, &ptr2, &bytes2, 0);
	}
	if(SUCCEEDED(result)) 
	{
		bool readerror = false;
		DWORD nread;
		if(!file.Read(bytes1, (LPBYTE)ptr1, &nread) || bytes1 != nread)
			readerror = true;
		if(bytes2) 
		{
			if(!file.Read(bytes2, (LPBYTE)ptr2, &nread) || bytes2 != nread)
				readerror = true;
		}
		result = SndBuf->Unlock(ptr1, bytes1, ptr2, bytes2);
		if(readerror) return false;
		return true;
	}
	return false;
}

HRESULT AudioDXBuffer::Release()
{
	HRESULT result = DS_OK;
	if(SndBuf) result = SndBuf->Release();
	SndBuf = 0;
	return result;
}

HRESULT AudioDXBuffer::Restore()
{
	HRESULT result = DS_OK;
	if(SndBuf) return SndBuf->Restore();
	return DS_OK;
}

void AudioDXBuffer::Terminate()
{
 	if(SndBuf)
	{
		if(FAILED(SndBuf->Restore())) return;		
		if(FAILED(SndBuf->Stop())) return;
	}
}

AudioDX::AudioDX()
{
	ds = 0;
}

AudioDX::~AudioDX()
{
	ReleaseAll();
}

bool AudioDX::CreateDS(HWND hWnd, int freq)
{
	HRESULT result = DirectSoundCreate(NULL, &ds, NULL);
	if (FAILED(result)) return false;
	result = ds->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if (FAILED(result))	return false;
	return dsbuf.CreateMainBuffer(*this, freq);
}

void AudioDX::ReleaseAll()
{
	if (ds) {
		dsbuf.Release();
		ds->Release();
		ds = 0;
	}
}

HRESULT AudioDX::RestoreAll()
{
	HRESULT result = dsbuf.Restore();
	if (result != DS_OK) return result;

	return DS_OK;
}