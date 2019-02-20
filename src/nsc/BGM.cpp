/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "BGM.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//		unsigned	int		_id			BGM番号
//		const		_CHAR	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
BGM::BGM(MMLfile* MML, unsigned int _id, const _CHAR _strName[]):
	TrackSet(MML, _id, false, false, _strName)
{
}

BGM::BGM(MMLfile* MML, unsigned int _id):
	TrackSet(MML, _id, false, false, _T("BGM"))
{
}


//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
BGM::~BGM(void)
{
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		コードを出力する曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	BGM::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "BGM" << m_id << ":" << endl;
	TrackSet::getAsm(MUS);
}
