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