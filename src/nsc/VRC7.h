/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			�N���X��`											*/
/*																*/
/****************************************************************/
class VRC7 :
	public MusicItem
{
//�����o�[�ϐ�
private:
	unsigned	int		m_id;

//static	const	Command_Info	Command[];	//�R�}���h�̏��

//�����o�[�֐�
public:
				VRC7(MMLfile* MML, unsigned int _id, const _CHAR _strName[] = _T("VRC7"));
				~VRC7(void);
		void	getAsm(MusicFile* MUS);
};
