/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*																*/
/*			�\���̒�`											*/
/*																*/
/****************************************************************/
class NSD_WORK{

public:
	int	gatemode;
	int	length;
	int	gate_q;
	int	gate_u;

	int	octave;
	int	octave1;		//One time octave
	int	detune_cent;
	int	detune_fine;

	int	trans;

	int	voice;
	int	volume;

	int	env_volume;
	int	env_voice;
	int	env_frequency;
	int	env_note;

	//APU
	int	sweep;
	int	apu_tri_time;

	//FDS
	int	fds_frequency;
	int	fds_sweepbias;

	//N163
	int	n163_num;
	int	n163_frequency;

	//PSG
	int	psg_switch;

			NSD_WORK(void){};
			~NSD_WORK(void){};

	void	init(void){
		//�����h���C�o�����l��ݒ�
		gatemode		= -1;
		length			= 24;
		gate_q			= 0;
		gate_u			= 0;
		octave			= 3;
		octave1			= 0;
		detune_cent		= 0;
		detune_fine		= 0;
		trans			= 0;
		voice			= -1;
		volume			= 15;
		env_volume		= -1;
		env_voice		= -1;
		env_frequency	= -1;
		env_note		= -1;
		sweep			= -1;
		apu_tri_time	= -1;
		fds_frequency	= -1;
		fds_sweepbias	= -1;
		n163_num		= -1;
		n163_frequency	= -1;
		psg_switch		= -1;
	}

	void	set(NSD_WORK* work){
		gatemode		=	work->gatemode;
		length			=	work->length;
		gate_q			=	work->gate_q;
		gate_u			=	work->gate_u;
		octave			=	work->octave;
		octave1			=	work->octave1;
		detune_cent		=	work->detune_cent;
		detune_fine		=	work->detune_fine;
		trans			=	work->trans;
		voice			=	work->voice;
		volume			=	work->volume;
		env_volume		=	work->env_volume;
		env_voice		=	work->env_voice;
		env_frequency	=	work->env_frequency;
		env_note		=	work->env_note;
		sweep			=	work->sweep;
		apu_tri_time	=	work->apu_tri_time;
		fds_frequency	=	work->fds_frequency;
		fds_sweepbias	=	work->fds_sweepbias;
		n163_num		=	work->n163_num;
		n163_frequency	=	work->n163_frequency;
		psg_switch		=	work->psg_switch;
	}

