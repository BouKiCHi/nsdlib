/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MusicHeader.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		const		_CHAR	_strName[]	�I�u�W�F�N�g��
//		string				_code
//	���Ԓl
//				����
//==============================================================
MusicHeader::MusicHeader(string _code):
	iBGM(1),
	iSE(0),
	iExternal(-1),
	bank(false),
	op_code(false),
	offsetPCM(0x10000),
	Label("_nsd_"),
	title(""),
	copyright(""),
	composer(""),
	segmentSEQ("RODATA"),
	segmentPCM("PCMDATA")
{
	if(_code.empty()){
		op_code = false;
		romcode = "nsd.bin";
	} else {
		op_code = true;
		romcode = _code;
	}
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
MusicHeader::~MusicHeader(void)
{
}

void	MusicHeader::Set_Title(MMLfile* MML)
{
	title = MML->GetString();
}

void	MusicHeader::Set_Copyright(MMLfile* MML)
{
	copyright = MML->GetString();
}

void	MusicHeader::Set_Composer(MMLfile* MML)
{
	composer = MML->GetString();
}

void	MusicHeader::Set_SegmentSEQ(MMLfile* MML)
{
	segmentSEQ = MML->GetString();
}

void	MusicHeader::Set_SegmentPCM(MMLfile* MML)
{
	segmentPCM = MML->GetString();
}
void	MusicHeader::Set_Label(MMLfile* MML)
{
	Label = MML->GetString();
}

void	MusicHeader::Set_OffsetPCM(MMLfile* MML)
{
	offsetPCM = MML->GetInt();

	if(bank==true){
		MML->Warning(_T("#Bank�w�莞�́A#offsetPCM�͖����ł��B"));
	} else {
		if((offsetPCM < 0xC000) || (offsetPCM > 0x10000)){
			MML->Err(_T("$C000 �` $10000�i��PCM���g�p�j�͈̔͂Ŏw�肵�ĉ������B"));
		}
		if((offsetPCM & 0x003F) != 0){
			MML->Warning(_T("��PCM�̔z�u�A�h���X��64�i$40�jByte�ŃA���C�����g���܂��B"));
			offsetPCM &= 0xFFC0;
			offsetPCM += 0x0040;
		}
	}
}

void	MusicHeader::Set_RomCode(MMLfile* MML)
{
	if(op_code == true){
		MML->Warning(_T("�I�v�V�����X�C�b�`�Ń����N����R�[�h���w�肳��Ă���̂ŁA#code�R�}���h�͖������܂��B"));
		MML->GetString();
	} else {
		romcode = MML->GetString();
	}
}

void	MusicHeader::Set_Number_BGM(MMLfile* MML)
{
	int	_n = MML->GetInt();

	if((_n > 255) || (_n < 0)){
		MML->Err(_T("#BGM��0�`255�͈̔͂Ŏw�肵�Ă��������B"));
	}
	iBGM = (unsigned char)_n;
}

void	MusicHeader::Set_Number_SE(MMLfile* MML)
{
	int	_n = MML->GetInt();

	if((_n > 255) || (_n < 0)){
		MML->Err(_T("#SE��0�`255�͈̔͂Ŏw�肵�Ă��������B"));
	}
	iSE = (unsigned char)_n;
}

void	MusicHeader::Set_External(MMLfile* MML)
{
	int	_n = MML->GetInt();

	if((_n > 0x3F) || (_n < 0)){
		MML->Err(_T("#External��$00�`#3F�͈̔͂Ŏw�肵�Ă��������B"));
	}
	iExternal = _n;
}

void	MusicHeader::Set_Bank(void)
{
	bank		= true;
	offsetPCM	= 0xC000;
}
