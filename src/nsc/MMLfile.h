/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*																*/
/*			�萔��`											*/
/*																*/
/****************************************************************/
struct	Macro_Stack {
				string		name;
	unsigned	int			line;
};

/****************************************************************/
/*																*/
/*			�N���X��`											*/
/*																*/
/****************************************************************/
class MMLfile	// :
//	public FileInput
{
//�����o�[�ϐ�
private:
	vector	<	FileInput*	>		ptcFiles;			//MML�t�@�C��
				FileInput*			nowFile;			//���݂̃t�@�C��
	unsigned	int					iFiles;				//���݂̃t�@�C��No.

	map		< string,		string>	ptcMac;				//Macro������̕ۑ�
	map		< string,		int>	lv_Mac;				//Macro�̊K�w

	vector	<	Macro_Stack	>		s_macro;			//�l�X�g���̃}�N�����X�^�b�N
				Macro_Stack			nowMacro;
				int					p_macro;			//���l�X�g�ځH

				bool				f_macro;			//�}�N���������I�������ɗ��t���O
				bool				f_2to1;				//�}���`�o�C�g������ϊ������H
public:
	map		<unsigned int,	Patch*>	ptcPatch;			//Patch

				int					offset_Ei;			//
				int					offset_Ev;			//
				int					offset_En;			//
				int					offset_Em;			//
				int					iReleaseVolume;		//
				int					iRepeatMode;		//
				int					iTieMode;			//
				int					timebase;			//
				bool				octave_reverse;		//Octave Reverse
				bool				q_reverse;			//q, Q q_reverse
				int					rest;				// r �R�}���h�̋���
				int					wait;				// w �R�}���h�̋���
	unsigned	int					QMax;				// qMax
	unsigned	char				priority;

//�����o�[�֐�
public:
	MMLfile(const char*	strFileName);
	~MMLfile(void);

				bool	eof(void);				//���݂̃t�@�C����EOF�`�F�b�N
				bool	eom(void);				//�l�l�k�̏I���`�F�b�N

				void	include();				//���|�C���^�ɂ���t�@�C����#include����
	
				void	SetMacro(int i_Lv);
				void	DeleteMacro(int i_Lv);
				void	CallMacro(void);
				int		GetMacroNest(void){return(p_macro);};

				void	SetPatch(void);
				bool	ChkPatch(unsigned int _no);

				int		tellg(void);					//���݂̃t�@�C���̃|�C���^�擾
				void	StreamPointerMove(long iSize);	//���݂̃t�@�C���̃|�C���^�ړ�
				void	Back_one(void);					//1�����߂�
				void	Back(void);						//1�����߂��i�S�p�E���p�ϊ��Ή��j

				char	read_char(void);	//1Byte�ǂݍ���
				char	cRead(void);		//1Byte�ǂݍ��݁i�S�p�E���p�ϊ��Ή��j
				char	GetChar(void);		//1Byte�ǂݍ��݁iwith EOF & Commend check�j
				string	GetString(void);	//""�t ������ �ǂݍ���
				int		GetNum(void);		//()�t  ���l  �ǂݍ���
				int		GetInt(void);		//���l�ǂݍ���
				int		GetLength(int DefaultLength);	//�����ǂݍ��݁i���Z�t���j
				int		readLength(int DefaultLength);	//�����ǂݍ���

				int		GetCommandID(const Command_Info _command[], unsigned int _size);	//�R�}���hID���擾

	unsigned	int		GetLine(void){return(nowFile->GetLine());};
				void	SetLine(unsigned int i){nowFile->SetLine(i);};


	void		Err(const _CHAR msg[]);
	void		Warning(const _CHAR msg[]);
};
