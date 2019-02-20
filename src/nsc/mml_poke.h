/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once
#include "MusicEvent.h"

/****************************************************************/
/*																*/
/*			�N���X��`											*/
/*																*/
/****************************************************************/
class mml_poke :
	public MusicEvent
{
//�����o�[�֐�
public:
	mml_poke(unsigned int _addr , unsigned char _data,  const _CHAR _strName[]=_T("Poke"));
	~mml_poke(void);
};
