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
struct	NSF_Header{
				char	Name[5];			//00	"NESM",0x1A
	unsigned	char	Version;			//05
	unsigned	char	MusicNumber;		//06
	unsigned	char	StartMusicNumber;	//07
	unsigned	short	LoadAddress;		//08
	unsigned	short	InitAddress;		//0A
	unsigned	short	MainAddress;		//0C
				char	Title[32];			//0E
				char	Composer[32];		//2E
				char	Copyright[32];		//4E
	unsigned	short	Frequency_NTSC;		//6E
	unsigned	char	Bank[8];			//70
	unsigned	short	Frequency_PAL;		//78
	unsigned	char	Video;				//7A
	unsigned	char	External;			//7B
	unsigned	char	Null1;				//7C
	unsigned	char	Null2;				//7D
	unsigned	char	Null3;				//7E
	unsigned	char	Null4;				//7F
};

/****************************************************************/
/*																*/
/*			�N���X��`											*/
/*																*/
/****************************************************************/
class MusicFile :
	public FileOutput, public MusicItem
{
//�����o�[�ϐ�
public:
			MusicHeader			Header;		//Header

	map<	int, FDSC*		>	ptcFDSC;	//FDS  wave table (career)
	map<	int, FDSM*		>	ptcFDSM;	//FDS  wave table (modulator)
	map<	int, VRC7*		>	ptcVRC7;	//VRC7 User Instrument
	map<	int, N163*		>	ptcN163;	//N163 wave table
	map<	int, Envelop*	>	ptcEnv;		//Envelop
	map<	int, BGM*		>	ptcBGM;		//BGM
	map<	int, SE*		>	ptcSE;		//SE 
	map<	int, Sub*		>	ptcSub;		//Subroutine Sequence
				DPCMinfo*		cDPCMinfo;

private:
//static	const	Command_Info	Command[];
					string		dpcm_code;
	unsigned	int				nsf_size;

//�����o�[�֐�
public:
	MusicFile(MMLfile* MML, string _code, const _CHAR _strName[]=_T("==== [ Music ] ===="));
	~MusicFile(void);

	void				TickCount(void);

	unsigned	int		SetDPCMOffset(unsigned int iMusSize);

	void	Fix_Address(void);

	//�o�C�i���[�����
	void	make_bin(size_t rom_size, int ptOffset);

	//�ۑ��t�F�[�Y
	void	saveNSF(const char*	strFileName);
	void	saveASM(const char*	strFileName);

	void	Err(const _CHAR msg[]);
	void	Warning(const _CHAR msg[]);

};
