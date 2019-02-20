/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

// �ÓI���f��

#ifndef __SUB__
#define __SUB__

// Include files
#include "MMLfile.h"
#include "TrackSet.h"
/****************************************************************/
/*																*/
/*			�N���X��`											*/
/*																*/
/****************************************************************/
class Sub : public TrackSet
{

//�����o�[�ϐ�
private:

public:

	Sub(MMLfile* MML, unsigned int _id, _CHAR _strName[] = _T("Sub"));
	~Sub();

	void				getAsm(MusicFile* MUS);

	unsigned	int		TickCount(MusicFile* MUS, NSD_WORK* work);

};// END CLASS DEFINITION Sub

#endif // __SUB__
