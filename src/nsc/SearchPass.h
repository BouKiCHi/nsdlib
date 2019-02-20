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
class SearchPass
{
protected:
	int					m_count;			//�o�^����
	map<int, string>	m_ptcPass;			//�����p�X�̈ꗗ�i�D�揇�ʁF�o�^���j

public:
						SearchPass(void);
						~SearchPass(void);

	void				clear();					//�o�^�̑S�N���A
	void				debug();					//�f�o�b�O�p

	void				add_one(string* _str);		//�ǉ��i�Pdir�j
	void				add(string*	_str);			//�ǉ��i";"�̋�؂�Ή��j
	void				add(char*	_str);			//�ǉ��i";"�̋�؂�Ή��j

	const char*			get(int id);				//������̎擾
	int					count(){return(m_count);};	//�o�^���ʂ̎擾


};
