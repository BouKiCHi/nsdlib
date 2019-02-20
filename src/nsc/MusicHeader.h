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
class MusicHeader
{
//�����o�[�ϐ�
public:
				bool	bank;
				bool	op_code;		//option �� code �w�肳�ꂽ�H
	unsigned	char	iBGM;			//BGM��
	unsigned	char	iSE;			//SE��
	unsigned	int		offsetPCM;
				int		iExternal;		//�g�������t���O
				string	Label;
				string	title;
				string	copyright;
				string	composer;
				string	segmentSEQ;
				string	segmentPCM;
				string	romcode;
//�����o�[�֐�
public:
				MusicHeader(string _code);
				~MusicHeader(void);
		void	Set_Title(MMLfile* MML);
		void	Set_Copyright(MMLfile* MML);
		void	Set_Composer(MMLfile* MML);
		void	Set_SegmentSEQ(MMLfile* MML);
		void	Set_SegmentPCM(MMLfile* MML);
		void	Set_Label(MMLfile* MML);
		void	Set_OffsetPCM(MMLfile* MML);
		void	Set_RomCode(MMLfile* MML);
		void	Set_Number_BGM(MMLfile* MML);
		void	Set_Number_SE(MMLfile* MML);
		void	Set_External(MMLfile* MML);
		void	Set_Bank(void);
};
