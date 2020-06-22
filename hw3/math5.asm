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
