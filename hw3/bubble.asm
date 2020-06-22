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