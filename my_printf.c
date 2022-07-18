#include "my_printf.h"
#include <stdio.h>

int my_putchar(char c)
{
    return write(1, &c, 1);
}

int my_putStr(char *c)
{
    int index = 0;
    char cc;
    if (c == NULL)
    {
        return write(1, "(null)", strlen("(null)"));
    }
    while (c[index] != 0)
    {   
        char cc =c[index];
        write(1, &cc, 1);
        index++;

    }
    return index;
}

char toLower(char a)
{
    if ( a >= 'A' && a <= 'Z')
    {
        a = a + 32;
    } 
    return a;
}


int my_printf( char *format, ...)
{
    va_list argsptr;  // argument list
    
    int index  = 0;   
    int output = 0;
    long int ln;
    unsigned int unsign;
   
    char c;
    char *str;

    va_start(argsptr, format);

    while(format[index] != '\0') //buffer not needed because printing done here
    {
        if(format[index] == '%') //will continue if %
        {
            index++;
            char change = toLower(format[index]); //change to lowercase

            switch (change) {
                case 'u':   //unsigned
                    unsign = va_arg(argsptr, unsigned int);
                    output += ItoaBase('u', unsign); //converts int 
                    break;

                case 'c': //char
                    c=(char)va_arg(argsptr, int);
                    write(1, &c, 1);
                    output++;
                    break;

                case 's':  //print string
                    str = va_arg(argsptr, char *);
                    output += my_putStr(str);
                    break;

                case 'p': // pointer to memory address
                    ln = (long int)va_arg(argsptr, void*);
                    if (ln == (long int)NULL)
                    {
                        my_putStr("nil");
                    }
                    my_putStr("0x");
                    output += 2;
                    output += ItoaBase('p', ln);
                    break;
                

                case '%' : // if this is part of the what is coming out, print it back
                    write(1, "%", 1);
                    output++;
                    continue;

                default: //illegal calls like %k will not  be processed
                    ln = va_arg(argsptr, long long);
                    output += ItoaBase(change, ln);
                    break;
            }
            index++;
            continue;

        }
        c = format[index];
        write(1, &c, 1);
        output++;
        index++;
       }
       va_end(argsptr);
       return output;


}

int ItoaBase(char base, ...)
{
    char *change;
    va_list elem;
    va_start (elem, base);
    int output;

    switch (base) 
    {
        case 'o': //octal
            change = convert(va_arg(elem, long long), 8, "01234567");
            
            break;
        
        case 'x': //hex
            change = convert(va_arg(elem, long long), 16, "0123456789abcdef");
            break;

        case 'd': // d int
            change = convert(va_arg(elem, int), 10, "0123456789");
            break;
        
        case 'u': //unsigned
            change = convert(va_arg(elem, long long), 10, "0123456789");
            break;

        case 'p': //pointer
            change = convert(va_arg(elem, long long), 16, "0123456789abcdef");
            break;
    }
            output = my_putStr(change);
            va_end(elem);
            return output;
}



char *convert(long long num, long long basenum, char *basestring )
{
    long long buffer = num;
    int count = 0;
    int ln = 0;
    int sign = 0;
    char* converted = NULL;

    if (num < 0) // if number is negative, multiply by negative to turn into positive
    {
        sign = -1;
        count++;
        num *= -1;
    }
    while (buffer != 0)
    {
        count++;
        buffer = buffer/basenum;
    }

    buffer = num;
    //memory allocation for converted string
    converted = (char*)malloc((count + 1)*sizeof(char)); 

    converted [count] = '\0';

    if (buffer == 0) {
        converted[count] = '0';
    }
    else {
        while(count >= 0)
        {
            count--;
            converted[count] = basestring[buffer % basenum];
            buffer = buffer/basenum;
        }
        if (sign < 0) {
            converted[0] = '-';
        }
    }
    return converted;
}
