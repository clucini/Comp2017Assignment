	.file	"runtest.c"
	.text
	.section	.rodata
.LC0:
	.string	"wb+"
.LC1:
	.string	"rb+"
.LC2:
	.string	"%s"
	.text
	.globl	init_fs
	.type	init_fs, @function
init_fs:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movl	%ecx, -44(%rbp)
	movl	$40, %edi
	call	malloc@PLT
	movq	%rax, -16(%rbp)
	movq	-24(%rbp), %rax
	leaq	.LC0(%rip), %rsi
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-32(%rbp), %rax
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, 8(%rax)
	movq	-40(%rbp), %rax
	leaq	.LC0(%rip), %rsi
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, 16(%rax)
	movq	-16(%rbp), %rax
	movl	-44(%rbp), %edx
	movl	%edx, 24(%rax)
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movl	$2, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	fseek@PLT
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	ftell@PLT
	movabsq	$-2049638230412172401, %rdx
	mulq	%rdx
	movq	%rdx, %rax
	shrq	$6, %rax
	movl	%eax, %edx
	movq	-16(%rbp), %rax
	movl	%edx, 28(%rax)
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	fseek@PLT
	movq	-16(%rbp), %rax
	movl	28(%rax), %eax
	cltq
	movl	$72, %esi
	movq	%rax, %rdi
	call	calloc@PLT
	movq	%rax, -8(%rbp)
	movq	-16(%rbp), %rax
	movq	8(%rax), %rcx
	movq	-16(%rbp), %rax
	movl	28(%rax), %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	movl	$72, %esi
	movq	%rax, %rdi
	call	fread@PLT
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-16(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, 32(%rax)
	movq	-16(%rbp), %rax
	movq	32(%rax), %rax
	addq	$72, %rax
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-16(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	init_fs, .-init_fs
	.globl	close_fs
	.type	close_fs, @function
close_fs:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	close_fs, .-close_fs
	.globl	check_gap_after
	.type	check_gap_after, @function
check_gap_after:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movl	%ecx, -44(%rbp)
	movl	$0, -12(%rbp)
	jmp	.L6
.L11:
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	salq	$3, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	movq	%rax, %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	cmpq	-24(%rbp), %rax
	je	.L12
	movq	-24(%rbp), %rax
	movl	64(%rax), %edx
	movq	-8(%rbp), %rax
	movl	64(%rax), %eax
	cmpl	%eax, %edx
	jb	.L12
	movq	-8(%rbp), %rax
	movl	64(%rax), %edx
	movq	-24(%rbp), %rax
	movl	64(%rax), %ecx
	movq	-24(%rbp), %rax
	movl	68(%rax), %eax
	addl	%ecx, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, %eax
	cmpq	%rax, -40(%rbp)
	jbe	.L9
	movl	$0, %eax
	jmp	.L10
.L12:
	nop
.L9:
	addl	$1, -12(%rbp)
.L6:
	movl	-12(%rbp), %eax
	cmpl	-44(%rbp), %eax
	jl	.L11
	movl	$1, %eax
.L10:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	check_gap_after, .-check_gap_after
	.globl	find_gap
	.type	find_gap, @function
find_gap:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$56, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%rdx, -56(%rbp)
	movq	-48(%rbp), %rax
	movl	28(%rax), %eax
	movl	%eax, -20(%rbp)
	movq	$0, -16(%rbp)
	movl	$0, -24(%rbp)
	jmp	.L14
.L17:
	movq	-48(%rbp), %rax
	movq	32(%rax), %rcx
	movl	-24(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	salq	$3, %rax
	addq	%rdx, %rax
	salq	$3, %rax
	addq	%rcx, %rax
	movq	%rax, -8(%rbp)
	movq	-48(%rbp), %rax
	movq	32(%rax), %rsi
	movl	-20(%rbp), %ecx
	movq	-40(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	check_gap_after
	testb	%al, %al
	je	.L15
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
	jmp	.L16
.L15:
	addl	$1, -24(%rbp)
.L14:
	movl	-24(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.L17
.L16:
	movq	-16(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	find_gap, .-find_gap
	.section	.rodata
.LC3:
	.string	"yeet"
	.text
	.globl	create_file
	.type	create_file, @function
create_file:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%rdi, -56(%rbp)
	movq	%rsi, -64(%rbp)
	movq	%rdx, -72(%rbp)
	movq	-72(%rbp), %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -24(%rbp)
	movl	$72, %edi
	call	malloc@PLT
	movq	%rax, -16(%rbp)
	movl	$0, -36(%rbp)
	movq	-32(%rbp), %rax
	movl	28(%rax), %eax
	testl	%eax, %eax
	je	.L20
	movq	-24(%rbp), %rdx
	movq	-72(%rbp), %rcx
	movq	-64(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	find_gap
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	64(%rax), %edx
	movq	-8(%rbp), %rax
	movl	68(%rax), %eax
	addl	%edx, %eax
	movl	%eax, -36(%rbp)
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L20:
	movq	-16(%rbp), %rax
	movq	-56(%rbp), %rcx
	movl	$64, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	strncpy@PLT
	movq	-64(%rbp), %rax
	movl	%eax, %edx
	movq	-16(%rbp), %rax
	movl	%edx, 68(%rax)
	movq	-16(%rbp), %rax
	movl	-36(%rbp), %edx
	movl	%edx, 64(%rax)
	movl	-36(%rbp), %edx
	movq	-24(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	fseek@PLT
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, %rcx
	movl	$1, %edx
	movl	$72, %esi
	movq	%rax, %rdi
	call	fwrite@PLT
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	create_file, .-create_file
	.globl	resize_file
	.type	resize_file, @function
resize_file:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	resize_file, .-resize_file
	.globl	repack
	.type	repack, @function
repack:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	repack, .-repack
	.globl	delete_file
	.type	delete_file, @function
delete_file:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	delete_file, .-delete_file
	.globl	rename_file
	.type	rename_file, @function
rename_file:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	rename_file, .-rename_file
	.globl	read_file
	.type	read_file, @function
read_file:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	%r8, -40(%rbp)
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	read_file, .-read_file
	.globl	write_file
	.type	write_file, @function
write_file:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	%r8, -40(%rbp)
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	write_file, .-write_file
	.globl	file_size
	.type	file_size, @function
file_size:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	file_size, .-file_size
	.globl	fletcher
	.type	fletcher, @function
fletcher:
.LFB17:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	fletcher, .-fletcher
	.globl	compute_hash_tree
	.type	compute_hash_tree, @function
compute_hash_tree:
.LFB18:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	compute_hash_tree, .-compute_hash_tree
	.globl	compute_hash_block
	.type	compute_hash_block, @function
compute_hash_block:
.LFB19:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	compute_hash_block, .-compute_hash_block
	.globl	success
	.type	success, @function
success:
.LFB20:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	success, .-success
	.globl	failure
	.type	failure, @function
failure:
.LFB21:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$1, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	failure, .-failure
	.section	.rodata
.LC4:
	.string	"file3"
.LC5:
	.string	"file2"
.LC6:
	.string	"file1"
	.text
	.globl	no_operation
	.type	no_operation, @function
no_operation:
.LFB22:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$1, %ecx
	leaq	.LC4(%rip), %rdx
	leaq	.LC5(%rip), %rsi
	leaq	.LC6(%rip), %rdi
	call	init_fs
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	close_fs
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	no_operation, .-no_operation
	.section	.rodata
.LC7:
	.string	"c"
.LC8:
	.string	"b"
.LC9:
	.string	"a"
.LC10:
	.string	"large_penis.txt"
	.text
	.globl	create_fil
	.type	create_fil, @function
create_fil:
.LFB23:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$1, %ecx
	leaq	.LC7(%rip), %rdx
	leaq	.LC8(%rip), %rsi
	leaq	.LC9(%rip), %rdi
	call	init_fs
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdx
	movl	$200, %esi
	leaq	.LC10(%rip), %rdi
	call	create_file
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	close_fs
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE23:
	.size	create_fil, .-create_fil
	.section	.rodata
.LC11:
	.string	"Passed %s\n"
.LC12:
	.string	"Failed %s returned %d\n"
	.text
	.globl	test
	.type	test, @function
test:
.LFB24:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rdx
	movl	$0, %eax
	call	*%rdx
	movl	%eax, -4(%rbp)
	cmpl	$0, -4(%rbp)
	jne	.L51
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC11(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L53
.L51:
	movl	-4(%rbp), %edx
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC12(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L53:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE24:
	.size	test, .-test
	.section	.rodata
.LC13:
	.string	"create_fil"
	.text
	.globl	main
	.type	main, @function
main:
.LFB25:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	leaq	.LC13(%rip), %rsi
	leaq	create_fil(%rip), %rdi
	call	test
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE25:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
