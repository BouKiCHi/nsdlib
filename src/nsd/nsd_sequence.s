;=======================================================================
;
;	NES Sound Driver & library (NSD.lib)	Track Sequencer
;
;-----------------------------------------------------------------------
;
;	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
;	For conditions of distribution and use, see copyright notice
;	  in "nsd.h" or "nsd.inc".
;
;=======================================================================

	.setcpu		"6502"

	.export		nsd_sequence

	.import		_nsd_snd_keyon
	.import		_nsd_snd_keyoff
	.import		_nsd_snd_sweep
	.import		_nsd_snd_voice
	.import		_nsd_play_se
	.import		nsd_work
	.importzp	nsd_work_zp

.if	.defined(VRC7) || .defined(OPLL)
	.import		_Wait42
.endif

.ifdef	DPCMBank
	.export		_nsd_ptr_bank
.endif

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


;=======================================================================
;		nsd_keyon
;-----------------------------------------------------------------------
;<<Contents>>
;	Key On
;<<Input>>
;	x	Channel * 2
;<<Output>>
;	nothing
;=======================================================================
.macro	_NSD_keyon

	.local	exit

	;Portamento
	lda	__por_depth,x
	bne	@L		;�|���^�����g���I�����Ă�����A����������B
	lda	#0
	sta	__por_target,x
@L:

	;tai check
	lda	__tai,x
	and	#$02
	bne	exit		;�O��̉������^�C�E�X���[�������I��

	;Hardware key on
	jsr	_nsd_snd_keyon

	;Software key on
	lda	__chflag,x
	and	#<~nsd_chflag::KeyOff
	ora	#nsd_chflag::KeyOff
	sta	__chflag,x

	;-----------------------
	;�ȍ~�́A��PCM�ł͕s�v
	cpx	#nsd::TR_BGM5
	beq	exit

	;���g���ݒ��K���ĂԂ悤�ɁB
	lda	#$FF
	sta	__frequency,x
	sta	__frequency + 1,x

	;Envelop Key on
	lda	#$00
	sta	__Envelop_F,x
	lda	#$01
	sta	__env_freq_ptr,x
	sta	__env_note_ptr,x

	lda	#$00
	sta	__Envelop_V,x
	lda	#$01
	sta	__env_vol_ptr,x
	cpx	#nsd::TR_BGM3		;-----------------------
	beq	exit			;�ȍ~�́A�O�p�g�ł͕s�v
	sta	__env_voi_ptr,x

	;���F�G���x���[�v
	lda	__gatemode,x		;�������@�œK���@������
	and	#nsd_mode::voice	;���ʃG���x���[�v��������������
	bne	@L2			;������KeyOn���̉��F�ɂ���B
	lda	__env_voice,x		;
	jmp	_nsd_snd_voice		;	�����́A JMP �ŗǂ��B
					;	macro�̌Ăяo����A"rts"������B
@L2:

exit:

.endmacro

;=======================================================================
;		nsd_keyoff
;-----------------------------------------------------------------------
;<<Contents>>
;	Key Off
;<<Input>>
;	x	Channel * 2
;<<Output>>
;	nothing
;=======================================================================
.macro	_NSD_keyoff

	.local	Freq_End
	.local	Note_End
	.local	Voice_End
	.local	Volume_End
	.local	exit

	;Key On���H
	lda	__chflag,x
	and	#nsd_chflag::KeyOff
	cmp	#nsd_chflag::KeyOff
	jne	exit			;Key On(=3)�̎��̂݁A�����B

	;tai check
	lda	__tai,x
	and	#$01
	jne	exit			;����̉������^�C�E�X���[�������I��

	;Software key Off
	lda	__gatemode,x
	and	#nsd_mode::gatemode
	sta	__tmp
	lda	__chflag,x
	and	#<~nsd_chflag::KeyOff
	ora	__tmp
	sta	__chflag,x

	;Hardware key off
	jsr	_nsd_snd_keyoff

	;-----------------------
	;�ȍ~�́A��PCM�ł͕s�v
	cpx	#nsd::TR_BGM5
	jeq	exit

	;---------------
	;Portamento
	lda	#$00
	tay
	sta	__por_depth,x		;�|���^�����g���I��������B

	;---------------
	;Frequency Envelop keyoff
	lda	__env_frequency + 1,x
.ifdef	DPCMBank
	ora	__env_frequency,x
	beq	Freq_End
	lda	__env_frequency + 1,x
.else
	beq	Freq_End
.endif
	sta	__ptr + 1
	lda	__env_frequency,x
	sta	__ptr
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif
	lda	(__ptr),y
	beq	Freq_End
	sta	__env_freq_ptr,x
	lda	__Envelop_F,x
	and	#$F0
	sta	__Envelop_F,x
Freq_End:

	;---------------
	;Note Envelop keyoff
	lda	__env_note + 1,x
.ifdef	DPCMBank
	ora	__env_note,x
	beq	Note_End
	lda	__env_note + 1,x
.else
	beq	Note_End
.endif
	sta	__ptr + 1
	lda	__env_note,x
	sta	__ptr
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif
	lda	(__ptr),y
	beq	Note_End
	sta	__env_note_ptr,x
	lda	__Envelop_F,x
	and	#$0F
	sta	__Envelop_F,x
Note_End:

	;---------------
	;Volume Envelop keyoff
	lda	__env_volume + 1,x
;	beq	Volume_End		;��{�I�ɂ͗L�����낤�̂ŁA�R�����g�A�E�g���Ă����B
	sta	__ptr + 1
	lda	__env_volume,x
	sta	__ptr
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif
	lda	(__ptr),y
	beq	Volume_End
	sta	__env_vol_ptr,x
	lda	__Envelop_V,x
	and	#$F0
	sta	__Envelop_V,x
Volume_End:

	;-----------------------
	;�ȍ~�́A�O�p�g�ł͕s�v
	cpx	#nsd::TR_BGM3
	beq	exit

	;---------------
	;Voice Envelop keyoff
	lda	__gatemode,x
	and	#nsd_mode::voice
	beq	Voice_End
	lda	__env_voice + 1,x
	sta	__ptr + 1
	lda	__env_voice,x
	sta	__ptr
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif
	lda	(__ptr),y
	beq	Voice_End
	sta	__env_voi_ptr,x
	lda	__Envelop_V,x
	and	#$0F
	sta	__Envelop_V,x
Voice_End:

	;---------------
	;���F�G���x���[�v
	lda	__chflag,x
	and	#$01			;�������@�œK���@������
	beq	@VoiceE			;gatemode = 1 ��������A
	lda	__gatemode,x		;������KeyOff���̉��F�ɂ���B
	shr	a, 4			; a = release voice
	jsr	_nsd_snd_voice		;
@VoiceE:

exit:

.endmacro

;=======================================================================
;		nsd_load_sequence;
;-----------------------------------------------------------------------
;<<Contents>>
;	read sequence data
;<<Input>>
;	x	Channel * 2
;	nds_work.Sequence_ptr
;<<Output>>
;	a	sequence data
;=======================================================================
.proc	nsd_load_sequence
.ifdef	DPCMBank
	tya
	pha

	ldy	__Sequence_ptr,x
	sty	__ptr
	ldy	__Sequence_ptr + 1,x
	sty	__ptr + 1

	jsr	_nsd_ptr_bank
	ldy	#0
	lda	(__ptr),y

	sta	__ptr
	pla
	tay
	lda	__ptr

.else
	lda	(__Sequence_ptr,x)	;[6]	6*4+2 = 26clock (52clock)
.endif
	inc	__Sequence_ptr,x	;[6]
	bne	exit			;[2]
	inc	__Sequence_ptr + 1,x
exit:	rts				;[6]
.endproc
;=======================================================================
;		nsd_load_ptr;
;-----------------------------------------------------------------------
;<<Contents>>
;	read sequence data (pointer)
;<<Input>>
;	x	Channel * 2
;	nds_work.Sequence_ptr
;<<Output>>
;	__ptr	now pointer
;	__tmp	sequence data
;	a	__tmp + 1
;=======================================================================
.proc	nsd_load_ptr

	lda	__Sequence_ptr,x	;
.ifdef	DPCMBank
	pha
.endif
	sta	__ptr			;
	add	#2			;[3]
	sta	__Sequence_ptr,x	;[4]

.ifdef	DPCMBank
	lda	__Sequence_ptr + 1,x	;
	sta	__ptr + 1		;	__ptr = __Sequence_ptr
	pha
	bcc	@l			;[2]9
	tay
.else
	ldy	__Sequence_ptr + 1,x	;
	sty	__ptr + 1		;	__ptr = __Sequence_ptr
	bcc	@l			;[2]9
.endif
	iny				;
	sty	__Sequence_ptr + 1,x	;
@l:

.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif
	ldy	#0
	lda	(__ptr),y
	sta	__tmp
	iny
	lda	(__ptr),y
	sta	__tmp + 1

.ifdef	DPCMBank
	pla
	sta	__ptr + 1
	pla
	sta	__ptr
.endif

@exit:

	rts
.endproc

;=======================================================================
;		_nsd_ptr_bank;
;-----------------------------------------------------------------------
;<<Contents>>
;	Bank Change and Calc. pointer
;<<Input>>
;	__ptr	16bit Pointer
;<<Output>>
;	__ptr	Real Address (8000 - BFFF) 100x xxxx xxxx xxxx
;=======================================================================
.ifdef	DPCMBank
.proc	_nsd_ptr_bank

	pha
	tya
	pha

	lda	__ptr + 1
	pha
	and	#$0F
	ora	#$80
	sta	__ptr + 1
	pla

	shr	a, 4
	add	#$03		;�ŏ� 3 bank �́A�����h���C�o�{��
	sta	$5FF8
	add	#1
	sta	$5FF9

	pla
	tay
	pla

	rts
.endproc
.endif
;=======================================================================
;	void	nsd_sequence(void);
;-----------------------------------------------------------------------
;<<Contents>>
;	Play sequence and control gate time
;<<Input>>
;	x	Channel * 2
;<<Output>>
;	nothing
;=======================================================================
.proc	nsd_sequence

.rodata

length:	.byte	96
	.byte	72
	.byte	48
	.byte	36
	.byte	32
	.byte	24
	.byte	18
	.byte	16
	.byte	12
	.byte	9
	.byte	8
	.byte	6
	.byte	4
	.byte	3
	.byte	2
	.byte	1

opaddr:	.addr	nsd_op00
	.addr	nsd_op01
	.addr	nsd_op02
	.addr	nsd_op03
	.addr	nsd_op04
	.addr	nsd_op05
	.addr	nsd_op06
	.addr	nsd_op07
	.addr	nsd_op08
	.addr	nsd_op09
	.addr	nsd_op0A
	.addr	nsd_op0B
	.addr	nsd_op0C
	.addr	nsd_op0D
	.addr	nsd_op0E
	.addr	nsd_op0F
	.addr	nsd_op10
	.addr	nsd_op11
	.addr	nsd_op12
	.addr	nsd_op13
	.addr	nsd_op14
	.addr	nsd_op15
	.addr	nsd_op16
	.addr	nsd_op17
	.addr	nsd_op18
	.addr	nsd_op19
	.addr	nsd_op1A
	.addr	nsd_op1B
	.addr	nsd_op1C
	.addr	nsd_op1D
	.addr	nsd_op1E
	.addr	nsd_op1F
	.addr	nsd_op20
	.addr	nsd_op21
	.addr	nsd_op22
	.addr	nsd_op23
	.addr	nsd_op24
	.addr	nsd_op25
	.addr	nsd_op26
	.addr	nsd_op27
	.addr	nsd_op28
	.addr	nsd_op29
	.addr	nsd_op2A
	.addr	nsd_op2B
	.addr	nsd_op2C
	.addr	nsd_op2D
	.addr	nsd_op2E
	.addr	nsd_op2F
	.addr	nsd_op30
	.addr	nsd_op31
	.addr	nsd_op32
	.addr	nsd_op33
	.addr	nsd_op34
	.addr	nsd_op35
	.addr	nsd_op36
	.addr	nsd_op37
	.addr	nsd_op38
	.addr	nsd_op39
	.addr	nsd_op3A
	.addr	nsd_op3B
	.addr	nsd_op3C
	.addr	nsd_op3D
	.addr	nsd_op3E
	.addr	nsd_op3F

.code

	;-------------------------------
	;now play?	(check: Sequence_ptr == 0 ?)	** upper 1 byte only **
	lda	__Sequence_ptr + 1,x
.ifdef	DPCMBank
	ora	__Sequence_ptr,x
.endif
	bne	Done
	rts

Done:
	stx	__channel		;channel <- x

	;-------------------------------
	;Gatetime control

	;length == gatetime q ?
	dec	__Length_ctr,x		;length--
	lda	__Gate,x
	cmp	__Length_ctr,x
	jne	GateTime_Exit		; if(__Length_ctr <= __Gate){
	_NSD_keyoff			; 	nsd_keyoff();
GateTime_Exit:				; }
	lda	__Length_ctr,x
	beq	Sequence		; if(__Length_ctr != 0){ return(); };
Exit:
	rts

;=======================================================================
;		Sequence
;=======================================================================

Sequence:
	jsr	nsd_load_sequence	;[6]

	tay
	asl	a
	bcs	Note			;[2]	a >= 80 ?

;=======================================================================
;		Control
;=======================================================================

	;-----------------------
	;op-code = 0x00 - 0x7F
Control:
;	cmp	#$80			;[2]
;	bcs	Short_Control		;[2]
	bmi	Short_Control

	;---------------
	;op-code = 0x00 - 0x3F
;	asl				;[2]
	tay				;[2]	x <- a * 2
	lda	opaddr,y		;[4]
	sta	__ptr			;[3]
	lda	opaddr + 1,y		;[4]
	sta	__ptr + 1		;[3]
	jmp	(__ptr)			;[5]	jump for each op-code

	;---------------
	;op-code = 0x40 - 0x7F
Short_Control:
	asl	a
	;---------------
	;op-code = 0x40 - 0x7F
	;---------------
	;0x40 - 0x4F
op40:	
	asl	a
	bcs	op60
	bmi	op50
	;Set default length
	tya
	and	#$0F
	tay
	lda	length,y
	sta	__length,x		;__length = length[a & 0x0F];
	jmp	Sequence
	;---------------
	;0x50 - 0x5F
op50:	
	;Set gate time (1)
	tya
	and	#$0F
	sta	__gate_q,x		;__gate_q = a & 0x0F;
	jmp	Sequence
	;---------------
	;0x60 - 0x6F
op60:	
	bmi	op70
	;Set volume
	tya
	and	#$0F
Set_Volume:
	sta	__tmp
	lda	__volume,x
	and	#$F0
	ora	__tmp
	sta	__volume,x		;__volume = (__volume & 0xF0) | (a & 0x0F);
	jmp	Sequence
	;---------------
	;0x70 - 0x7F
op70:	;Ser release volume
	asl	a
	sta	__tmp
	lda	__volume,x
	and	#$0F
	ora	__tmp
	sta	__volume,x		;__volume = (__volume & 0x0F) | (a << 4);
	jmp	Sequence

;=======================================================================
;		Note
;=======================================================================

	;-----------------------
	;op-code = 0x80 - 0xFF
Note:

	;-------
	;bit 6 check (Slur)
Chk_Slur:
	asl	a
	rol	__tai,x

	;-------
	;bit 5 check (Length )
Chk_Length:
	asl	a
	sta	__tmp
	bcs	@L
	lda	__length,x
	bne	@E			;��{0����Ȃ��B
@L:	jsr	nsd_load_sequence
@E:	sta	__Length_ctr,x

	;-------
	;bit 4 check (Gate Time)
Chk_GateTime:
	asl	__tmp
	bcc	@L

	jsr	nsd_load_sequence
	cmp	#0
	beq	GateSet
	sta	__tmp
	lda	__Length_ctr,x
	sub	__tmp			; a = __Length_ctr - __Gate;
	bcs	GateSet			; if(a < 0){
	lda	#$0			;    a = 0x0; //no gate
	beq	GateSet			; }	// relative jump because "0"
@L:
	lda	__gate_u,x		;if (__gate_u,x == 0) then @q
	beq	@q			;
	lda	__Length_ctr,x
	sub	__gate_u,x		; a = __Length_ctr - __gate_u,x	(gate timing)
	bcc	@q			; if (a < 0) then @q
	cmp	__gate_q,x		;
	bcs	GateSet			; if( a < __gate_q){
@q:	lda	__gate_q,x		;    a = __gate_q;
	cmp	__Length_ctr,x		; }
	bcc	GateSet
	lda	#0
GateSet:
	sta	__Gate,x

Calc_Note_Number:
	tya
	and	#$0F
	cmp	#12
	bcc	NoteSet
	beq	@Exit

	;0x0D-0x0F	[Rest mode 0-2]
@Rest:	sbc	#$0D	;cy �� `H'
	sta	__tmp
	lda	__tai,x
	and	#$02
	bne	@Exit		;If tai then exit
	lda	__chflag,x
	and	#<~nsd_chflag::KeyOff
	ora	__tmp
	sta	__chflag,x

	and	#$01			;�������@�œK���@������
	beq	@Exit			;gatemode = 1 ��������A
	lda	__gatemode,x		;������KeyOff���̉��F�ɂ���B
	shr	a, 4			; a = release voice
	jmp	_nsd_snd_voice		;
@Exit:
	;0x0C		[Rest mode 3]
	rts

	;0x00-0x0B	[Note]
NoteSet:
	add	__octave,x
	add	__trans,x
	add	__trans_one,x
	sta	__note,x
	lda	#0
	sta	__trans_one,x	;0 reset
	_NSD_keyon
	rts

;=======================================================================
;		opcode	0x00:	End of Track / End of Subroutine
;-----------------------------------------------------------------------
nsd_op00:
	lda	__subroutine + 1,x
	sta	__Sequence_ptr + 1,x
.ifdef	DPCMBank
	ora	__subroutine,x
.endif
	beq	@L	;if (a == 0)
	lda	__subroutine,x
	sta	__Sequence_ptr,x	;�߂�Ƃ��͑��
	lda	#0
	sta	__subroutine,x
	sta	__subroutine + 1,x
	jmp	Sequence
@L:
	sta	__Sequence_ptr,x	;__Sequence_ptr = 0

	lda	__chflag,x
	and	#<~nsd_chflag::KeyOff
	sta	__chflag,x
;
;	Sweep�̏�Ԃ����ɖ߂��B
;

.ifdef	SE
@SE1:	
	cpx	#nsd::TR_SE_Pluse1
	bne	@SE1_E
	lda	__sweep_ch1
	jsr	_nsd_snd_sweep
	;���g���ݒ��K���ĂԂ悤�ɁB
	lda	#$FF
	sta	__frequency + nsd::TR_BGM1
	sta	__frequency + nsd::TR_BGM1 + 1
	sta	__apu_frequency1
	jmp	@Exit
@SE1_E:
.endif

@SE2:	
	cpx	#nsd::TR_SE_Pluse2
	bne	@SE2_E
	lda	__sweep_ch2
	jsr	_nsd_snd_sweep
	;���g���ݒ��K���ĂԂ悤�ɁB
	lda	#$FF
	sta	__frequency + nsd::TR_BGM2
	sta	__frequency + nsd::TR_BGM2 + 1
	sta	__apu_frequency2
	jmp	@Exit
@SE2_E:

.ifdef	SE
@SE3:
	cpx	#nsd::TR_SE_Tri
	bne	@SE3_E

	;���g���ݒ��K���ĂԂ悤�ɁB
	lda	#$FF
	sta	__frequency + nsd::TR_BGM3
	sta	__frequency + nsd::TR_BGM3 + 1
	sta	__apu_frequency3

	;Hardware key on for ch3
	lda	__env_volume + nsd::TR_BGM3 + 1
.ifdef	DPCMBank
	ora	__env_volume + nsd::TR_BGM3
.endif
	;���ʃG���x���[�v���L���ȏꍇ�A�G���x���[�v�����ɔC��
	bne	@Exit

	lda	__gatemode + nsd::TR_BGM3
	ora	#nsd_mode::RetSE
	sta	__gatemode + nsd::TR_BGM3

	jmp	@Exit

@SE3_E:
.endif

@SE4:
	cpx	#nsd::TR_SE_Noise
	bne	@SE4_E
	;���g���ݒ��K���ĂԂ悤�ɁB
	lda	#$FF
	sta	__frequency + nsd::TR_BGM4
	sta	__frequency + nsd::TR_BGM4 + 1
@SE4_E:


.ifdef	SE
@SE5:
	cpx	#nsd::TR_SE_Dpcm
	bne	@SE5_E
@SE5_E:
.endif


@Exit:
	rts

;=======================================================================
;		opcode	0x02:	Sub-routine call
;-----------------------------------------------------------------------
nsd_op02:
	lda	__Sequence_ptr,x
	add	#2
	sta	__subroutine,x
	lda	__Sequence_ptr + 1,x
	adc	#0
	sta	__subroutine + 1,x		;2byte��̃|�C���^���L������B

;=======================================================================
;		opcode	0x01:	Jump (End of Track with loop) 
;-----------------------------------------------------------------------
nsd_op01:
Jump:
	lda	__Sequence_ptr,x	;
	sta	__ptr			;
	ldy	__Sequence_ptr + 1,x	;
	sty	__ptr + 1		;	__ptr = __Sequence_ptr

.ifdef	DPCMBank
	sta	__tmp
	sty	__tmp + 1
	jsr	_nsd_ptr_bank
.endif

	ldy	#0			;[2]11
.ifdef	DPCMBank
	lda	__tmp
.endif
	add	(__ptr),y
	sta	__Sequence_ptr,x

	iny
.ifdef	DPCMBank
	lda	__tmp + 1
.else
	lda	__ptr + 1
.endif
	adc	(__ptr),y
	sta	__Sequence_ptr + 1,x	;__Sequence_ptr = __ptr + __tmp

	jmp	Sequence

;=======================================================================
;		opcode	0x03:	Repeat start.
;-----------------------------------------------------------------------
nsd_op03:
	jsr	nsd_load_sequence
	sta	__repeat_ctr,x
	jmp	Sequence

;=======================================================================
;		opcode	0x04:	Repeat break.
;-----------------------------------------------------------------------
nsd_op04:
	lda	__repeat_ctr,x
	cmp	#1
	beq	Jump
	jsr	nsd_load_ptr
	jmp	Sequence

;=======================================================================
;		opcode	0x05:	Repeat end.
;-----------------------------------------------------------------------
nsd_op05:
	dec	__repeat_ctr,x
	bne	Jump
	jsr	nsd_load_ptr
	jmp	Sequence

;=======================================================================
;		opcode	0x06:	Call SE
;-----------------------------------------------------------------------
nsd_op06:
	jsr	nsd_load_ptr

	lda	__ptr
	add	__tmp
	tay
	lda	__ptr + 1
	adc	__tmp + 1
	tax
	tya
	jsr	_nsd_play_se

	ldx	__channel

	jmp	Sequence

;=======================================================================
;		opcode	0x18:	Repeat start.
;-----------------------------------------------------------------------
nsd_op18:
	lda	#0			;counter <= 0
	sta	__repeat_ctr2,x
	jmp	Sequence

;=======================================================================
;		opcode	0x19:	repeat 1 time point
;-----------------------------------------------------------------------
nsd_op19:
	lda	__repeat_ctr2,x
	beq	@exit			;counter == 0 �������牽�����Ȃ��B
	lda	__repeat2,x
	sta	__Sequence_ptr,x
	lda	__repeat2 + 1,x
	sta	__Sequence_ptr + 1,x	;����ȊO�́A :| �̎��ɃW�����v
@exit:
	jmp	Sequence

;=======================================================================
;		opcode	0x1A:	Repeat end.
;-----------------------------------------------------------------------
nsd_op1A:
	inc	__repeat_ctr2,x		;counter ++

	lda	__Sequence_ptr,x
	add	#2
	sta	__repeat2,x
	lda	__Sequence_ptr + 1,x
	adc	#0
	sta	__repeat2 + 1,x		;2byte��̃|�C���^���L������B

	jmp	Jump			;���΃W�����v��

;=======================================================================
;		opcode	0x07:	Write data to memory.
;-----------------------------------------------------------------------
nsd_op07:
	jsr	nsd_load_sequence
	sta	__tmp
	jsr	nsd_load_sequence
	sta	__tmp + 1
	jsr	nsd_load_sequence
	ldy	#0
	sta	(__tmp),y
	jmp	Sequence

;=======================================================================
;		opcode	0x08:	Tempo [BPM]
;-----------------------------------------------------------------------
nsd_op08:
	jsr	nsd_load_sequence

	;---------------------
	;SE���`�F�b�N
	cpx	#nsd::TR_SE
	bcs	Set_Tempo_SE

Set_Tempo:
	sta	__Tempo
Set_Tempo_SE:
	jmp	Sequence

;=======================================================================
;		opcode	0x0C:	Relative Tempo [BPM]
;-----------------------------------------------------------------------
nsd_op0C:
	jsr	nsd_load_sequence

	;---------------------
	;SE���`�F�b�N
	cpx	#nsd::TR_SE
	bcs	Set_Tempo_SE

	add	__Tempo
	jmp	Set_Tempo

;=======================================================================
;		opcode	0x09:	Note length of �ȗ��� [Ticks] 
;-----------------------------------------------------------------------
nsd_op09:
	jsr	nsd_load_sequence
	sta	__length,x
	jmp	Sequence

;=======================================================================
;		opcode	0x0A:	Gate time (key-off timing from last) 
;-----------------------------------------------------------------------
nsd_op0A:
	jsr	nsd_load_sequence
	sta	__gate_q,x
	jmp	Sequence

;=======================================================================
;		opcode	0x0B:	Gate time (length of ����) 
;-----------------------------------------------------------------------
nsd_op0B:
	jsr	nsd_load_sequence
	sta	__gate_u,x
	jmp	Sequence

;=======================================================================
;		opcode	0x0D:	gate mode	(vol = 0)
;-----------------------------------------------------------------------
nsd_op0D:
	lda	__gatemode,x
	and	#<~nsd_mode::gatemode
;	ora	#$00
	sta	__gatemode,x
	jmp	Sequence

;=======================================================================
;		opcode	0x0E:	gate mode
;-----------------------------------------------------------------------
nsd_op0E:
	lda	__gatemode,x
	and	#<~nsd_mode::gatemode
	ora	#$01
	sta	__gatemode,x
	jmp	Sequence

;=======================================================================
;		opcode	0x0F:	gate mode	(release)
;-----------------------------------------------------------------------
nsd_op0F:
	lda	__gatemode,x
	and	#<~nsd_mode::gatemode
	ora	#$02
	sta	__gatemode,x
	jmp	Sequence

;=======================================================================
;		opcode	0x10:	Voice envelop. 
;-----------------------------------------------------------------------
nsd_op10:
	jsr	nsd_load_ptr

	cpx	#nsd::TR_BGM3
	beq	@Exit
	cpx	#nsd::TR_BGM5
	beq	@Exit

	lda	__ptr
	add	__tmp
	sta	__env_voice,x
	lda	__ptr + 1
	adc	__tmp + 1
@Zero:	sta	__env_voice + 1,x	;__env_voice = __ptr + __tmp

	lda	#$01
	sta	__env_voi_ptr,x

	lda	__gatemode,x
	ora	#nsd_mode::voice	;Voice Envelope �L��
	sta	__gatemode,x
@Exit:
	jmp	Sequence

;=======================================================================
;		opcode	0x11:	Volume envelop.
;-----------------------------------------------------------------------
nsd_op11:
	jsr	nsd_load_ptr

	cpx	#nsd::TR_BGM5
	beq	@Exit

	lda	__tmp + 1
	ora	__tmp
.ifdef	DPCMBank
	bne	@L
	sta	__env_volume,x
.endif
	beq	@Zero
@L:
	lda	__ptr
	add	__tmp
	sta	__env_volume,x
	lda	__ptr + 1
	adc	__tmp + 1
@Zero:	sta	__env_volume + 1,x

	lda	#$01
	sta	__env_vol_ptr,x
@Exit:
	jmp	Sequence

;=======================================================================
;		opcode	0x12:	Frequency envelop. 
;-----------------------------------------------------------------------
nsd_op12:
	jsr	nsd_load_ptr

	cpx	#nsd::TR_BGM5
	beq	@Exit

	lda	__tmp + 1
	ora	__tmp
.ifdef	DPCMBank
	bne	@L
	sta	__env_frequency,x
.endif
	beq	@Zero
@L:
	lda	__ptr
	add	__tmp
	sta	__env_frequency,x
	lda	__ptr + 1
	adc	__tmp + 1
@Zero:	sta	__env_frequency + 1,x

	lda	#$01
	sta	__env_freq_ptr,x
@Exit:
	jmp	Sequence

;=======================================================================
;		opcode	0x13:	Note envelop. 
;-----------------------------------------------------------------------
nsd_op13:
	jsr	nsd_load_ptr

	cpx	#nsd::TR_BGM5
	beq	@Exit

	lda	__tmp + 1
	ora	__tmp
.ifdef	DPCMBank
	bne	@L
	sta	__env_note,x
.endif
	beq	@Zero
@L:
	lda	__ptr
	add	__tmp
	sta	__env_note,x
	lda	__ptr + 1
	adc	__tmp + 1
@Zero:	sta	__env_note + 1,x

	lda	#$01
	sta	__env_note_ptr,x
@Exit:
	jmp	Sequence

;=======================================================================
;		opcode	0x14:	Detune (16 = 100 cent)
;-----------------------------------------------------------------------
nsd_op14:
	jsr	nsd_load_sequence
	cpx	#nsd::TR_BGM5
	beq	@exit
	sta	__detune_cent,x
@exit:
	jmp	Sequence

;=======================================================================
;		opcode	0x15:	Detune (Resister base) 
;-----------------------------------------------------------------------
nsd_op15:
	jsr	nsd_load_sequence
	cpx	#nsd::TR_BGM5
	beq	@exit
	sta	__detune_fine,x
@exit:
	jmp	Sequence

;=======================================================================
;		opcode	0x16:	Sweep (Resister base) 
;-----------------------------------------------------------------------
nsd_op16:
	jsr	nsd_load_sequence
	jsr	_nsd_snd_sweep
	jmp	Sequence

;=======================================================================
;		opcode	0x17:	Portamento (Frequency += n2, every n3 [VBlank]) 
;-----------------------------------------------------------------------
nsd_op17:

	lda	__Sequence_ptr,x	;
	sta	__ptr			;
	add	#4			;[3]
	sta	__Sequence_ptr,x	;[4]

	ldy	__Sequence_ptr + 1,x	;
	sty	__ptr + 1		;	__ptr = __Sequence_ptr
	bcc	@l			;[2]9
	iny				;
	sty	__Sequence_ptr + 1,x	;
@l:

	cpx	#nsd::TR_BGM5
	beq	@exit

.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif

	ldy	#0
	lda	(__ptr),y
	sta	__por_ctr,x

	iny
	lda	(__ptr),y
	sta	__por_rate,x

	iny
	lda	(__ptr),y
	sta	__por_depth,x

	iny
	lda	(__ptr),y
	sta	__por_target,x

	lda	#0
	sta	__por_now + 0,x
	sta	__por_now + 1,x		;���݂̕ψ�

@exit:
	jmp	Sequence

;=======================================================================
;		opcode	0x1B:	Voice
;-----------------------------------------------------------------------
nsd_op1B:

	jsr	nsd_load_sequence

.ifdef	VRC7
	;VRC7�́Amode 2�̎��̓����[�X�������Ȃ��B
	cpx	#nsd::TR_VRC7
	bcc	@VRC7L
	cpx	#nsd::TR_VRC7 + 6*2
	bcs	@VRC7L
	jmp	nsd_op1B_FM
@VRC7L:
.endif

.ifdef	OPLL
	;OPLL�́Amode 2�̎��̓����[�X�������Ȃ��B
	cpx	#nsd::TR_OPLL
	bcc	@OPLLL
	cpx	#nsd::TR_OPLL + 9*2
	bcs	@OPLLL
	jmp	nsd_op1B_FM
@OPLLL:
.endif

nsd_op1B_done:

	sta	__env_voice,x
	lda	#0
	sta	__env_voice + 1,x

	lda	__gatemode,x
	and	#<~nsd_mode::voice	;Voice Envelope����
	sta	__gatemode,x

	jmp	Sequence

.if	.defined(VRC7) || .defined(OPLL)
nsd_op1B_FM:
	pha
	jsr	_nsd_snd_voice		;���F�����W�X�^�ݒ�
	pla

	cmp	#$10
	bcc	nsd_op1B_done		;16������������A���F��`
	jmp	Sequence
.endif

;=======================================================================
;		opcode	0x1C:	VRC7 : Set user instrument 
;-----------------------------------------------------------------------
nsd_op1C:

	jsr	nsd_load_ptr

.if	.defined(VRC7) || .defined(OPLL)
	lda	__ptr
	add	__tmp
	sta	__ptr

	lda	__ptr + 1
	adc	__tmp + 1
	sta	__ptr + 1		;__ptr �e�[�u���̃|�C���^
.endif

.ifdef	OPLL
	cpx	#nsd::TR_OPLL
	bcs	@OPLL
.endif

.ifdef	VRC7
@VRC7:
	lda	__ptr
	sta	__vrc7_reg
	lda	__ptr + 1
	sta	__vrc7_reg + 1

	ldx	#0			;8 byte table
	ldy	#0
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif
@L:
	stx	VRC7_Resister		;��Resister Write
	lda	(__ptr),y		;[5]
	iny				;[2]
	sta	VRC7_Data		;��Data Write
	jsr	_Wait42

	inx				;[2]
	cpx	#8			;[2]
	bne	@L			;[2]	6 + 36 = 42

	ldx	__channel
.endif
	jmp	Sequence


.ifdef	OPLL
@OPLL:
	lda	__ptr
	sta	__opll_reg
	lda	__ptr + 1
	sta	__opll_reg + 1

	ldx	#0			;8 byte table
	ldy	#0
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif
@L2:
	stx	OPLL_Resister		;��Resister Write
	lda	(__ptr),y		;[5]
	iny				;[2]
	sta	OPLL_Data		;��Data Write
	jsr	_Wait42

	inx				;[2]
	cpx	#8			;[2]
	bne	@L2			;[2]	6 + 36 = 42

	ldx	__channel

	jmp	Sequence
.endif

;=======================================================================
;		opcode	0x1D:	VRC7 : Set resister
;-----------------------------------------------------------------------
nsd_op1D:

	jsr	nsd_load_sequence

.ifdef	OPLL
	cpx	#nsd::TR_OPLL
	bcs	@OPLL
.endif

.ifdef	VRC7
	sta	VRC7_Resister
.endif
	jsr	nsd_load_sequence
.ifdef	VRC7
	sta	VRC7_Data
.endif
	jmp	Sequence

.ifdef	OPLL
@OPLL:
	sta	OPLL_Resister
	jsr	nsd_load_sequence
	sta	OPLL_Data
	jmp	Sequence
.endif

;=======================================================================
;		opcode	0x1E:	n163 : Set wave table
;-----------------------------------------------------------------------
nsd_op1E:
	jsr	nsd_load_sequence

.ifdef	N163
	shl	a, 1
	ora	#$80
	sta	N163_Resister
.endif

	jsr	nsd_load_ptr

.ifdef	N163
	lda	__ptr
	add	__tmp
	sta	__ptr

	lda	__ptr + 1
	adc	__tmp + 1
	sta	__ptr + 1		;__ptr �e�[�u���̃|�C���^

.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif

	ldy	#0
	lda	(__ptr),y
	iny
	tax				;x <- size of table
@L:
	lda	(__ptr),y
	iny
	sta	N163_Data

	dex
	bne	@L

	ldx	__channel

.endif
	jmp	Sequence

;=======================================================================
;		opcode	0x1F:	n163 : Set channel number
;-----------------------------------------------------------------------
nsd_op1F:
	jsr	nsd_load_sequence
.ifdef	N163
	shl	a, 4
	and	#$70
	sta	__n163_num
.endif
	jmp	Sequence

;=======================================================================
;		opcode	0x20:	Volume down (-1) 
;-----------------------------------------------------------------------
nsd_op20:
	lda	__volume,x
	and	#$0F
	beq	@L
	dec	__volume,x
@L:
	jmp	Sequence

;=======================================================================
;		opcode	0x21:	Volume up (+1) 
;-----------------------------------------------------------------------
nsd_op21:
	lda	__volume,x
	and	#$0F
	cmp	#$0F
	beq	@L
	inc	__volume,x
@L:
	jmp	Sequence

;=======================================================================
;		opcode	0x22:	FDS : Set career wave table
;-----------------------------------------------------------------------
nsd_op22:

	jsr	nsd_load_ptr

.ifdef	FDS
	lda	__ptr
	add	__tmp
	sta	__ptr

	lda	__ptr + 1
	adc	__tmp + 1
	sta	__ptr + 1		;__ptr �e�[�u���̃|�C���^

	lda	#$80
	sta	FDS_Write_Enable

.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif
	ldy	#0
@L:
	lda	(__ptr),y
	sta	FDS_Wave_Table,y
	iny
	cpy	#64
	bne	@L

	lda	__chflag,x
	and	#nsd_chflag::FDSVOL
	shr	a,2
	sta	FDS_Write_Enable
.endif
	jmp	Sequence

;=======================================================================
;		opcode	0x23:	FDS : Set modulator wave table
;-----------------------------------------------------------------------
nsd_op23:

	jsr	nsd_load_ptr

.ifdef	FDS
	lda	__ptr
	add	__tmp
	sta	__ptr

	lda	__ptr + 1
	adc	__tmp + 1
	sta	__ptr + 1		;__ptr �e�[�u���̃|�C���^

	lda	__fds_frequency
	ora	#$80
	sta	FDS_Mod_CTUNE

.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif
	ldy	#0
@L:
	lda	(__ptr),y
	sta	FDS_Mod_Append
	iny
	cpy	#32
	bne	@L

	lda	__fds_frequency
	sta	FDS_Mod_CTUNE
.endif
	jmp	Sequence

;=======================================================================
;		opcode	0x24:	FDS : Modulator frequency
;-----------------------------------------------------------------------
nsd_op24:

	jsr	nsd_load_sequence
.ifdef	FDS
	sta	FDS_Mod_FTUNE		;����
.endif

	jsr	nsd_load_sequence
.ifdef	FDS
	and	#$0F
	sta	FDS_Mod_CTUNE		;���
	sta	__fds_frequency
.endif

	jmp	Sequence

;=======================================================================
;		opcode	0x25:	FDS master volume
;-----------------------------------------------------------------------
nsd_op25:

.ifdef	FDS
	lda	__chflag,x
	and	#<~nsd_chflag::FDSVOL
	sta	__tmp
.endif

	jsr	nsd_load_sequence

.ifdef	FDS
	and	#$03
	sta	FDS_Write_Enable
	shl	a,2
	ora	__tmp
	sta	__chflag,x
.endif

	jmp	Sequence

;=======================================================================
;		opcode	0x26:	 FME7: Envelope frequency
;-----------------------------------------------------------------------
nsd_op26:

.ifdef	PSG
	ldy	#PSG_Envelope_Low
	sty	PSG_Register
.endif

	jsr	nsd_load_sequence

.ifdef	PSG
	sta	PSG_Data
	iny
	sty	PSG_Register
.endif

	jsr	nsd_load_sequence
	
.ifdef	PSG
	sta	PSG_Data
.endif

nsd_op27:
	jmp	Sequence

;=======================================================================
;		opcode	0x2A:	Transpose (an absolute value) 
;-----------------------------------------------------------------------
nsd_op2A:
	jsr	nsd_load_sequence
nsd_Set_Trans:
	sta	__trans,x
	jmp	Sequence

;=======================================================================
;		opcode	0x2B:	Transpose (a relative value) 
;-----------------------------------------------------------------------
nsd_op2B:
	jsr	nsd_load_sequence
	add	__trans,x
	jmp	nsd_Set_Trans

;=======================================================================
;		opcode	0x2C:	One time octave down (-1) 
;-----------------------------------------------------------------------
nsd_op2C:
	lda	#<-12
	bne	nsd_Set_Trans_One

;=======================================================================
;		opcode	0x2D:	One time octave up (+1) 
;-----------------------------------------------------------------------
nsd_op2D:
	lda	#+12
	bne	nsd_Set_Trans_One

;=======================================================================
;		opcode	0x2E:	One time transpose (a relative value) 
;-----------------------------------------------------------------------
nsd_op2E:
	jsr	nsd_load_sequence
nsd_Set_Trans_One:
	add	__trans_one,x
	sta	__trans_one,x
	jmp	Sequence

;=======================================================================
;		opcode	0x2F:	Sub opcode
;-----------------------------------------------------------------------
.rodata
_sub_op_adr:
	.addr	nsd_op2F_00
	.addr	nsd_op2F_01
	.addr	nsd_op2F_02
	.addr	nsd_op2F_03
;	.addr	nsd_op2F_04
;	.addr	nsd_op2F_05
;	.addr	nsd_op2F_06
;	.addr	nsd_op2F_07

;	.addr	nsd_op2F_08
;	.addr	nsd_op2F_09
;	.addr	nsd_op2F_0A
;	.addr	nsd_op2F_0B
;	.addr	nsd_op2F_0C
;	.addr	nsd_op2F_0D
;	.addr	nsd_op2F_0E
;	.addr	nsd_op2F_0F

.code
nsd_op2F:
	jsr	nsd_load_sequence
	tay
	asl	a
	bcs	nsd_op2F_80

	;---------------
	;sub op-code = 0x00 - 0x7F
nsd_op2F_Command:
	tay				;[2]	x <- a * 2
	lda	_sub_op_adr,y		;[4]
	sta	__ptr			;[3]
	lda	_sub_op_adr + 1,y	;[4]
	sta	__ptr + 1		;[3]
	jmp	(__ptr)			;[5]	jump for each op-code

	;---------------
	;sub op-code = 0x80 - 0xFF
	;---------------
	;0x80 - 0x8F
nsd_op2F_80:
;	asl	a
;	bcs	nsd_op2F_C0
;	asl	a
;	bcs	nsd_op2F_A0
;	bmi	nsd_op2F_90
;
;	tya
;	and	#$0F
;	sta	__tmp
;	lda	__volume,x
;	and	#$0F
;	add	__tmp
;	cmp	#$10
;	bcc	@L
;	lda	#$0F
;@L:	jmp	Set_Volume

	;---------------
	;0x90 - 0x9F
nsd_op2F_90:
;	tya
;	and	#$0F
;	sta	__tmp
;	lda	__volume,x
;	and	#$0F
;	sub	__tmp
;	bcs	@L
;	lda	#$00
;@L:	jmp	Set_Volume

	;---------------
	;0xA0 - 
nsd_op2F_A0:
nsd_op2F_B0:
nsd_op2F_C0:
nsd_op2F_D0:
nsd_op2F_E0:
nsd_op2F_F0:
	jmp	Sequence


;=======================================================================
;		opcode	0x2F 00:	Jump Flag On
;-----------------------------------------------------------------------
nsd_op2F_00:
	lda	#nsd_flag::Jump
	ora	__flag
	sta	__flag
	jmp	Sequence

;=======================================================================
;		opcode	0x2F 01:	Jump Flag Off
;-----------------------------------------------------------------------
nsd_op2F_01:
	lda	#<~nsd_flag::Jump
	and	__flag
	sta	__flag
	jmp	Sequence

;=======================================================================
;		opcode	0x2F 02:	Relative Detune (CENT)
;-----------------------------------------------------------------------
nsd_op2F_02:
	jsr	nsd_load_sequence
	add	__detune_cent,x
	sta	__detune_cent,x
	jmp	Sequence

;=======================================================================
;		opcode	0x2F 03:	Relative Detune (Reg.)
;-----------------------------------------------------------------------
nsd_op2F_03:
	jsr	nsd_load_sequence
	add	__detune_fine,x
	sta	__detune_fine,x

nsd_op2F_04:
nsd_op2F_05:
nsd_op2F_06:
nsd_op2F_07:
	jmp	Sequence

;=======================================================================
;		opcode	0x30 - 0x37:	Voice of release (after key-off) 
;-----------------------------------------------------------------------
nsd_op30:
	lda	#$00
nsd_Set_Voice:
	sta	__tmp
	lda	__gatemode,x
	and	#$0F
	ora	__tmp
	sta	__gatemode,x
	jmp	Sequence

nsd_op31:
	lda	#$10
	bne	nsd_Set_Voice

nsd_op32:
	lda	#$20
	bne	nsd_Set_Voice

nsd_op33:
	lda	#$30
	bne	nsd_Set_Voice

nsd_op34:
	lda	#$40
	bne	nsd_Set_Voice

nsd_op35:
	lda	#$50
	bne	nsd_Set_Voice

nsd_op36:
	lda	#$60
	bne	nsd_Set_Voice

nsd_op37:
	lda	#$70
	bne	nsd_Set_Voice

;=======================================================================
;		opcode	0x29:	Octave up (+1) 
;-----------------------------------------------------------------------
nsd_op29:
	lda	__octave,x
	add	#12
	bpl	nsd_Set_Octave
	jmp	Sequence

;=======================================================================
;		opcode	0x28:	Octave down (-1) 
;-----------------------------------------------------------------------
nsd_op28:
	lda	__octave,x
	sub	#12
	bmi	nsd_op28_Exit
nsd_Set_Octave:
	sta	__octave,x
nsd_op28_Exit:
	jmp	Sequence

;=======================================================================
;		opcode	0x38 - 0x3F:	Octave
;-----------------------------------------------------------------------
nsd_op38:
	lda	#0
	beq	nsd_Set_Octave

nsd_op39:
	lda	#12
	bne	nsd_Set_Octave

nsd_op3A:
	lda	#24
	bne	nsd_Set_Octave

nsd_op3B:
	lda	#36
	bne	nsd_Set_Octave

nsd_op3C:
	lda	#48
	bne	nsd_Set_Octave

nsd_op3D:
	lda	#60
	bne	nsd_Set_Octave

nsd_op3E:
	lda	#72
	bne	nsd_Set_Octave

nsd_op3F:
	lda	#84
	bne	nsd_Set_Octave

nsd_op2F_08:
;	lda	#96
;	bne	nsd_Set_Octave

nsd_op2F_09:
;	lda	#108
;	bne	nsd_Set_Octave

nsd_op2F_0A:
;	lda	#120
;	bne	nsd_Set_Octave

nsd_op2F_0B:
;	lda	#132
;	bne	nsd_Set_Octave

nsd_op2F_0C:
;	lda	#144
;	bne	nsd_Set_Octave

nsd_op2F_0D:
;	lda	#156
;	bne	nsd_Set_Octave

nsd_op2F_0E:
;	lda	#168
;	bne	nsd_Set_Octave

nsd_op2F_0F:
;	lda	#180
;	bne	nsd_Set_Octave

.endproc

