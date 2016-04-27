#include "stdio.h"
#include "stdlib.h"

int main()
{
    int i = 1;
    char b = *(char *)&i;
    if(b == 1) 
    printf("little endian\n");
    return 0;
}
#if 0
int main()
{
    union   union_data{
    int un_int;
    char un_char[4];
    };
    union union_data data;
    data.un_int = 0x000102;
    if(sizeof(int)==4)
    (data.un_char[0] == 2) ? printf ("little endian\n") : printf("big endian \n");
    else
        printf("...sizeof int = %d\n", sizeof(int));

    return 0;
}
#endif

#if 0
int main()
{
    union   union_data{
    int un_int;
    char *un_char;
    };
    union union_data *data = (union union_data *)malloc(sizeof(union union_data));        
    data->un_char = (char * )malloc(sizeof(int)*4);
    data->un_int = 0x000102;
    if(sizeof(int)==4)
    if    (data->un_char[0] == 2) 
        printf ("little endian\n") ;
    else
    printf("big endian \n");
    else
        printf("...sizeof int = %d\n", sizeof(int));

    return 0;
}
#endif 
#if 0
int main()
{
    union   union_data{
    int un_int;
    char *un_char;
    };
    union union_data data;        
    data.un_int = 0x000102;
    if(sizeof(int)==4)
    if    (data.un_char[0] == 2) 
        printf ("little endian\n") ;
    else
    printf("big endian \n");
    else
        printf("...sizeof int = %d\n", sizeof(int));

    return 0;
}
#endif
