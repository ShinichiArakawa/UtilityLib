/**
 *	@file	Bgm.cpp
 *	@date	2017 / 10 / 04
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Bgm.h"
#include <Windows.h>
#include <mmsystem.h>
#include <string>
#include <tchar.h>

#pragma comment(lib,"winmm.lib")

#ifdef __cplusplus_cli
#pragma unmanaged
#endif

namespace
{
	const int CommandMax = 2048;
	const int AliasMax = 512;
}
/**
 *	@class	CCreateMCICommand
 *	@brief	MCIのコマンド生成クラス
 *	@brief	内部バッファにコマンドの文字列を作成する
 */
class CCreateMCICommand
{
private:

    wchar_t command_[CommandMax];	//	!<	作成したコマンドを入れる
    wchar_t alias_[AliasMax];		//	!<	エイリアスの指定
   /**
	*	@fn		Command
	*	@brief	コマンドを生成する
	*	@param[in]	argCmd		!<	コマンド
	*	@param[in]	argAlias	!<	エイリアス
	*	@param[in]	argIndex	!<	インデックス
	*/
    void Command(wchar_t* argCmd,wchar_t* argAlias, unsigned long argIndex)
    {
        swprintf_s(command_, CommandMax, L"%s %s%d", argCmd, argAlias, argIndex);
    }

public:
	/**
	 *	@constructor	CCreateMCICommand
	 *	@brief			MCIのコマンド生成クラス
	 *	@param[in]		argAlias	!<	エイリアス
	 */
    CCreateMCICommand(wchar_t* argAlias)
    {
		wcscpy_s(alias_, AliasMax, argAlias);
    }
	/**
	 *	@fn			Open
	 *	@brief		メディアをロードする
	 *	@param[in]	argIndex		!<	インデックス
	 *	@param[in]	argFilename		!<	ファイルパス
	 */
    void Open(unsigned long argIndex,wchar_t* argFilename)
    {
        _stprintf_s(command_, CommandMax,L"Open \"%s\" alias %s%d", argFilename, alias_, argIndex);
    }
	/**
	 *	@fn			Play
	 *	@brief		メディアを再生するコマンドを作成する
	 *	@param[in]	argIndex	!<	インデックス
	 */
    void Play(unsigned long argIndex)
    {
        Command(L"Play", alias_, argIndex);
    };
	/**
	 *	@fn			Stop
	 *	@brief		メディアを停止するコマンドを作成する
	 *	@param[in]	argIndex	!<	インデックス
	 */
    void Stop(unsigned long argIndex)
    {
        Command(L"Stop", alias_, argIndex);
    }
	/**
	 *	@fn			Pause
	 *	@brief		メディアをポーズするコマンドを作成する
	 *	@param[in]	argIndex	!<	インデックス
	 */
    void Pause(unsigned long argIndex)
    {
        Command(L"Pause", alias_, argIndex);
    }
	/**
	 *	@fn			Resume
	 *	@brief		ポーズしたメディアを再生するコマンドを作成する
	 *	@param[in]	argIndex	!<	インデックス
	 */
    void Resume(unsigned long argIndex)
    {
        Command(L"Resume", alias_, argIndex);
    }
	/**
	 *	@fn			Close
	 *	@brief		メディアをクローズするコマンドを作成する
	 *	@param[in]	argIndex	!<	インデックス
	 */
    void Close(unsigned long argIndex)
    {
        Command(L"Close", alias_, argIndex);
    }

};

Utility::Bgm::Bgm() = default;
Utility::Bgm::~Bgm() = default;

/**
 *	@fn			Loab
 *	@brief		メディアをロードする
 *	@param[in]	argFilename	!<	ファイルパス
 *	@retval		true		!<	成功
 *	@retval		false		!<	失敗
 */
