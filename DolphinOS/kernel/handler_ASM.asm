global _asm_general_handler
global _asm_inthandler21_keyboard
global _asm_inthandler20_timer
global read_cr3_ASM

extern inthandler21_keyboard
extern inthandler20_timer
extern general_handler

INT_M_CTL EQU 0X20
EOI EQU 0X20

[section .text]
[bits 32]
;.code32 'worry label

_asm_inthandler21_keyboard:	; IRQ 1		keyboard
	PUSH	ES			
	PUSH	DS
	PUSHAD					
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS			
	MOV		DS,AX
	MOV		ES,AX
	CALL	inthandler21_keyboard	
	POP		EAX				
	POPAD
	POP		DS
	POP		ES
	IRETD					
	
_asm_inthandler20_timer:
	push es
	push ds
	pushad
	mov eax,esp
	push eax
	mov ax,ss
	mov ds,ax
	mov es,ax
	call inthandler20_timer
	pop eax
	popad
	pop ds
	pop es
	iretd

_asm_general_handler:
	push es
	push ds
	pushad
	mov eax,esp
	push eax
	mov ax,ss
	mov ds,ax
	mov es,ax
	call general_handler
	pop eax
	popad
	pop ds
	pop es
	iretd

read_cr3_ASM:
	mov eax, cr3
	ret

