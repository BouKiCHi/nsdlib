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
class Envelop :
	public MusicItem
{
//�����o�[�ϐ�
private:
	unsigned	int		m_id;				//�G���x���[�v�ԍ�
				int		Loop_Normal;		//���[�v�ʒu�i�������j
				int		Loop_Release;		//���[�v�ʒu�i�����[�X���j
				bool	Release;			//�����[�X�p�^�[�������邩�ǂ���
				int		ptEnvelop;			//�����|�C���^

//static	const	Command_Info	Command[];	//�R�}���h�̏��

//�����o�[�֐�
public:
				Envelop(MMLfile* MML, unsigned int _id, const _CHAR _strName[] = _T("Envelope"));
				~Envelop(void);
		void	SetLoop(MMLfile* MML, int LoopPoint);
		void	setHold(int length);
		int		sweep(MMLfile* MML);
		void	getAsm(MusicFile* MUS);
};