bool Utility::Bgm::Loab(wchar_t* argFilename)
{
    TCHAR str[CommandMax];
	str[0] = '\0';
    
    swprintf_s(str, L"open %s type mpegvideo alias Music", argFilename);

	MCIERROR errCode =  mciSendString(str, NULL, 0, NULL);
    if(errCode != 0)
    {
        TCHAR error[CommandMax];
        mciGetErrorString(errCode, error, CommandMax);

        // TODO このエラーメッセージをどこで使用するか決めていない
        return false;
    }

    return true;
}

/**
 *	@fn			Unload
 *	@brief		メディアをアンロードする
 *	@retval		true		!<	成功
 *	@retval		false		!<	失敗
 */
bool Utility::Bgm::Unload()
{
    MCIERROR errCode = mciSendString(L"Close Music", NULL, 0, NULL);
    if(errCode != 0)
		return false;
    return true;
}
/**
 *	@fn			Play
 *	@brief		メディアを再生する(繰り返し)
 *	@retval		true		!<	成功
 *	@retval		false		!<	失敗
 */
bool Utility::Bgm::Play()
{
    MCIERROR errCode = mciSendString(L"Seek Music to start", NULL, 0, NULL);
    if(errCode != 0) 
		return false;
	errCode = mciSendString(L"Play Music Repeat", NULL, 0, NULL);
    if(errCode != 0)
		return false;
    return true;
}

/**
 *	@fn			Stop
 *	@brief		再生中のメディアを停止する
 *	@retval		true		!<	成功
 *	@retval		false		!<	失敗
 */
bool Utility::Bgm::Stop()
{
    MCIERROR errCode = mciSendString( L"Stop Music", NULL, 0, NULL );
    if(errCode != 0)
		return false;
    return true;
}
/**
 *	@fn			Pause
 *	@brief		再生中のメディアをポーズする
 *	@retval		true		!<	成功
 *	@retval		false		!<	失敗
 */
bool Utility::Bgm::Pause()
{
    MCIERROR errCode = mciSendString(L"Pause Music", NULL, 0, NULL);
    if(errCode != 0)
		return false;
    return true;
}
/**
 *	@fn			Resume
 *	@brief		ポーズ中のメディアを再生する
 *	@retval		true		!<	成功
 *	@retval		false		!<	失敗
 */
bool Utility::Bgm::Resume()
{
    MCIERROR errCode = mciSendString(L"Resume Music", NULL, 0, NULL);
    if(errCode != 0 ) 
		return false;
    return true ;
}

/**
 *	@fn			Volume
 *	@brief		ボリュームの変更
 *	@param[in]	volume	!<	0～1000のボリューム
 *	@retval		true	!<	成功
 *	@retval		false	!<	失敗
 */
bool Utility::Bgm::Volume(int argVolume)
{
    TCHAR str[2048];
	str[0] = '\0';
	if(argVolume < 0) 
		argVolume = 0;
	if(argVolume > 1000) 
		argVolume = 1000;
	swprintf_s(str,L"setaudio Music volume to %d", argVolume);
	MCIERROR errCode = mciSendString(str, NULL, 0, NULL);
    if(errCode != 0)
		return false;
    return true ;
}

// 参考
//char buff[ 512 ];
//char *file = "C:\\My Music\\music.wma";　←ファイル名(WMA,MID,MP3,WAVなど)
//
//sprintf( buff, "Open \"%s\" argAlias Music", file );
//mciSendString( buff, NULL, 0, NULL );　←メディアを開く
//getchar();
//mciSendString( "Play Music Repeat", NULL, 0, NULL );　←再生(繰り返し)
//getchar();
//mciSendString( "Pause Music", NULL, 0, NULL );　←一時停止
//getchar();
//mciSendString( "Resume Music", NULL, 0, NULL );　←一時停止から再開
//getchar();
//mciSendString( "Stop Music", NULL, 0, NULL );　←停止
//getchar();
//mciSendString( "Close Music", NULL, 0, NULL );　←メディアを閉じる


#ifdef __cplusplus_cli
#pragma managed
#endif