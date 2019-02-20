;=======================================================================
;
;	NES Sound Driver & library (NSD.lib)	Envelope Controler
;
;-----------------------------------------------------------------------
;
;	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
;	For conditions of distribution and use, see copyright notice
;	  in "nsd.h" or "nsd.inc".
;
;=======================================================================

	.setcpu		"6502"

	.export		nsd_envelop

	.import		_nsd_snd_voice
	.import		_nsd_snd_volume
	.import		_nsd_snd_frequency
	.import		_nsd_mul

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
;	void	nsd_envelop_proc(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	Process of envelop
;<<Input>>
;	__ptr	Pointer of envelop pattern
;	y	Address
;<<Output>>
;	a	Next Address
;	__ptr	Value
;=======================================================================
.macro	ENV	Address, Pointer, Now, Counter, n

	lda	Counter,x
	.if	(n = 0)
		and	#$0F			;
		beq	@Done			;
		lda	Counter,x		;
		sub	#$01			;
	.else
		and	#$F0			;
		beq	@Done			;if(counter != 0){
		lda	Counter,x		;
		sub	#$10			;  counter--
	.endif
	sta	Counter,x		;
	jmp	@Decay			;} else {

@Done:	lda	Address,x
	sta	__ptr			;  __ptr = (table address of envelop)
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif
	ldy	Pointer,x		;  y     = (_envelop pointer)
@Loop:	lda	(__ptr),y		;  do{
	bpl	@Value			;  a = __ptr[y]
	cmp	#$C0			;    if( a >= 0x80 ){
	bcc	@SetDecay		;      if( a >= 0xc0){
	and	#$3F			;        y = a & 0x3F;  //continue
	tay				;      } else {
	jmp	@Loop			;        (decay counter) = a<<4 | (decay counter);

@SetDecay:				;        break;
	.if	(n = 4)
		shl	a, n		;      }
	.else
		and	#$0F
	.endif
	ora	Counter,x		;    } else { 
	sta	Counter,x		;      (now value) = a;
	jmp	@SetY			;      break;
					;    }
@Value:	sta	Now,x			;  } while(1);
@SetY:	iny				;  (_envelop pointer) = y + 1;
	tya				;}
	sta	Pointer,x		;a = (now value)
@Decay:	lda	Now,x			;
@Set:	

.endmacro
;=======================================================================
;	void	nsd_envelop(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	Envelop & LFO main routine
;<<Input>>
;	x	Channel * 2
;<<Output>>
;	nothing
;<<Note>>
;	DPCM track do not call this routine.
;=======================================================================
.proc	nsd_envelop

	stx	__channel

	;=======================================
	;Frequency (note)
Frequency:
	;r-��ԁH
	lda	__chflag,x
	and	#nsd_chflag::KeyOff
	beq	@V0
	;�ŏ��̋x���H
	lda	__note,x
	cmp	#$FF
	bne	@L
@V0:	

;.ifdef	SE
;	cpx	#nsd::TR_SE_Tri
;	bne	@V0S
;	rts
;@V0S:
;.endif

	lda	#0			; if (rest with vol=0){
	jmp	_nsd_snd_volume		;	volume = 0
@L:					; } else {


	;-------------------------------
	;Envelop of Note
	lda	__env_note + 1,x
.ifdef	DPCMBank
	ora	__env_note,x
	beq	@NOENV
	lda	__env_note + 1,x
.else
	beq	@NOENV				;envelop is disable?
.endif
	sta	__ptr + 1
	ENV	__env_note, __env_note_ptr, __env_note_now, __Envelop_F, 4
	cmp	#$40
	bcc	@Sigh
	ora	#$80
@Sigh:	add	__note,x
	jmp	@Note_Exit
@NOENV:
	lda	__note,x
@Note_Exit:

	;-------------------------------
	; __tmp = (__note,x + __trans,x) << 4;
	sta	__tmp + 1
	shl	a, 4
	sta	__tmp
	lda	__tmp + 1
	shr	a, 4
	sta	__tmp + 1

	;-------------------------------
	;Por
Portamento:
	lda	__por_target,x
	jeq	F_Env			;�|���^�����g���H

	lda	__por_depth,x
	beq	Por_S			;�^�[�Q�b�g�ɓ��B�H

	dec	__por_ctr,x		;MML�R���p�C���ł�decay�� +1 ���鎖�B
	bne	Por_S

	lda	__por_rate,x
	sta	__por_ctr,x

	ldy	#$00
	lda	__por_depth,x
	bpl	@PL
	dey
@PL:	add	__por_now + 0,x
	sta	__por_now + 0,x
	tya
	adc	__por_now + 1,x
	sta	__por_now + 1,x		;__por_now += __por_depth

	lda	__por_target,x
	shl	a, 4
	sta	__ptr
	lda	__por_target,x
	shr	a, 4
	cpy	#00
	beq	@L
	ora	#$F0			;�Z�p�V�t�g�I�Ȃ���
	tay				;__ptr = __por_target << 4
	bne	Por_MI			;�K�� �� 0
@L:	tay

Por_PL:	cmp	__por_now + 1,x		;if ((__por_target<<4 - __por_now) < 0) then 
	bcc	Por_O			;
	beq	Por_P2			;
	bcs	Por_S			;
Por_P2:	lda	__ptr
	cmp	__por_now + 0,x
	bcc	Por_O
	bcs	Por_S

Por_MI:	cmp	__por_now + 1,x	
	bcc	Por_S			;
	beq	Por_M2			;
	bcs	Por_O			;
Por_M2:	lda	__ptr
	cmp	__por_now + 0,x
	bcc	Por_S
;	bcs	Por_O

Por_O:	lda	__ptr
	sta	__por_now + 0,x
	tya
	sta	__por_now + 1,x
	lda	#$0
	sta	__por_rate,x
	sta	__por_depth,x

Por_S:	lda	__por_now,x		;�����Ƀ|���^�����g�l���Z
	add	__tmp
	sta	__tmp
	lda	__por_now + 1,x
	adc	__tmp + 1
	sta	__tmp + 1
Por_E:

	;-------------------------------
	;Envelop of Frequency
F_Env:	
	lda	__env_frequency + 1,x
.ifdef	DPCMBank
	ora	__env_frequency,x
	beq	@Freq_Exit
	lda	__env_frequency + 1,x
.else
	beq	@Freq_Exit
.endif
	sta	__ptr + 1
	ENV	__env_frequency, __env_freq_ptr, __env_freq_now, __Envelop_F, 0
	ldy	#$00
	cmp	#$40
	bcc	@L
	dey
	ora	#$80
@L:	add	__tmp
	sta	__tmp
	tya
	adc	__tmp+1
	sta	__tmp+1
@Freq_Exit:



	;-------------------------------
	;Detune of cent unit
Detune:	
	ldy	#$00
	lda	__detune_cent,x
	bpl	@L
	dey				; ay = __detune_cent (sign expand)
@L:	add	__tmp
	sta	__tmp
	tya
	adc	__tmp + 1		;__tmp += (signed int)__detune_cent

	;-----------------------
	;Setting device (APU)
	tax				;
	lda	__tmp			;ax <- frequency�@�i����ōςނ�ˁB�j
	jsr	_nsd_snd_frequency	;nsd_snd_frequency(ax);
	ldx	__channel		;�`�����l���̕��A�i���K�v�j



;	----------------------------------------
;	mode	env	voi	vol
;	0	-	-	v0	
;	1	-	rel	rel	Mode1
;	2	off	-	rel	Mode2
;	2	on	env	vol*env	Envelope
;	3	off	-	vol	Mode3
;	3	on	env	vol*env	Envelope
;	----------------------------------------

	lda	__chflag,x		;gatemode �� 1 �̏ꍇ�AMode1�ցB
	and	#$02			;���F�ݒ�́Ansd_key_on()�֐��y��
	bne	V_Envelope		;nsd_key_off()�֐��ł��B

Mode1:	;-----------------------
	;Release Mode	(Mode 1)

	cpx	#nsd::TR_BGM3
	beq	Exit
.ifdef	SE
	cpx	#nsd::TR_SE_Tri
	beq	Exit
.endif

	lda	__volume,x

.ifdef	VRC6
	cpx	#nsd::TR_VRC6 + 4	;VRC6 SAW ?
	bne	@VRC6
	and	#$F0
	jmp	@EX
@VRC6:
.endif

.ifdef	FDS
	cpx	#nsd::TR_FDS
	bne	@FDS
	and	#$F0
	jmp	@EX
@FDS:
.endif
	lsr	a
	lsr	a
@EX:	lsr	a
	lsr	a
	jmp	_nsd_snd_volume

Exit:	rts



	;-----------------------
	;Envelope mode	(Mode2&3)
V_Envelope:

	;-------------------------------
	;Envelop of Voice
Voice:
	lda	__gatemode,x		;
	and	#nsd_mode::voice	;Envelope�L���H
	beq	Voice_Exit		;Ch3 �͕K��Low �ɂȂ��Ă���B
;	cpx	#nsd::TR_BGM3		;�̂ŁA�R�����g�A�E�g
;	beq	Voice_Exit		;
;.ifdef	SE
;	cpx	#nsd::TR_SE_Tri
;	beq	Voice_Exit
;.endif

@Envelop:
	lda	__Envelop_V,x
	and	#$F0			;
	beq	@Done			;if(counter != 0){
	lda	__Envelop_V,x		;
	sub	#$10			;  counter--
	sta	__Envelop_V,x		;
	jmp	Voice_Exit		;} else {

@Done:	
	lda	__env_voice + 1,x
	sta	__ptr + 1
	lda	__env_voice,x
	sta	__ptr			;  __ptr = (table address of envelop)
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif
	ldy	__env_voi_ptr,x		;  y     = (_envelop pointer)
@Loop:	lda	(__ptr),y		;  do{
	bpl	@Value			;  a = __ptr[y]
	cmp	#$C0			;    if( a >= 0x80 ){
	bcc	@SetDecay		;      if( a >= 0xc0){
	and	#$3F			;        y = a & 0x3F;  //continue
	tay				;      } else {
	jmp	@Loop			;        (decay counter) = a<<4 | (decay counter);

@SetDecay:				;
	shl	a, 4			;
	ora	__Envelop_V,x		;
	sta	__Envelop_V,x		;
	iny				;
	tya				;
	sta	__env_voi_ptr,x		;
	jmp	Voice_Exit

@Value:
	pha
	iny
	tya
	sta	__env_voi_ptr,x
	pla
	;-----------------------
	;Setting device (APU)
Set_Voice:
	jsr	_nsd_snd_voice		;nsd_snd_voice(a);
Voice_Exit:

	;-------------------------------
	;Envelop of Volume
Volume:
	lda	__env_volume + 1,x
.ifdef	DPCMBank
	ora	__env_volume,x
	beq	@No_Envelop
	lda	__env_volume + 1,x
.else
	beq	@No_Envelop
.endif

@Envelop:
	sta	__ptr + 1
	ENV	__env_volume, __env_vol_ptr, __env_vol_now, __Envelop_V, 0

	cpx	#nsd::TR_BGM3		;
	beq	@S			;ch3 �̓��j�A�J�E���^�Ȃ̂ŁAv �R�}���h�l�Ƃ̏�Z�͂��Ȃ��B
.ifdef	SE
	cpx	#nsd::TR_SE_Tri
	beq	@S
.endif
	sta	__tmp
	lda	__volume,x
	ldx	__tmp
	jsr	_nsd_mul
	ldx	__channel
@S:	jmp	_nsd_snd_volume		;nsd_snd_volume(a);


	;-----------------------
	;Envelope �������̏���
@No_Envelop:
	cpx	#nsd::TR_BGM3
	beq	Exit2
.ifdef	SE
	cpx	#nsd::TR_SE_Tri
	beq	Exit2
.endif

	lda	__chflag,x
	and	#nsd_chflag::KeyOff
	cmp	#$02
	bne	Mode3

Mode2:	;-----------------------
	;Key Off��
.ifdef	VRC7
	;VRC7�́Amode 2�̎��̓����[�X�������Ȃ��B
	cpx	#nsd::TR_VRC7
	bcc	@VRC7L
	cpx	#nsd::TR_VRC7 + 6*2
	bcs	@VRC7L
	lda	__volume,x
	jmp	_nsd_snd_volume
@VRC7L:
.endif

.ifdef	OPLL
	;OPLL�́Amode 2�̎��̓����[�X�������Ȃ��B
	cpx	#nsd::TR_OPLL
	bcc	@OPLLL
	cpx	#nsd::TR_OPLL + 9*2
	bcs	@OPLLL
	lda	__volume,x
	jmp	_nsd_snd_volume
@OPLLL:
.endif
	jmp	Mode1

Mode3:	;-----------------------
	;Key On��
	lda	__volume,x

.ifdef	VRC6
	cpx	#nsd::TR_VRC6 + 4
	bne	@VRC6V
	shl	a, 2
@VRC6V:
.endif

.ifdef	FDS
	cpx	#nsd::TR_FDS
	bne	@FDSV
	shl	a, 2
@FDSV:
.endif
	jmp	_nsd_snd_volume

Exit2:	rts

.endproc
