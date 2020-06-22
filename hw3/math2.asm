mov eax, [0x600000]
neg eax
mov ebx, [0x600004]
imul ebx
shl rdx, 32
mov edx, eax
add rdx, [0x600008]
mov rax, rdx
done: