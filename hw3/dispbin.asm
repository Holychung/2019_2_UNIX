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