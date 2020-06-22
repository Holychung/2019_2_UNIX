; mul result is in edx:eax
add eax, [0x600000]
add eax, [0x600004]
mov ebx, [0x600008]
imul ebx
shl rdx, 32
mov edx, eax
mov [0x60000c], rdx
done: