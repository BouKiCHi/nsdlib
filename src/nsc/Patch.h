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
typedef	struct{

	//�ݒ萔�l
	unsigned	int		iVoi;		//
	unsigned	int		iEvoi;		//
	unsigned	int		iEvol;		//
	unsigned	int		iEm;		//
	unsigned	int		iEn;		//
	unsigned	int		iKey;		//KeyShift
	unsigned	char	iSweep;		//
	unsigned	int		iSub;		//
	unsigned	int		iGate_q;	//
	unsigned	int		iGate_u;	//

	//�ݒ肷�邩�ǂ����idefailt = false�j
				bool	fVoi;		//
				bool	fEvoi;		//
				bool	fEvol;		//
				bool	fEm;		//
				bool	fEn;		//
				bool	fKey;		//
				bool	fSweep;		//
				bool	fSub;		//
				bool	fSub_opt;	//true�ōœK������
				bool	fGate_q;	//
				bool	fGate_u;	//

	//�G���x���[�v��sw�i�L��ꍇ true�j
				bool	sw_Evoi;	//
				bool	sw_Evol;	//
				bool	sw_Em;		//
				bool	sw_En;		//

} patch_scrap;

/****************************************************************/
/*																*/
/*			�N���X��`											*/
/*																*/
/****************************************************************/
class Patch
{
//�����o�[�ϐ�
protected:

		unsigned	int					m_id;			//�p�b�`�ԍ�
	map<unsigned	int, patch_scrap*>	m_Patch;		//�p�b�`�̏��

		unsigned	int					m_kn;			//�������̃m�[�g�ԍ�

		patch_scrap*					m_now_Patch;	//�������̃p�b�`�̃|�C���^


//�����o�[�֐�
public:
						Patch(	MMLfile* MML, int _id);		
						~Patch(void);					

	//Patch �I�u�W�F�N�g������
				void	setKey(	MMLfile* MML, int key);
				void	setN(	MMLfile* MML, int note);

				void	DebugMsg(void);

	//�V�[�P���X�L�q�u���b�N����ĂԊ֐��Q
				void	setNote(int i);		//�m�[�g�ԍ��̃Z�b�g

	unsigned	int		get_iVoi(void){		return(m_now_Patch->iVoi);};
	unsigned	int		get_iEvoi(void){	return(m_now_Patch->iEvoi);};
	unsigned	int		get_iEvol(void){	return(m_now_Patch->iEvol);};
	unsigned	int		get_iEm(void){		return(m_now_Patch->iEm);};
	unsigned	int		get_iEn(void){		return(m_now_Patch->iEn);};
	unsigned	int		get_iKey(void){		return(m_now_Patch->iKey);};
	unsigned	char	get_iSweep(void){	return(m_now_Patch->iSweep);};
	unsigned	int		get_iSub(void){		return(m_now_Patch->iSub);};
	unsigned	int		get_iGate_q(void){	return(m_now_Patch->iGate_q);};
	unsigned	int		get_iGate_u(void){	return(m_now_Patch->iGate_u);};

				bool	get_fVoi(void){		return(m_now_Patch->fVoi);};
				bool	get_fEvoi(void){	return(m_now_Patch->fEvoi);};
				bool	get_fEvol(void){	return(m_now_Patch->fEvol);};
				bool	get_fEm(void){		return(m_now_Patch->fEm);};
				bool	get_fEn(void){		return(m_now_Patch->fEn);};
				bool	get_fKey(void){		return(m_now_Patch->fKey);};
				bool	get_fSweep(void){	return(m_now_Patch->fSweep);};
				bool	get_fSub(void){		return(m_now_Patch->fSub);};
				bool	get_fSub_opt(void){	return(m_now_Patch->fSub_opt);};
				bool	get_fGate_q(void){	return(m_now_Patch->fGate_q);};
				bool	get_fGate_u(void){	return(m_now_Patch->fGate_u);};

				bool	get_sw_Evoi(void){	return(m_now_Patch->sw_Evoi);};
				bool	get_sw_Evol(void){	return(m_now_Patch->sw_Evol);};
				bool	get_sw_Em(void){	return(m_now_Patch->sw_Em);};
				bool	get_sw_En(void){	return(m_now_Patch->sw_En);};

};
