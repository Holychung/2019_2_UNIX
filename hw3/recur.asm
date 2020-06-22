push 20 ; depends on question 
call MAIN
jmp EXIT

MAIN:
	push rbp
	mov rbp, rsp
	mov rbx, [rbp+0x10]

	cmp rbx, 0
	jle ZERO
	cmp rbx, 1
	je ONE

	OTHERS:
	sub rbx, 1
	push rbx
	call MAIN

	mov edx, 2
	mul edx
	pop rbx
	push rax

	sub rbx, 1
	push rbx
	call MAIN

	mov edx, 3
	mul edx
	pop rbx
	pop rbx
	add rax, rbx
	jmp RETURN

	ZERO:
		mov rax, 0
		jmp RETURN
	ONE:
		mov rax, 1
		jmp RETURN

	RETURN:
	mov rsp, rbp
	pop rbp
	ret

EXIT:
done: