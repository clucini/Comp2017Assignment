	.file	"runtest.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"wb+"
.LC1:
	.string	"rb+"
.LC2:
	.string	"%s"
	.text
	.p2align 4,,15
	.globl	init_fs
	.type	init_fs, @function
init_fs:
.LFB52:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movq	%rdi, %r14
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movl	$40, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movl	%ecx, %ebp
	movq	%rdx, %r12
	movq	%rsi, %r13
	call	malloc@PLT
	leaq	.LC0(%rip), %rsi
	movq	%r14, %rdi
	movq	%rax, %rbx
	call	fopen@PLT
	leaq	.LC1(%rip), %rsi
	movq	%r13, %rdi
	movq	%rax, (%rbx)
	call	fopen@PLT
	leaq	.LC0(%rip), %rsi
	movq	%r12, %rdi
	movq	%rax, 8(%rbx)
	call	fopen@PLT
	movq	8(%rbx), %rdi
	movl	$2, %edx
	xorl	%esi, %esi
	movl	%ebp, 24(%rbx)
	movq	%rax, 16(%rbx)
	call	fseek@PLT
	movq	8(%rbx), %rdi
	call	ftell@PLT
	movabsq	$-2049638230412172401, %rdx
	movq	8(%rbx), %rdi
	xorl	%esi, %esi
	mulq	%rdx
	shrq	$6, %rdx
	movl	%edx, 28(%rbx)
	xorl	%edx, %edx
	call	fseek@PLT
	movslq	28(%rbx), %r12
	movl	$72, %esi
	movq	%r12, %rdi
	call	calloc@PLT
	movq	8(%rbx), %rcx
	movq	%r12, %rdx
	movl	$72, %esi
	movq	%rax, %rdi
	movq	%rax, %rbp
	call	fread@PLT
	leaq	.LC2(%rip), %rsi
	movq	%rbp, %rdx
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	leaq	72(%rbp), %rdx
	leaq	.LC2(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	movq	%rbp, 32(%rbx)
	call	__printf_chk@PLT
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE52:
	.size	init_fs, .-init_fs
	.p2align 4,,15
	.globl	close_fs
	.type	close_fs, @function
close_fs:
.LFB53:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	(%rdi), %rdi
	call	fclose@PLT
	movq	8(%rbx), %rdi
	call	fclose@PLT
	movq	16(%rbx), %rdi
	call	fclose@PLT
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.cfi_endproc
.LFE53:
	.size	close_fs, .-close_fs
	.p2align 4,,15
	.globl	check_gap_after
	.type	check_gap_after, @function
check_gap_after:
.LFB54:
	.cfi_startproc
	testl	%ecx, %ecx
	jle	.L10
	leal	-1(%rcx), %eax
	leaq	(%rax,%rax,8), %rax
	leaq	72(%rsi,%rax,8), %r8
	.p2align 4,,10
	.p2align 3
.L9:
	cmpq	%rsi, %rdi
	je	.L8
	movl	64(%rdi), %ecx
	movl	64(%rsi), %eax
	cmpl	%eax, %ecx
	jb	.L8
	addl	68(%rdi), %ecx
	subl	%ecx, %eax
	cmpq	%rdx, %rax
	jb	.L11
.L8:
	addq	$72, %rsi
	cmpq	%rsi, %r8
	jne	.L9
.L10:
	movl	$1, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L11:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE54:
	.size	check_gap_after, .-check_gap_after
	.p2align 4,,15
	.globl	find_gap
	.type	find_gap, @function
find_gap:
.LFB55:
	.cfi_startproc
	movl	28(%rsi), %eax
	testl	%eax, %eax
	jle	.L22
	movq	32(%rsi), %r10
	subl	$1, %eax
	leaq	(%rax,%rax,8), %rax
	leaq	72(%r10,%rax,8), %r9
	movq	%r10, %r8
	.p2align 4,,10
	.p2align 3
.L18:
	movq	%r8, %rax
	movq	%r10, %rdx
	.p2align 4,,10
	.p2align 3
.L17:
	cmpq	%rdx, %r8
	je	.L15
	movl	64(%r8), %esi
	movl	64(%rdx), %ecx
	cmpl	%ecx, %esi
	jb	.L15
	addl	68(%r8), %esi
	subl	%esi, %ecx
	cmpq	%rcx, %rdi
	ja	.L16
.L15:
	addq	$72, %rdx
	cmpq	%r9, %rdx
	jne	.L17
	rep ret
	.p2align 4,,10
	.p2align 3
.L16:
	addq	$72, %r8
	cmpq	%r9, %r8
	jne	.L18
.L22:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE55:
	.size	find_gap, .-find_gap
	.section	.rodata.str1.1
.LC3:
	.string	"yeet"
	.text
	.p2align 4,,15
	.globl	create_file
	.type	create_file, @function
create_file:
.LFB56:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movq	%rsi, %r14
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rdx, %r13
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rdi, %r12
	movl	$72, %edi
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	movq	8(%rdx), %rbp
	call	malloc@PLT
	movl	28(%r13), %r15d
	movq	%rax, %rbx
	cmpl	$0, %r15d
	jne	.L24
	xorl	%r13d, %r13d
.L25:
	movq	%r12, %rsi
	movq	%rbx, %rdi
	movl	$64, %edx
	call	strncpy@PLT
	movl	%r15d, %edx
	movq	%rbp, %rdi
	xorl	%esi, %esi
	movl	%r14d, 68(%rbx)
	movl	%r13d, 64(%rbx)
	call	fseek@PLT
	movq	%rbp, %rcx
	movq	%rbx, %rdi
	movl	$1, %edx
	movl	$72, %esi
	call	fwrite@PLT
	movq	%rbp, %rdi
	call	fclose@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
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
	.p2align 4,,10
	.p2align 3
.L24:
	.cfi_restore_state
	jle	.L31
	leal	-1(%r15), %eax
	movq	32(%r13), %r8
	leaq	(%rax,%rax,8), %rax
	movq	%r8, %rcx
	leaq	72(%r8,%rax,8), %rsi
	.p2align 4,,10
	.p2align 3
.L32:
	movl	64(%rcx), %edi
	movl	68(%rcx), %r13d
	movq	%r8, %rax
	addl	%edi, %r13d
	.p2align 4,,10
	.p2align 3
.L28:
	cmpq	%rax, %rcx
	je	.L26
	movl	64(%rax), %edx
	cmpl	%edi, %edx
	ja	.L26
	subl	%r13d, %edx
	cmpq	%rdx, %r14
	ja	.L27
.L26:
	addq	$72, %rax
	cmpq	%rax, %rsi
	jne	.L28
	leaq	.LC3(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	movl	%r13d, %r15d
	call	__printf_chk@PLT
	jmp	.L25
	.p2align 4,,10
	.p2align 3
.L27:
	addq	$72, %rcx
	cmpq	%rsi, %rcx
	jne	.L32
.L31:
	movl	64, %eax
	ud2
	.cfi_endproc
.LFE56:
	.size	create_file, .-create_file
	.section	.rodata.str1.1
.LC4:
	.string	"c"
.LC5:
	.string	"b"
.LC6:
	.string	"a"
.LC7:
	.string	"large_penis.txt"
	.text
	.p2align 4,,15
	.globl	create_fil
	.type	create_fil, @function
create_fil:
.LFB70:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	leaq	.LC4(%rip), %rdx
	leaq	.LC5(%rip), %rsi
	leaq	.LC6(%rip), %rdi
	movl	$1, %ecx
	call	init_fs
	leaq	.LC7(%rip), %rdi
	movq	%rax, %rbx
	movq	%rax, %rdx
	movl	$200, %esi
	call	create_file
	movq	(%rbx), %rdi
	call	fclose@PLT
	movq	8(%rbx), %rdi
	call	fclose@PLT
	movq	16(%rbx), %rdi
	call	fclose@PLT
	movq	%rbx, %rdi
	call	free@PLT
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE70:
	.size	create_fil, .-create_fil
	.p2align 4,,15
	.globl	resize_file
	.type	resize_file, @function
resize_file:
.LFB57:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE57:
	.size	resize_file, .-resize_file
	.p2align 4,,15
	.globl	repack
	.type	repack, @function
repack:
.LFB58:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE58:
	.size	repack, .-repack
	.p2align 4,,15
	.globl	delete_file
	.type	delete_file, @function
delete_file:
.LFB59:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE59:
	.size	delete_file, .-delete_file
	.p2align 4,,15
	.globl	rename_file
	.type	rename_file, @function
rename_file:
.LFB60:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE60:
	.size	rename_file, .-rename_file
	.p2align 4,,15
	.globl	read_file
	.type	read_file, @function
read_file:
.LFB61:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE61:
	.size	read_file, .-read_file
	.p2align 4,,15
	.globl	write_file
	.type	write_file, @function
write_file:
.LFB77:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE77:
	.size	write_file, .-write_file
	.p2align 4,,15
	.globl	file_size
	.type	file_size, @function
file_size:
.LFB63:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE63:
	.size	file_size, .-file_size
	.p2align 4,,15
	.globl	fletcher
	.type	fletcher, @function
fletcher:
.LFB64:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE64:
	.size	fletcher, .-fletcher
	.p2align 4,,15
	.globl	compute_hash_tree
	.type	compute_hash_tree, @function
compute_hash_tree:
.LFB75:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE75:
	.size	compute_hash_tree, .-compute_hash_tree
	.p2align 4,,15
	.globl	compute_hash_block
	.type	compute_hash_block, @function
compute_hash_block:
.LFB66:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE66:
	.size	compute_hash_block, .-compute_hash_block
	.p2align 4,,15
	.globl	success
	.type	success, @function
success:
.LFB67:
	.cfi_startproc
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE67:
	.size	success, .-success
	.p2align 4,,15
	.globl	failure
	.type	failure, @function
failure:
.LFB68:
	.cfi_startproc
	movl	$1, %eax
	ret
	.cfi_endproc
.LFE68:
	.size	failure, .-failure
	.section	.rodata.str1.1
.LC8:
	.string	"file3"
.LC9:
	.string	"file2"
.LC10:
	.string	"file1"
	.text
	.p2align 4,,15
	.globl	no_operation
	.type	no_operation, @function
no_operation:
.LFB69:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	leaq	.LC8(%rip), %rdx
	leaq	.LC9(%rip), %rsi
	leaq	.LC10(%rip), %rdi
	movl	$1, %ecx
	call	init_fs
	movq	(%rax), %rdi
	movq	%rax, %rbx
	call	fclose@PLT
	movq	8(%rbx), %rdi
	call	fclose@PLT
	movq	16(%rbx), %rdi
	call	fclose@PLT
	movq	%rbx, %rdi
	call	free@PLT
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE69:
	.size	no_operation, .-no_operation
	.section	.rodata.str1.1
.LC11:
	.string	"Passed %s\n"
.LC12:
	.string	"Failed %s returned %d\n"
	.text
	.p2align 4,,15
	.globl	test
	.type	test, @function
test:
.LFB71:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	xorl	%eax, %eax
	movq	%rsi, %rbx
	call	*%rdi
	testl	%eax, %eax
	je	.L57
	movq	%rbx, %rdx
	leaq	.LC12(%rip), %rsi
	movl	%eax, %ecx
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movl	$1, %edi
	xorl	%eax, %eax
	jmp	__printf_chk@PLT
	.p2align 4,,10
	.p2align 3
.L57:
	.cfi_restore_state
	movq	%rbx, %rdx
	leaq	.LC11(%rip), %rsi
	movl	$1, %edi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	__printf_chk@PLT
	.cfi_endproc
.LFE71:
	.size	test, .-test
	.section	.rodata.str1.1
.LC13:
	.string	"create_fil"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB72:
	.cfi_startproc
	leaq	.LC13(%rip), %rsi
	leaq	create_fil(%rip), %rdi
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	test
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE72:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
