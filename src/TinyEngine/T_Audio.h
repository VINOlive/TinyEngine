//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#pragma once
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <dsound.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")
 
class AudioDX;

class AudioWave {
public:
	AudioWave();
	virtual ~AudioWave();
	bool Open(const wchar_t *path);
	bool StartRead();
	bool Read(unsigned long size, void *data, unsigned long *nread);
	bool Close();
	WAVEFORMATEX *GetFormat() { return wfex; }
	DWORD CkSize() const { return ckIn.cksize; }

protected:
	WAVEFORMATEX *wfex;
	HMMIO hmmio;	
	MMCKINFO ckIn;
	MMCKINFO ckInRIFF;
} ;

class AudioDXBuffer 
{
protected:
	LPDIRECTSOUNDBUFFER SndBuf;
	LONG Volume;
	LONG Pan;
	DWORD Freq;

public:
	AudioDXBuffer();
	~AudioDXBuffer();

	bool CreateBuffer(AudioDX &ds, WAVEFORMATEX *format, int size);
	bool CreateMainBuffer(AudioDX &ds, int freq=22050);
	bool LoadWave(AudioDX &ds, const wchar_t *path);
	HRESULT Release();
	HRESULT Restore();
	void Terminate();
	bool Play(bool loop=false)
	{
		if(SndBuf == NULL) return false;
		if (FAILED(SndBuf->Play(0, 0, loop? DSBPLAY_LOOPING: 0)))
			return false;
		return true;
	}

	bool Stop(){ return SndBuf->Stop() == DS_OK; }
	bool IsAlive() const { return SndBuf != 0; }
	LPDIRECTSOUNDBUFFER operator ->() { return SndBuf; }
} ;

class AudioDX 
{
public:
	AudioDX();
	~AudioDX();

	bool CreateDS(HWND hWnd, int freq=22050);
	void ReleaseAll();
	HRESULT RestoreAll();
	LPDIRECTSOUND operator ->() { return ds; }

protected:
	LPDIRECTSOUND ds;
	AudioDXBuffer dsbuf;
} ;
