/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*																*/
/*			�N���X��`											*/
/*																*/
/****************************************************************/
class OPSW {
//�����o�[�ϐ�
public:
	unsigned	char		cDebug;			//�f�o�b�O�p
				bool		fErr;			//�G���[�o�͐�	true:�W���G���[�o�́^false:�W���o��
				bool		saveNSF;		//.nsf ���o�͂��邩
				bool		saveASM;		//.s   ���o�͂��邩
				bool		flag_Optimize;	//�œK����L���ɂ��邩�H
				bool		flag_OptObj;	//�œK����L���ɂ��邩�H
				bool		flag_OptSeq;	//�œK����L���ɂ��邩�H
		//		bool		flag_TickCount;	//TickCount�𖳌��ɂ��邩�H
				bool		flag_SearchPass;//SearchPass�̏������ʂ��o�͂��邩
				char		fHelp;			//�w���v���w�肵�����H
				string		strMMLname;		//�w�肵��MML�t�@�C����
				string		strNSFname;		//�w�肵��NSF�t�@�C����
				string		strASMname;		//�w�肵��ASM�t�@�C����
				string		strCodeName;	//ROM Code�̖��O

				SearchPass	m_pass_code;	//�����p�X	".bin"�t�@�C��
				SearchPass	m_pass_dmc;		//�����p�X	".dmc"�t�@�C��
				SearchPass	m_pass_inc;		//�����p�X	�C���N���[�h�t�@�C��

//�����o�[�֐�
public:
		OPSW();								//�������̂�
		OPSW(int argc, char* argv[]);		//�������e����A�N���X�����������t�@�C���I�[�v��
		~OPSW();							//�t�@�C���N���[�Y
private:
void	opError(const _CHAR *stErrMsg);		//�I�v�V�����G���[
void	print_help();						//help message
};
