#include <stdio.h>
#define MEM_SIZE  10000
char Memory[MEM_SIZE];
int Frame_Pointer = 0;
int Stack_Pointer = 0;
int Temp = 0;
double Temp_F = 0.0;
int main() {
Stack_Pointer +=  13 ;
*(int *)&Memory[Stack_Pointer] = Frame_Pointer;
Stack_Pointer += 4;
Temp = Stack_Pointer;
*(int *)&Memory[Stack_Pointer] =  300 ;
Stack_Pointer += 4;
Frame_Pointer = Temp;
f1();
Stack_Pointer -= 4;
Stack_Pointer -= 4;
Frame_Pointer = *(int *)&Memory[Stack_Pointer];
*(int *)&Memory[Frame_Pointer + 0 ] = Temp;
*(int *)&Memory[Stack_Pointer] = Frame_Pointer;
Stack_Pointer += 4;
Temp = Stack_Pointer;
*(double *)&Memory[Stack_Pointer] =  1.800000 ;
Stack_Pointer += 8;
Frame_Pointer = Temp;
f2();
Stack_Pointer -= 8;
Stack_Pointer -= 4;
Frame_Pointer = *(int *)&Memory[Stack_Pointer];
*(double *)&Memory[Frame_Pointer + 4 ] = Temp_F;
*(int *)&Memory[Stack_Pointer] = Frame_Pointer;
Stack_Pointer += 4;
Temp = Stack_Pointer;
*(int *)&Memory[Stack_Pointer] =  3 ;
Stack_Pointer += 1;
Frame_Pointer = Temp;
f3();
Stack_Pointer -= 1;
Stack_Pointer -= 4;
Frame_Pointer = *(int *)&Memory[Stack_Pointer];
Memory[Frame_Pointer + 12 ] = Temp;
Temp = *(int *)&Memory[Frame_Pointer + 0 ];
printf("%d\n", Temp);
Temp_F = *(double *)&Memory[Frame_Pointer + 4 ];
printf("%6.4f\n", Temp_F);
Temp = Memory[Frame_Pointer + 12 ];
printf("%d\n", Temp);
Stack_Pointer -=  13 ;
return 0;
 }
int f1()
{
*(int *)&Memory[Frame_Pointer + 0 ] = *(int *)&Memory[Frame_Pointer + 0 ]+ 4 ;
Temp = *(int *)&Memory[Frame_Pointer + 0 ];
printf("%d\n", Temp);
Temp = *(int *)&Memory[Frame_Pointer + 0 ];
goto LABEL_2;
LABEL_2:
return 0;
}
int f2()
{
*(double *)&Memory[Frame_Pointer + 0 ] = *(double *)&Memory[Frame_Pointer + 0 ]+ 4.200000 ;
Temp_F = *(double *)&Memory[Frame_Pointer + 0 ];
printf("%6.4f\n", Temp_F);
Temp_F = *(double *)&Memory[Frame_Pointer + 0 ];
goto LABEL_3;
LABEL_3:
return 0;
}
int f3()
{
Memory[Frame_Pointer + 0 ] = Memory[Frame_Pointer + 0 ]+ 1 ;
Temp = Memory[Frame_Pointer + 0 ];
printf("%d\n", Temp);
Temp = Memory[Frame_Pointer + 0 ];
goto LABEL_4;
LABEL_4:
return 0;
}
