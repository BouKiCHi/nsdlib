/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "mml_Address.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		unsigned	char 	_code		�R�[�h
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_Address::mml_Address(unsigned char _code, const _CHAR _strName[]):
	f_id(false),
	MusicEvent(_strName)
{

	iSize = 3;
	code.resize(iSize);
	code[0] = _code;
	code[1] = 0;
	code[2] = 0;
}

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		unsigned	char 	_code		�R�[�h
//		unsigned	char 	_data		����1
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_Address::mml_Address(unsigned char _code, unsigned char _data, const _CHAR _strName[]):
	f_id(false),
	MusicEvent(_strName)
{
	iSize = 4;
	code.resize(iSize);
	code[0] = _code;
	code[1] = _data;
	code[2] = 0;
	code[3] = 0;
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
mml_Address::~mml_Address(void)
{
}

//==============================================================
//		�A�h���X�̐ݒ�
//--------------------------------------------------------------
//	������
//		unsigned	int		_addr	�A�h���X
//	���Ԓl
//				����
//==============================================================
void	mml_Address::set_Address(unsigned int _addr)
{
	switch(iSize){
		case(3):
			code[1] = (unsigned char)((_addr     ) & 0xFF);
			code[2] = (unsigned char)((_addr >> 8) & 0xFF);
			break;
		case(4):
			code[2] = (unsigned char)((_addr     ) & 0xFF);
			code[3] = (unsigned char)((_addr >> 8) & 0xFF);
			break;
		default:
			_CERR << _T("mml_Address::set_Address()�֐��ŃG���[���������܂����B") << endl;
			nsc_exit(EXIT_FAILURE);
			break;
	}
	

}

//==============================================================
//		�A�h���X�̎擾
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
/*
unsigned	int	mml_Address::get_Address(void)
{
	return((unsigned char)code[1] + ((unsigned char)code[2]<<8) );
}
*/

//==============================================================
//		id�̐ݒ�
//--------------------------------------------------------------
//	������
//		unsigned	int		_id		�ԍ�
//	���Ԓl
//				����
//==============================================================
void	mml_Address::set_id(unsigned int _id)
{
	f_id = true;
	m_id = _id;
}

//==============================================================
//		id�̎擾
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//		unsigned	int		�ԍ�
//==============================================================
unsigned int	mml_Address::get_id(void)
{
	return(m_id);
}
//==============================================================
//		id�̎擾
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//		unsigned	int		�ԍ�
//==============================================================
bool	mml_Address::get_flag(void)
{
	return(f_id);
}

