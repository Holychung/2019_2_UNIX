addsub1
mov eax, 0xadf8fb97
add eax, 0xd9c1fb36
sub eax, 0x75c44445
done:

addsub2
mov eax, [0x600000]
add eax, [0x600004]
sub eax, [0x600008]
mov [0x60000c], eax
done:

swapreg
xchg rax, rbx
done:

swapmem
mov eax, [0x600000]
mov ebx, [0x600008]
mov [0x600008], eax
mov [0x600000], ebx
done:

leax
lea eax, [edi*2]
lea ebx, [edi*2+edi]
lea ecx, [edi*4+edi]
lea edx, [edi*8+edi]
done:

eval1
add eax, [0x600004]
sub eax, [0x600008]
sub eax, [0x600000]
mov [0x60000c], eax
done:

tolower
mov eax, [0x600000] 
sub eax, 32
mov [0x600001], eax
done:

clear17
and eax, 4294836223
done:
; zero-based means start form 0 index
; 11111111111111011111111111111111

dec2ascii
add al, 48
done:

ul+lu
xor ch, 32
done:

mulbyshift
; mul result is in edx:eax
mov eax, [0x600000]
mov ebx, 26
imul ebx
shl rdx, 32
mov edx, eax
mov [0x600004], rdx
done:

isolatebit
shr ax, 5
shl al, 1
ror al, 1
mov [0x600000], al
done:

math1
; mul result is in edx:eax
add eax, [0x600000]
add eax, [0x600004]
mov ebx, [0x600008]
imul ebx
shl rdx, 32
mov edx, eax
mov [0x60000c], rdx
done:

math2
mov eax, [0x600000]
neg eax
mov ebx, [0x600004]
imul ebx
shl rdx, 32
mov edx, eax
add rdx, [0x600008]
mov rax, rdx
done:

math3
; 32-bit unsigned arithmetic
; note: overflowed part should be truncated
mov eax, [0x600000]
mov ebx, 5
mul ebx
CDQ
mov ecx, [0x600004]
sub ecx, 3
div ecx
mov [0x600008], eax
done:

math4
; 32-bit signed arithmetic
; note: overflowed part should be truncated
mov eax, [0x600000]
mov ebx, 5
neg ebx
imul ebx
CDQ
mov ebx, eax

mov eax, [0x600004]
neg eax
CDQ
mov ecx, [0x600008]
idiv ecx
mov ecx, edx

mov eax, ebx
CDQ
idiv ecx
mov [0x60000c], eax
done:

math5 
; 32-bit signed arithmetic
; note: overflowed part should be truncated
mov eax, [0x600000]
mov ecx, [0x600004]
neg ecx
imul ecx
CDQ

mov ecx, [0x600008]
sub ecx, ebx
CDQ

idiv ecx
mov [0x600008], eax
done:

posneg
; test if registers are positive or negative.
cmp eax, 0
jl L1
mov eax, 1
mov [0x600000], eax
jmp L12
L1: 
mov eax, -1
mov [0x600000], eax

L12:
cmp ebx, 0
jl L2
mov ebx, 1
mov [0x600004], ebx
jmp L23
L2: 
mov ebx, -1
mov [0x600004], ebx

L23:
cmp ecx, 0
jl L3
mov ecx, 1
mov [0x600008], ecx
jmp L34
L3: 
mov ecx, -1
mov [0x600008], ecx

L34:
cmp edx, 0
jl L4
mov edx, 1
mov [0x60000c], edx
jmp L44
L4: 
mov edx, -1
mov [0x60000c], edx
L44:
done:
; mov to variable 會噴錯 ambiguous size for move

loop15
mov rax, [0x600000]
mov ecx, 8
L1:
cmp al, 0
jz ZERO
cmp al, 91
jl LOWER
ror rax, 8
jmp NEXT
LOWER:
add al, 32
ror rax, 8
jmp NEXT
ZERO:
mov al, 0
ror rax, 8
NEXT:
loop L1

