/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "mml_poke.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		unsigned	int		_addr		�A�h���X
//		unsigned	char	_data		�f�[�^
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_poke::mml_poke(unsigned int _addr , unsigned char _data, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	iSize = 4;
	code.resize(iSize);
	code[0] = nsd_Poke;
	code[1] = (unsigned char)(_addr     );
	code[2] = (unsigned char)(_addr >> 8);
	code[3] = _data;
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
mml_poke::~mml_poke(void)
{
}
