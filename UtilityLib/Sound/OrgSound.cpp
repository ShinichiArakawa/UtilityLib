/**
 *	@file	OrgSound.cpp
 *	@date	2017 / 10 / 04
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "OrgSound.h"

#include <MMREG.H>
#include <Msacm.h>	// ACM

#include <mmsystem.h>
#include <dSound.h>
#include <vector>

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"MsAcm32.lib")

#include <wrl.h>

#ifdef __cplusplus_cli
#pragma unmanaged
#endif // __cplusplus_cli

class Utility::OrgSound::Impl
{

private:
	typedef struct tORGWAVEINFO
	{
		LPWAVEFORMATEX	Format;
		uint8_t*		Buffer;
		uint32_t		BufferSize;

	} ORGWAVEINFO, *LPORGWAVEINFO;

	enum class ePause
	{
		None,
		Play,
		Looping,
	};

public:
	Impl() = default;
	~Impl()
	{
		Release();
	}

private:
	LPDIRECTSOUNDBUFFER CreateBuffer(LPVOID argBuffer)
	{
		LPDWORD bufData = reinterpret_cast<unsigned long*>(argBuffer);

		// ファイルを確認する
		const unsigned long Riff = *bufData++;
		unsigned long length = *bufData++;
		unsigned long type = *bufData++;

		if (Riff != mmioFOURCC('R', 'I', 'F', 'F'))
			return nullptr;

		if (type != mmioFOURCC('W', 'A', 'V', 'E'))
			return nullptr;

		LPDWORD end = reinterpret_cast<LPDWORD>(reinterpret_cast<LPBYTE>(bufData) + length - 4);

		// ファイルから必要な情報を得る
		LPWAVEFORMATEX waveHeader = nullptr;
		LPBYTE  waveData = nullptr;
		unsigned long waveSize = 0;

		while (bufData < end)
		{
			type = *bufData++;
			length = *bufData++;

			switch (type)
			{
			case mmioFOURCC('f', 'm', 't', ' '):
				// ヘッダ取得
				if (!waveHeader)
				{
					if (length < sizeof(WAVEFORMAT))
						return false;
					waveHeader = reinterpret_cast<LPWAVEFORMATEX>(bufData);
				}
				break;
			case mmioFOURCC('d', 'a', 't', 'a'):
				// データ取得
				if ((!waveData) || (!waveSize))
				{
					waveData = reinterpret_cast<LPBYTE>(bufData);
					waveSize = length;
				}
				break;
			}
			if (waveHeader && waveData && waveSize)
				break;

			bufData = reinterpret_cast<LPDWORD>(reinterpret_cast<LPBYTE>(bufData) + ((length + 1) & ~1));
		}

		if (end <= bufData)
		{
			OutputDebugString(L"Not Sound data...\n");
			return nullptr;
		}
		if (waveHeader->wFormatTag != WAVE_FORMAT_PCM)
		{
			// PCM以外のフォーマットだったときの処理
			// PCMに変換してからサウンドバッファを作成する

			// 変換元の情報
			ORGWAVEINFO src;
			src.BufferSize = waveSize;
			src.Buffer = waveData;
			src.Format = waveHeader;

			// PCMに変換
			LPORGWAVEINFO info = ConvertFormat(&src);

			// 変換後のデータでサウンドバッファを作成
			Microsoft::WRL::ComPtr<IDirectSoundBuffer> buffer = CreateBuffer(info->Format, info->Buffer, info->BufferSize);

			// 変換で使ったデータを破棄
			ReleaseConvertFormat(info);

			// サウンドバッファを返す(NULLの場合のある)
			return buffer.Detach();
		}

		// PCMデータでサウンドバッファを作成
		return CreateBuffer(waveHeader, waveData, waveSize);
	}

	LPDIRECTSOUNDBUFFER CreateBuffer(LPWAVEFORMATEX argWaveFormat, LPBYTE argWaveData, unsigned long argWaveSize)
	{
		if (!sound_)
			return nullptr;

		DSBUFFERDESC dsbdesc;
		ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_GETCURRENTPOSITION2;
		dsbdesc.dwBufferBytes = argWaveSize;
		dsbdesc.lpwfxFormat = argWaveFormat;

		// バッファ作成
		Microsoft::WRL::ComPtr<IDirectSoundBuffer> buffer;
		HRESULT hr = sound_->CreateSoundBuffer(&dsbdesc, buffer.GetAddressOf(), nullptr);
		if (hr != DS_OK)
			return nullptr;

		// バッファをロック
		LPVOID pMem1, pMem2;
		unsigned long dwSize1, dwSize2;
		hr = buffer->Lock(0, argWaveSize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0);
		if (hr != DS_OK)
			return nullptr;

		// バッファをコピー
		CopyMemory(pMem1, argWaveData, dwSize1);
		if (0 != dwSize2)
			CopyMemory(pMem2, argWaveData + dwSize1, dwSize2);

		// ロックを解除
		buffer->Unlock(pMem1, dwSize1, pMem2, dwSize2);

		return buffer.Detach();
	}


	LPORGWAVEINFO ConvertFormat(LPORGWAVEINFO argSrc)
	{

		WAVEFORMATEX *waveFmt = new WAVEFORMATEX;	//	!<	変換後のフォーマット
		ZeroMemory(waveFmt, sizeof(WAVEFORMATEX));
		waveFmt->wFormatTag = WAVE_FORMAT_PCM;		//	!<	変換後はPCM

		// 変換できるか調べる
		acmFormatSuggest(nullptr, argSrc->Format, waveFmt, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);

		HACMSTREAM hAcm;	//	!<	ACMのハンドル
		//	変換元のストリームを開く
		acmStreamOpen(&hAcm, nullptr, argSrc->Format, waveFmt, nullptr, 0, 0, ACM_STREAMOPENF_NONREALTIME);

		unsigned long dwOutSize;	//	!<	変換後のサイズ
		// 変換後のサイズを取得
		acmStreamSize(hAcm, argSrc->BufferSize, &dwOutSize, ACM_STREAMSIZEF_SOURCE);

		// 変換後用のバッファを確保
		unsigned char *lpBuf = new unsigned char[dwOutSize];

		// 変換情報の設定
		ACMSTREAMHEADER head;
		ZeroMemory(&head, sizeof(head));
		head.cbStruct = sizeof(ACMSTREAMHEADER);
		head.pbSrc = argSrc->Buffer;
		head.cbSrcLength = argSrc->BufferSize;
		head.pbDst = lpBuf;
		head.cbDstLength = dwOutSize;

		// 変換情報をストリームに渡す
		acmStreamPrepareHeader(hAcm, &head, 0);

		// 実際に変換を行う
		acmStreamConvert(hAcm, &head, 0);

		// ACMの操作
		LPORGWAVEINFO pInfo = new ORGWAVEINFO;
		pInfo->Format = waveFmt;
		pInfo->Buffer = lpBuf;
		pInfo->BufferSize = head.cbDstLengthUsed;

		// 変換で使用したワークの破棄
		acmStreamUnprepareHeader(hAcm, &head, 0);
		// ストリームを閉じる
		acmStreamClose(hAcm, 0);

		return pInfo;
	}

	void ReleaseConvertFormat(LPORGWAVEINFO lpInfo)
	{
		if (!lpInfo)
			return;
		delete lpInfo->Buffer;
		delete lpInfo->Format;
		delete lpInfo;

	}
public:

	bool Create(HWND argHWnd)
	{
		// すでに作成済み
		if ((sound_) || (primaryBuf_))
			return false;

		Microsoft::WRL::ComPtr<IDirectSound> sound;
		// インターフェイスを作成
		HRESULT hr = DirectSoundCreate(nullptr, sound.GetAddressOf(), nullptr);
		if (hr != DS_OK)
		{
			OutputDebugString(L"Create DirectSound Failed...\n");
			return false;
		}

		// 協調レベルを設定
		hr = sound->SetCooperativeLevel(argHWnd, DSSCL_NORMAL);
		if (hr != DS_OK)
		{
			OutputDebugString(L"Set CooperativeLevel Failed...\n");
			return false;
		}

		DSBUFFERDESC dsbdesc;
		ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));

		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
		dsbdesc.dwBufferBytes = 0;
		dsbdesc.lpwfxFormat = nullptr;

		// バッファ作成
		Microsoft::WRL::ComPtr<IDirectSoundBuffer> buffer;
		hr = sound->CreateSoundBuffer(&dsbdesc, buffer.GetAddressOf(), nullptr);
		if (FAILED(hr))
			return false;

		// 新しいインターフェイスとバッファをメンバーに設定する
		sound_ = sound;
		primaryBuf_ = buffer;

		return true;

	}

	bool CreateBuffer(LPVOID argBuf, unsigned int argIndex)
	{
		Microsoft::WRL::ComPtr<IDirectSoundBuffer> buffer;

		// バッファの開放
		ReleaseBuffer(argIndex);

		buffer = CreateBuffer(argBuf);

		if (!buffer)
			return false;

		if (bufferArray_.size() <= argIndex)
		{
			bufferArray_.resize(argIndex + 1);
		}
		bufferArray_[argIndex] = buffer;

		return true;
	}

	void ReleaseBuffer(unsigned int argIndex)
	{
		if (bufferArray_.size() <= static_cast<int>(argIndex))
			return;

		bufferArray_[argIndex].Reset();
	}

	void ReleaseBufferAll()
	{
		bufferArray_.clear();
	}

	bool Play(unsigned int argIndex, int argPos, BOOL argIsLoop)
	{
		if (!sound_)
			return false;

		// 指定のインデックスはない
		if (bufferArray_.size() <= static_cast<int>(argIndex))
			return false;

		auto buffer = bufferArray_.at(argIndex);

		if (!buffer)
			return false;

		if (argPos >= 0)
			buffer->SetCurrentPosition(argPos);

		unsigned long dwLoop = 0;
		if (argIsLoop)
			dwLoop = DSBPLAY_LOOPING;

		HRESULT hr = buffer->Play(0, 0, dwLoop);

		if (hr != DS_OK)
			return false;

		return true;
	}

	bool Replay(unsigned int argIndex)
	{
		return Play(argIndex, -1, false);
	}

	int PauseAll()
	{
		if (IsPause())
			return GetPauseNum();

		unsigned long dwStatus;

		for (unsigned long lBufferIndex = 0, lBufferSize = bufferArray_.size(); lBufferIndex < lBufferSize; lBufferIndex++)
		{
			Microsoft::WRL::ComPtr<IDirectSoundBuffer> lpSoundBuffer = bufferArray_[lBufferIndex];

			int iPlay = static_cast<int>(ePause::None);

			if (lpSoundBuffer)
			{
				HRESULT hResult = lpSoundBuffer->GetStatus(&dwStatus);

				if (hResult == DS_OK)
				{
					if (dwStatus & DSBSTATUS_PLAYING)
					{
						lpSoundBuffer->Stop();
						iPlay = (dwStatus & DSBSTATUS_LOOPING) ?
							static_cast<int>(ePause::Looping) :	// ループで再生
							static_cast<int>(ePause::Play);		// 普通に再生
					}
				}

			}
			bufPauseArray_.push_back(iPlay);
		}

		return GetPauseNum();
	}

	bool IsPause()
	{
		if (bufPauseArray_.empty())
			return false;

		return true;
	}

	int GetPauseNum()
	{
		int count = 0;

		for (unsigned long lBufferIndex = 0, lBufferSize = bufPauseArray_.size(); lBufferIndex < lBufferSize; lBufferIndex++)
		{
			if (bufPauseArray_[lBufferIndex])
				count++;
		}

		return count;
	}

	bool Stop(uint32_t argIndex)
	{
		if (!sound_)
			return false;

		if (bufferArray_.size() <= static_cast<int>(argIndex))
			return false;

		// バッファ取得
		auto buffer = bufferArray_[(static_cast<int>(argIndex))];

		if (!buffer)
			return false;

		unsigned long dwStatus;
		// 再生中か
		HRESULT hr = buffer->GetStatus(&dwStatus);
		if (hr != DS_OK)
			return false;
		if (!(dwStatus & DSBSTATUS_PLAYING))
			return false;

		// サウンド停止
		hr = buffer->Stop();

		if (hr != DS_OK)
			return false;

		return true;
	}

	int SolvePause()
	{
		if (!IsPause())
			return false;

		int count = 0;

		for (unsigned long lBufferIndex = 0, lBufferSize = bufferArray_.size(); lBufferIndex < lBufferSize; lBufferIndex++)
		{
			if (lBufferSize <= lBufferIndex)
				break;

			auto buffer = bufferArray_[lBufferIndex];

			if (buffer)
			{
				switch (bufPauseArray_[lBufferIndex])
				{
				case static_cast<int>(ePause::Play) :		// はじめから再生
					buffer->Play(0, 0, 0);
					count++;
					break;
				case static_cast<int>(ePause::Looping) :		// ループで再生
					buffer->Play(0, 0, DSBPLAY_LOOPING);
					count++;
					break;
				}
			}
		}

		bufPauseArray_.clear();

		return count;
	}

	void Release()
	{
		ReleaseBufferAll();

		primaryBuf_.Reset();
		sound_.Reset();
	}

	void SetVolume(uint32_t argIndex, int argVolume)
	{
		if (bufferArray_.size() <= argIndex)
			return;

		auto buffer = bufferArray_[argIndex];

		if (!buffer)
			return;

		buffer->SetVolume(argVolume);
	}

	void SetPan(uint32_t argIndex, int argPan)
	{
		if (bufferArray_.size() <= argIndex)
			return;

		auto buffer = bufferArray_[argIndex];

		if (!buffer)
			return;

		buffer->SetPan(argPan);
	}

private:

	Microsoft::WRL::ComPtr<IDirectSound>		sound_;
	Microsoft::WRL::ComPtr<IDirectSoundBuffer>  primaryBuf_;

	std::vector<Microsoft::WRL::ComPtr<IDirectSoundBuffer>> bufferArray_;	//	!<	バッファテーブル
	std::vector<int> bufPauseArray_;											//	!<	ポーズ状態

};

Utility::OrgSound::OrgSound() 
	: pImpl(new Impl())
{
}
Utility::OrgSound::~OrgSound() = default;

bool Utility::OrgSound::Create(HWND argHWnd)
{
	return pImpl->Create(argHWnd);

}

bool Utility::OrgSound::CreateBuffer(void* argBuf, uint32_t argIndex)
{
	return pImpl->CreateBuffer(argBuf, argIndex);
}

void Utility::OrgSound::ReleaseBuffer(uint32_t argIndex)
{
	pImpl->ReleaseBuffer(argIndex);
}

void Utility::OrgSound::ReleaseBufferAll()
{
	pImpl->ReleaseBufferAll();
}

bool Utility::OrgSound::Play(unsigned int argIndex)
{
	return pImpl->Play(argIndex, 0, false);
}
bool Utility::OrgSound::Play(unsigned int argIndex, int argPos, BOOL argIsLoop)
{
	return pImpl->Play(argIndex, argPos, argIsLoop);
}

bool Utility::OrgSound::Replay(uint32_t argIndex)
{
	return pImpl->Play(argIndex, -1, false);
}

int Utility::OrgSound::PauseAll()
{
	return pImpl->PauseAll();
}

bool Utility::OrgSound::IsPause()
{
	return pImpl->IsPause();
}

int Utility::OrgSound::GetPauseNum()
{
	return pImpl->GetPauseNum();
}

bool Utility::OrgSound::Stop(uint32_t argIndex)
{
	return pImpl->Stop(argIndex);
}

int Utility::OrgSound::SolvePause()
{
	return pImpl->SolvePause();
}

void Utility::OrgSound::Release()
{
	pImpl->Release();
}

void Utility::OrgSound::SetVolume(uint32_t argIndex,int argVolume)
{
	pImpl->SetVolume(argIndex, argVolume);
}

void Utility::OrgSound::SetPan(uint32_t argIndex,int argPan)
{
	pImpl->SetPan(argIndex, argPan);
}