mov rbx, [0x600000+8]
mov ecx, 8
L2:
cmp bl, 0
jz ZERO2
cmp bl, 91
jl LOWER2
ror rbx, 8
jmp NEXT2
LOWER2:
add bl, 32
ror rbx, 8
jmp NEXT2
ZERO2:
mov bl, 0
ror rbx, 8
NEXT2:
loop L2

mov [0x600010], rax
mov [0x600018], rbx
done:

dispbin
; divide by two will have overflow problem, so need to increase the register size to ebx
mov ecx, 16
mov ebx, 2
L1:
CDQ
cmp ax, 0
jz ZERO 
div ebx
cmp edx, 0 ; edx is remainder
jz ZERO
mov dl, 49
mov [0x600000+ecx-1], dl ; 1
jmp DO_NEXT
ZERO:
mov dl, 48
mov [0x600000+ecx-1], dl ; 0
DO_NEXT:
loop L1
done:

bubble
mov ecx, 9
l1:
mov dl, 4
mov eax, 9
sub eax, ecx
mul dl
mov ebx, [0x600000+eax]
mov edx, [0x600000+eax+4]
cmp ebx, edx
jg swap
jmp noswap
swap:
mov [0x600000+eax], edx
mov [0x600000+eax+4], ebx
noswap:
loop l1

mov ecx, 9
l2:
mov dl, 4
mov eax, 9
sub eax, ecx
mul dl
mov ebx, [0x600000+eax]
mov edx, [0x600000+eax+4]
cmp ebx, edx
jg swap2
jmp noswap2
swap2:
mov [0x600000+eax], edx
mov [0x600000+eax+4], ebx
noswap2:
loop l2

mov ecx, 7
l3:
mov dl, 4
mov eax, 7
sub eax, ecx
mul dl
mov ebx, [0x600000+eax]
mov edx, [0x600000+eax+4]
cmp ebx, edx
jg swap3
jmp noswap3
swap3:
mov [0x600000+eax], edx
mov [0x600000+eax+4], ebx
noswap3:
loop l3

mov ecx, 6
l4:
mov dl, 4
mov eax, 6
sub eax, ecx
mul dl
mov ebx, [0x600000+eax]
mov edx, [0x600000+eax+4]
cmp ebx, edx
jg swap4
jmp noswap4
swap4:
mov [0x600000+eax], edx
mov [0x600000+eax+4], ebx
noswap4:
loop l4

mov ecx, 5
l5:
mov dl, 4
mov eax, 5
sub eax, ecx
mul dl
mov ebx, [0x600000+eax]
mov edx, [0x600000+eax+4]
cmp ebx, edx
jg swap5
jmp noswap5
swap5:
mov [0x600000+eax], edx
mov [0x600000+eax+4], ebx
noswap5:
loop l5

mov ecx, 4
l6:
mov dl, 4
mov eax, 4
sub eax, ecx
mul dl
mov ebx, [0x600000+eax]
mov edx, [0x600000+eax+4]
cmp ebx, edx
jg swap6
jmp noswap6
swap6:
mov [0x600000+eax], edx
mov [0x600000+eax+4], ebx
noswap6:
loop l6

mov ecx, 3
l7:
mov dl, 4
mov eax, 3
sub eax, ecx
mul dl
mov ebx, [0x600000+eax]
mov edx, [0x600000+eax+4]
cmp ebx, edx
jg swap7
jmp noswap7
swap7:
mov [0x600000+eax], edx
mov [0x600000+eax+4], ebx
noswap7:
loop l7

mov ecx, 2
l8:
mov dl, 4
mov eax, 2
sub eax, ecx
mul dl
mov ebx, [0x600000+eax]
mov edx, [0x600000+eax+4]
cmp ebx, edx
jg swap8
jmp noswap8
swap8:
mov [0x600000+eax], edx
mov [0x600000+eax+4], ebx
noswap8:
loop l8

mov ebx, [0x600000]
mov edx, [0x600000+4]
cmp ebx, edx
jg swap9
jmp end
swap9:
mov [0x600000], edx
mov [0x600000+4], ebx
end:
done:

minicall 
call a
a:
mov eax, [esp]
done:

recur
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

