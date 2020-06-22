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