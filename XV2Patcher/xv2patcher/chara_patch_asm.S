	.intel_syntax noprefix
	.globl	GetRealAura
	.def	GetRealAura;	.scl	2;	.type	32;	.endef
	
GetRealAura:

	cmp	rcx, 4095
	ja	ret1
	lea	rdx, cus_aura_lookup[rip]
	movzx	eax, WORD PTR [rdx+rcx*2]
	cmp	ax, -1
	je	ret1
	mov ebx, eax
	
ret1:

	mov rdx, 0x123456789ABCDEF
	jmp rdx
	.p2align 4,,15
	
	.global GetCABehaviour11
	.def	GetCABehaviour11;	.scl	2;	.type	32;	.endef
	
GetCABehaviour11:

	cmp edx, 4095
	ja orig2
	lea rcx, cus_aura_bh11_lookup[rip]
	movsx rdx, edx
	movzx ecx, BYTE PTR [rcx+rdx]
	cmp ecx, 21
	ja orig2
	mov edx, ecx
	
orig2:

	cmp edx, 2
	jl ret2
	
	cmp edx, 3
	jle lab_E0
	
	cmp edx, 0xD
	je lab_E0
	
	cmp edx, 0x16
	je lab_E0
	
	cmp edx, 0x17
	jne ret2
	
	mov DWORD PTR [rbx+0xDC], 0x1C2
	mov DWORD PTR [rbx+0xE0], 0x1C3
	jmp ret2
	
lab_E0:

	mov DWORD PTR [rbx+0xDC], 0xE0
	mov DWORD PTR [rbx+0xE0], 0xE1

ret2:

	mov rcx, 0x123456789ABCDEF
	jmp rcx
	.p2align 4,,15
	

	.intel_syntax noprefix
	.globl	ForceTeleport
	.def	ForceTeleport;	.scl	2;	.type	32;	.endef
	
ForceTeleport:

	mov ecx, [rsi+0x13B8]
	cmp ecx, 4095
	ja default_teleport
	lea rdx, force_teleport[rip]
	movzx ecx, BYTE PTR [rcx+rdx]
	test ecx, ecx
	jne	teleport
	
default_teleport:

	cmp  [rsi+0x13B8], eax
	jne no_teleport

teleport:	
	
	mov rcx, 0x123456789ABCDEF
	jmp rcx
	
no_teleport:

	mov rcx, 0xFEDCBA987654321
	jmp rcx

	.p2align 4,,15
	

	
	