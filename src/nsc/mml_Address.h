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
class mml_Address :
	public MusicEvent
{
//�����o�[�ϐ�
	unsigned	int		m_id;
				bool	f_id;		
//�����o�[�֐�
public:
	mml_Address(unsigned char _code, const _CHAR _strName[]=_T("Address"));
	mml_Address(unsigned char _code, unsigned char _data, const _CHAR _strName[]=_T("Address"));
	~mml_Address(void);

				void	set_Address(unsigned int _addr);
//	unsigned	int		get_Address(void);
				void	set_id(unsigned int _id);
	unsigned	int		get_id(void);
				bool	get_flag(void);
};
