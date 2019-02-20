/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "FileOutput.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
FileOutput::FileOutput(void)
{
//#ifdef	_WIN32
	imbue( std::locale::classic() );
//#endif
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
FileOutput::~FileOutput(void)
{
}

//--------------------------------
//�t�@�C�����J���@�G���[�����t��
//--------------------------------
void	FileOutput::fileopen(const char*	strFileName){

	//File open
	open(strFileName,ios_base::out | ios_base::binary);
	if(good()==false){
		perror(strFileName);
		nsc_exit(EXIT_FAILURE);
	};
};

//--------------------------------
//���΃V�[�N
//--------------------------------
void	FileOutput::StreamPointerAdd(long iSize){
	seekp((long)iSize,ios::cur);
};

//--------------------------------
//��΃V�[�N
//--------------------------------
void	FileOutput::StreamPointerMove(long iSize){
	seekp((long)iSize,ios::beg);
};

