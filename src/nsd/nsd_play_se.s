;=======================================================================
;
;	NES Sound Driver & library (NSD.lib)	Library Functions
;
;-----------------------------------------------------------------------
;
;	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
;	For conditions of distribution and use, see copyright notice
;	  in "nsd.h" or "nsd.inc".
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_play_se

	.import		_nsd_play
	.import		_nsd_stop_se

.ifdef	DPCMBank
	.import		_nsd_ptr_bank
.endif

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


.code
;=======================================================================
;	void	__fastcall__	nsd_play_se(void* se );
;-----------------------------------------------------------------------
;<<Contents>>
;	Play start the SE
;<<Input>>
;	ax	pointer of SE
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_play_se: near

	pha
	lda	#nsd_flag::Disable
	ora	__flag
	sta	__flag			;�T�E���h�����֎~
	pla

	sta	__ptr
	stx	__ptr + 1

.ifdef	DPCMBank
	stx	__tmp + 1		;�ۑ�
	jsr	_nsd_ptr_bank
.endif


	lda	__channel
	pha				;BGM����Ă΂��̂ŁA�ۑ����Ă����B

	ldy	#0

	;-----------------------
Channel:
	lda	(__ptr),y		; a = number of track
	iny
	shl	a, 1
	add	#nsd::TR_SE
	sta	__channel

	;-----------------------
Priority:
	lda	(__ptr),y		;
	iny
	and	#$03
	shl	a,2
	sta	__tmp			; __tmp  = �D��x

	lda	#nsd_flag::SE
	bit	__flag			;
	bne	@S			;1(SE�Đ�����)��������A�Đ���

	ldx	#0			;0(SE�Đ���)��������A�D��x��r
	lda	__flag
	and	#nsd_flag::Priority	; 0000-1100<2>
	cmp	__tmp
	bcc	@L			;�D��x����

@S:
	lda	__flag
	and	#<~nsd_flag::Priority
	ora	__tmp
	sta	__flag			;�D��x�X�V
	ldx	#1			;����̌��ʉ���D�悷��
@L:
	stx	__tmp			; �� 0 ��������O��̕����D��x�����B

	;-----------------------
	;Init the channel structure
	ldx	#nsd::TR_SE

Loop:
	cpx	__channel
	bcs	Loop_End

	lda	__tmp
	bne	@Done			;�D��x�����������疳�����ōĐ�

	lda	__Sequence_ptr + 1,x
	beq	@Done1			;�g���b�N���g���ĂȂ������疳�����ōĐ�

	iny
	iny
	jmp	@None

@Done1:
	lda	__tmp
@Done:
	pha				;���ʉ��D��x���̕ۑ�
	lda	(__ptr),y
	iny
	sta	__tmp
	ora	(__ptr),y
	beq	@L
	jsr	_nsd_play

	lda	#$08
.ifdef	SE
	cpx	#nsd::TR_SE_Pluse1
	bne	@L0
	sta	APU_PULSE1RAMP
@L0:
.endif
	cpx	#nsd::TR_SE_Pluse2
	bne	@L1
	sta	APU_PULSE2RAMP
@L1:

@L:
	iny
	pla
	sta	__tmp			;���ʉ��D��x���̕��A

@None:
	inx
	inx
	cpx	#nsd::TR_SE + nsd::SE_Track * 2
	bcc	Loop
Loop_End:


	;-----------------------
	;������������

.ifdef	SE
	lda	#$FF
	sta	__se_tri_time		;�O�p�g ����
.endif

	;-----------------------

	pla
	sta	__channel

	lda	#<~(nsd_flag::SE + nsd_flag::Disable)
	and	__flag
	sta	__flag			;SE & Main Enable

	rts
.endproc
