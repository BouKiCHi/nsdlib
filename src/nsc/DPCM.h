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
class DPCM :
	public FileInput, public MusicItem
{
//�����o�[�ϐ�
private:
	unsigned	int		m_id;
	unsigned	char	_DPCM_size;

//�����o�[�֐�
public:
						DPCM(MMLfile* MML, const char* dmcfile, unsigned int _id, const _CHAR _strName[] = _T("==== [ DPCM ]===="));
						~DPCM(void);
	unsigned	char	getDPCMsize(void){return(_DPCM_size);};
				void	getAsm(MusicFile* MUS);
};
