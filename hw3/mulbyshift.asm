; mul result is in edx:eax
mov eax, [0x600000]
mov ebx, 26
imul ebx
shl rdx, 32
mov edx, eax
mov [0x600004], rdx
done:
