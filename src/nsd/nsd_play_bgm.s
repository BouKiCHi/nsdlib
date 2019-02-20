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

	.export		_nsd_play_bgm
	.export		_nsd_play

	.import		_nsd_stop_bgm

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
;	void	__fastcall__	nsd_play_bgm(void* bgm );
;-----------------------------------------------------------------------
;<<Contents>>
;	Play start the BGM
;<<Input>>
;	ax	pointer of BGM
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_play_bgm: near

	pha
	txa
	pha

	jsr	_nsd_stop_bgm		;stop the BGM (disable BGM)

	lda	#nsd_flag::Disable
	ora	__flag
	sta	__flag			;�T�E���h�����֎~

	pla
	sta	__ptr + 1
.ifdef	DPCMBank
	sta	__tmp + 1	;�ۑ�
.endif
	pla
	sta	__ptr
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif

	ldy	#0

	;-----------------------
Channel:
	lda	(__ptr),y		; a = number of track
	iny
	shl	a, 1
	sta	__channel

	;-----------------------
Reserve:
	iny

	;-----------------------
	;Init the channel structure
	ldx	#nsd::TR_BGM1

Loop:
	cpx	__channel
	bcs	Loop_End

	lda	(__ptr),y
	iny
	sta	__tmp
;	lda	(__ptr),y
;	ora	__tmp
;	bne	@L		;�|�C���^�[��0��������A�g���b�N�����B
;	iny
;	jmp	@E
;
;@L:
	jsr	_nsd_play
	iny

@E:
	inx
	inx
	cpx	#nsd::TR_BGM1 + nsd::BGM_Track * 2
	bcc	Loop
Loop_End:

	;-----------------------
	;common
	lda	#$08
	sta	__sweep_ch1
	sta	__sweep_ch2

.ifdef	SE
	lda	__Sequence_ptr + 1 + nsd::TR_SE_Pluse1
.ifdef	DPCMBank
	ora	__Sequence_ptr + 0 + nsd::TR_SE_Pluse1
.endif
	bne	@L1
	lda	#$08
.endif
	sta	APU_PULSE1RAMP
@L1:

	lda	__Sequence_ptr + 1 + nsd::TR_SE_Pluse2
.ifdef	DPCMBank
	ora	__Sequence_ptr + 0 + nsd::TR_SE_Pluse2
.endif
	bne	@L2
	lda	#$08
	sta	APU_PULSE2RAMP
@L2:
	lda	#120
	sta	__Tempo

	lda	#$FF
	sta	__Tempo_ctr
	sta	__apu_tri_time		;�O�p�g ����
@L3:

.ifdef	FDS
	lda	#$00
	sta	__fds_sweepbias
.endif

.ifdef	OPLL
	lda	#OPLL_RHYTHM
	sta	OPLL_Resister

	lda	#$00
	sta	__opll_ryhthm
	tay
	sta	OPLL_Data
.endif

.ifdef	N163
	lda	#$E0			;56(0x38) << 2
	sta	__n163_frequency + 0
	sta	__n163_frequency + 1
	sta	__n163_frequency + 2
	sta	__n163_frequency + 3
	sta	__n163_frequency + 4
	sta	__n163_frequency + 5
	sta	__n163_frequency + 6
	sta	__n163_frequency + 7
.endif

	lda	#<~(nsd_flag::BGM + nsd_flag::Disable)
	and	__flag
	sta	__flag			;BGM & Main Enable

	rts
.endproc

;=======================================================================
;	Play
;-----------------------------------------------------------------------
;<<Contents>>
;	Play
;<<Input>>
;	a	__chflag,x	�̒l
;	__tmp	(__ptr),y-1
;	__ptr	pointer
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_play

.ifdef	MASK
	lda	__chflag,x
	and	#$80			;Channnel Mask��Ԃ̕ێ�
	ora	#$01
.else
	lda	#$01			;Release Mode = 1
.endif
	sta	__chflag,x

	lda	__tmp
	ora	(__ptr),y
	bne	@L
	lda	#0			;0��������A���t�|�C���^���O
	sta	__Sequence_ptr,x
	beq	@SET
@L:
	lda	__tmp
	add	__ptr
	sta	__Sequence_ptr,x
	lda	(__ptr),y
.ifdef	DPCMBank
	adc	__tmp + 1
.else
	adc	__ptr + 1
.endif

@SET:
	sta	__Sequence_ptr + 1,x	;__Sequence_ptr = __ptr + (__ptr),y

	lda	#0
	sta	__tai,x
	sta	__Gate,x
	sta	__gate_q,x
	sta	__gate_u,x
	sta	__detune_cent,x
	sta	__detune_fine,x
	sta	__por_target,x
	sta	__trans,x
	sta	__trans_one,x
	sta	__subroutine,x
	sta	__subroutine + 1,x

	sta	__env_frequency,x
	sta	__env_frequency + 1,x
	sta	__env_freq_ptr,x
	sta	__env_note,x
	sta	__env_note + 1,x
	sta	__env_note_ptr,x

	;Ch5(DPCM) ?
	cpx	#nsd::TR_BGM5
	beq	Step5
.ifdef	SE
	cpx	#nsd::TR_SE_Dpcm
	beq	Step5
.endif

	sta	__env_volume,x
	sta	__env_volume + 1,x
	sta	__env_vol_ptr,x

	;Ch3(Tri) ?
	cpx	#nsd::TR_BGM3
	beq	Step3
.ifdef	SE
	cpx	#nsd::TR_SE_Tri
	beq	Step5
.endif

	sta	__env_voice + 1,x
	sta	__env_voi_ptr,x
	lda	#$02
	sta	__env_voice,x
;	lda	#$00
;	sta	__voice,x	;�p�~

Step3:
	lda	#$2F
	sta	__volume,x

Step5:
	lda	#1
	sta	__Length_ctr,x

	lda	#24
	sta	__length,x

	lda	#36
	sta	__octave,x

	lda	#$FF
	sta	__note,x
	lda	#$2
	sta	__gatemode,x

	rts
.endproc