	void	get(NSD_WORK* work){work->set(this);};
};
/****************************************************************/
/*																*/
/*			�N���X��`											*/
/*																*/
/****************************************************************/
class MusicTrack :
	public MusicItem
{
//�����o�[�ϐ�
public:
	//----------------------------------
	//�����h���C�o�@�V�~�����[�g�pWORK
	NSD_WORK	nsd;

private:
	//----------------------------------
	//Tick �J�E���g�p
				int		iTickTotal;
				int		iTickLoop;

	//----------------------------------
	//�R���p�C������
	unsigned	int		offset_now;				//���݂̃I�t�Z�b�g
				bool	compile_flag;			//���݃R���p�C�����H

				bool	jump_flag;				// J

	//----------------------------------
	//����
	//			int		DefaultLength;			//l
				int		opt_DefaultLength;
	
	//----------------------------------
	//�N�I���^�C�Y
				int		QMax;					//QMax
				int		gatetime_q;				//q
				int		gatetime_Q;				//Q
				int		opt_gatetime_q;			//
				int		opt_gatetime_u;			//

	//----------------------------------
	//����
//				char	volume;					//���݂̉���
				int		opt_volume;

	//----------------------------------
	//�I�N�^�[�u
//				char	octave;					//���݂̃I�N�^�[�u
//				char	octave1;				//�ꎟ�I�ȑ��΃I�N�^�[�u�@�v�Z�p
				int		opt_octave;

	//----------------------------------
	//�m�[�g
	mml_note*			_old_note;

	//----------------------------------
	//��������p
				char	KeySignature[8];		//����(c,d,e,f,g,a,b,r)
				char	nowKey;					//���݂̒�
				char	nowScale;				//���݂̃X�P�[���i���[�h�j
				
	//----------------------------------
	//�ڒ�
				int		iKeyShift;				//k
				
//				int		iTranspose;				//_

	//----------------------------------
	//�^���G�R�[
				bool	echo_already;			//���ɐ����������H
				bool	echo_vol_ret;			//�G�R�[�̉��ʂ����ɖ߂������H

				bool	echo_flag;				//�^���G�R�[ �t���O
				bool	echo_slur;				//�^���G�R�[ �X���[�łȂ���H
				int		echo_length;			//�^���G�R�[ ����
	unsigned	char	echo_volume;			//�^���G�R�[ ����
	unsigned	char	echo_value;				//�^���G�R�[ ���O�H
				char	oldNote[256];			//�^���G�R�[�p�����O�o�b�t�@
	unsigned	char	pt_oldNote;				//�^���G�R�[�p�����O�o�b�t�@�@�|�C���^

	//----------------------------------
	//�p�b�`
				bool	f_Patch;				//�p�b�`�������H
	unsigned	int		i_Patch;

	//���݂̏�ԁi�ݒ萔�l�j
//	unsigned	int		iVoi;			//
//	unsigned	int		iEvoi;			//
//	unsigned	int		iEvol;			//
//	unsigned	int		iEm;			//
//	unsigned	int		iEn;			//
	unsigned	char	iSweep;			//
	unsigned	int		iSub;			//�T�u���[�`���p

	//���݂̏�ԁi�G���x���[�v��sw�j
				bool	sw_Evoi;		//
				bool	sw_Evol;		//
				bool	sw_Em;			//
				bool	sw_En;			//

	//�ݒ肷�邩�ǂ����idefailt = false�j
				bool	f_opt_Voi;		//
				bool	f_opt_Evoi;		//
				bool	f_opt_Evol;		//
				bool	f_opt_Em;		//
				bool	f_opt_En;		//
				bool	f_opt_Key;		//
				bool	f_opt_Sweep;	//
				bool	f_opt_Sub;		//�T�u���[�`���i�p�b�`�p�j

	//----------------------------------
	//�������[�v
				bool	loop_flag;				// L �R�}���h�o���������H
	unsigned	int		offset_loop;			// L �R�}���h�̃I�t�Z�b�g

	//----------------------------------
	//���s�[�g�֌W
	mml_Address*		_old_repeatA_Branch;
	mml_repeat*			_old_repeat;

	unsigned	int		offset_repeat_a_s;		//�O��� [  �R�}���h�̃I�t�Z�b�g
	unsigned	int		offset_repeat_a_b;		//�O��� :  �R�}���h�̃I�t�Z�b�g
	unsigned	int		offset_repeat_b_s;		//�O��� |: �R�}���h�̃I�t�Z�b�g
	unsigned	int		offset_repeat_b_b;		//�O��� \  �R�}���h�̃I�t�Z�b�g
				int		count_repeat_a;

			vector<	int			>			repeat_type;		//�ǂ̃��s�[�g���g���Ă��邩�H
			vector<	int			>::iterator	it_repeat_type;

			unsigned	int					sp_repeat_c;		//���s�[�g(C)�̃X�^�b�N�|�C���^
	list<	unsigned	int				>	st_ct_repeat_c;
	list<	list<	MusicItem*>::iterator>	st_it_repeat_c_s;
	list<	list<	MusicItem*>::iterator>	st_it_repeat_c_b;
	list<	list<	MusicItem*>::iterator>	st_it_repeat_c_e;
	list<	unsigned	int				>::iterator		it_ct_repeat_c;
	list<	list<	MusicItem*>::iterator>::iterator	it_it_repeat_c_s;
	list<	list<	MusicItem*>::iterator>::iterator	it_it_repeat_c_b;
	list<	list<	MusicItem*>::iterator>::iterator	it_it_repeat_c_e;

	//----------------------------------
	//�I�u�W�F�N�g
	vector<	mml_Address*	>	ptcFDSC;		//FDS Carrer
	vector<	mml_Address*	>	ptcFDSM;		//FDS Modlator
	vector<	mml_Address*	>	ptcOPLL;		//VRC7, OPLL
	vector<	mml_Address*	>	ptcWave;		//N163
	vector<	mml_Address*	>	ptcSE;			//���ʉ��R�}���h�ꗗ
	vector<	mml_Address*	>	ptcSub;			//�T�u���[�`���R�}���h�ꗗ
	vector<	mml_Address*	>	ptcEnv;			//�G���x���[�v�R�}���h�ꗗ


//�����o�[�֐�
public:
			MusicTrack(MMLfile* MML, const _CHAR _strName[] = _T("==== [ Music Track ]===="));
			~MusicTrack(void);

	unsigned	int		TickCount(MusicFile* MUS, NSD_WORK* work);
	unsigned	int		TickCount(MusicFile* MUS);
	unsigned	int		GetTickTotal(void){	return(iTickTotal);};
	unsigned	int		GetTickLoop(void){	return(iTickLoop);};

				void	OptimizeDefineCheck(MusicFile* MUS);
				void	Fix_Address(MusicFile* MUS);
				void	SetEvent(MusicItem* _item);		//�C�x���g�̒ǉ�

				//���̃g���b�N���R���p�C�����邩�ǂ���
				bool	GetCompileFlag(void){return(compile_flag);};
				void	SetCompileFlag(bool _flag){compile_flag = _flag;};

				//----------------------------------
				//���̃g���b�N�ɂ��������l�l�k�R�}���h
				size_t	SetEnd(MMLfile* MML);			//�L�q�u���b�N�I��
				void	SetLoop();						//�������[�v

				void	SetJump(MMLfile* MML);			//�W�����v

				void	SetRepeat_Start(MMLfile* MML);
				void	SetRepeat_End(MMLfile* MML);
				void	SetRepeat_Branch(MMLfile* MML);

				void	SetRepeat_A_Start(MMLfile* MML);
				void	SetRepeat_A_End(MMLfile* MML);
				void	SetRepeat_C_Start(MMLfile* MML);
				void	SetRepeat_C_End(MMLfile* MML);

				void	SetRepeat_B_Start();
				void	SetRepeat_B_Branch(MMLfile* MML);
				void	SetRepeat_B_End(MMLfile* MML);
		mml_Address*	CopyAddressEvent(unsigned char cOpCode, string* sOpCode, list<MusicItem*>::iterator pt_itMusic);
				void	CopyEnvEvent(unsigned char cOpCode, string* sOpCode, list<MusicItem*>::iterator pt_itMusic);

				void	SetSE(MMLfile* MML);
				void	SetSubroutine(unsigned int _no);
				void	SetSubWithParch(unsigned int _no,bool _f);

				void	SetPatch(MMLfile* MML);	
				void	SetPatch();				//@P off
				void	CallPatch(MMLfile* MML, char _note);

				void	SetEnvelop_Evoi(unsigned int _no);
				void	SetEnvelop_Evol(unsigned int _no);
				void	SetEnvelop_Em(unsigned int _no);
				void	SetEnvelop_En(unsigned int _no);
				void	SetVoice(unsigned int _no);		//E@ off
				void	SetEnvelop_Evol();				//Ev off
				void	SetEnvelop_Em();				//Em off
				void	SetEnvelop_En();				//En off

				void	SetSweep(MMLfile* MML);
				void	SetSweep(unsigned char _c);

				void	SetFDSC(MMLfile* MML);			//@FC
				void	SetFDSM(MMLfile* MML);			//@FM
				void	SetVRC7(MMLfile* MML);			//@V
				void	SetN163(MMLfile* MML);			//@N
				void	SetN163_Load(MMLfile* MML);		//@NL
				void	SetN163_Set(MMLfile* MML);		//@NS

				void	Set_q(int i);
				void	Set_u(int i);
				void	SetGatetime_Q(MMLfile* MML);
				void	SetGatetime(MMLfile* MML);
				void	SetGatetime_u(MMLfile* MML);

				void	SetReleaseMode(MMLfile* MML);
				void	SetReleaseVoice(MMLfile* MML);
				void	SetReleaseVolume(MMLfile* MML);

				void	SetKeyFlag(char _c, char _d, char _e, char _f, char _g, char _a, char _b);
				void	SetKey(int _key, int _scale);

				void	SetMajor();
				void	SetMinor();
				void	SetHMinor(MMLfile* MML);
				void	SetMMinor(MMLfile* MML);
				void	SetScale(MMLfile* MML);
				void	SetKeySignature(MMLfile* MML);	//�����̐ݒ�

				void	SetEcho(void);
				void	SetEcho(MMLfile* MML);
				void	SetEchoBuffer(MMLfile* MML,int note);
				void	ResetEcho();
				void	EchoVolRet();
				void	GenerateEcho(MMLfile* MML, int Length, int GateTime, bool	Slur);
				char	calc_note(MMLfile*	MML,int note);
				int		calc_length(MMLfile* MML);
				int		calc_gate(MMLfile* MML);
				bool	calc_slur(MMLfile* MML);
				void	SetNote(MMLfile* MML, int _key, int Length, int GateTime, bool Slur);
				void	SetNote(MMLfile* MML, int note);
				void	SetRest(MMLfile* MML, int mode);
				void	SetTai(MMLfile* MML);
				void	SetLength(MMLfile* MML);

				void	SetProtament(MMLfile* MML);
				void	SetProtament(MMLfile* MML, unsigned char iTempo);

				void	SetKeyShift(MMLfile* MML);
				void	SetKeyShift_Relative(MMLfile* MML);

				void	SetTranspose(int _no);
				void	SetTranspose_Relative(int _no);

				void	SetOctave(MMLfile* MML);
				void	SetOctaveInc();
				void	SetOctaveDec();
				void	SetOctaveOne_Inc();
				void	SetOctaveOne_Dec();
		
				void	SetVolume(MMLfile* MML);
				void	SetVolumeInc(MMLfile* MML);
				void	SetVolumeDec(MMLfile* MML);

//	unsigned	int		GetDefaultLength(void){return(nsd.length);};

				void	Reset_opt(void){
					opt_octave			= -1;
					opt_volume			= -1;
					opt_gatetime_q		= -1;
					opt_gatetime_u		= -1;
					opt_DefaultLength	= -1;
					f_opt_Voi			= false;	//
					f_opt_Evoi			= false;	//
					f_opt_Evol			= false;	//
					f_opt_Em			= false;	//
					f_opt_En			= false;	//
					f_opt_Key			= false;	//
					f_opt_Sweep			= false;	//
					f_opt_Sub			= false;	//
			//		if((echo_flag == true) && (echo_slur == false)){
			//			echo_vol_ret	= true;		//
			//		}
				}
};
