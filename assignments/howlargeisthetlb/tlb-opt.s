	.file	"tlb.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"#Pages\t proc\t sum"
.LC2:
	.string	"%d\t %.6f\t %ld \n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB39:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$0, %r12d
	movl	$1, %ebx
	jmp	.L6
.L4:
	addl	$1, %eax
	cmpl	%ebx, %eax
	jne	.L4
	leaq	1(%r12,%rbp), %rbp
.L5:
	addl	$1, %edx
	cmpl	%edx, %r13d
	je	.L3
.L2:
	movl	$0, %eax
	testl	%ebx, %ebx
	jg	.L4
	jmp	.L5
.L3:
	call	clock@PLT
	subq	%r14, %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC1(%rip), %xmm0
	movq	%rbp, %rcx
	movl	%ebx, %edx
	leaq	.LC2(%rip), %rsi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk@PLT
	addl	$1, %ebx
	addq	$1, %r12
	cmpl	$17, %ebx
	je	.L14
.L6:
	movl	$1048576, %eax
	cltd
	idivl	%ebx
	movl	%eax, %r15d
	movl	%eax, %r13d
	call	clock@PLT
	movq	%rax, %r14
	movl	$0, %edx
	movl	$0, %ebp
	testl	%r15d, %r15d
	jg	.L2
	jmp	.L3
.L14:
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE39:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1093567616
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
