/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "DPCM.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		FileInput*			DPCMfile	��PCM�i*.dmc�j�̃t�@�C����
//		unsigned	int		_id			�ԍ�
//		const		_CHAR	_strName[]	���̃I�u�W�F�N�g�̖��O
//	���Ԓl
//					����
//==============================================================
DPCM::DPCM(MMLfile* MML, const char* dmcfile, unsigned int _id, const _CHAR _strName[]):
	MusicItem(_strName),
	m_id(_id)
{
	//----------------------
	//Local�ϐ�
	unsigned	int		_size;
	unsigned	int		i = 0;

	fileopen(dmcfile, &cOptionSW->m_pass_dmc);
	_size = GetSize();
	if(_size > 4081){
		MML->Err(_T("��PCM��4081Byte�ȉ��ɂ��Ă��������B"));
	}

	if((_size & 0x000F) != 0x01){
		iSize = (_size & 0x0FF0) + 0x0011;
	} else {
		iSize = _size;
	}
	_DPCM_size = (char)(iSize >> 4);

	code.resize(iSize);

	//��PCM���̂�]��
	while(i < _size){
		code[i] = cRead();
		i++;
	}
	//Padding
	while(i < iSize){
		code[i] = (unsigned char)0xAA;
		i++;
	}

	close();
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
DPCM::~DPCM(void)
{
}
//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		MusicFile*	MUS		�R�[�h���o�͂���ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	DPCM::getAsm(MusicFile* MUS)
{
	*MUS << ".align	$40\n" << MUS->Header.Label.c_str() << "DPCM" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}