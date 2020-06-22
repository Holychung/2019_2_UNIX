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