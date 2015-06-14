#ifndef LEXICO_H
#define LEXICO_H
#include <stdio.h>
#include <ctype.h>
char Enviar(struct NodoLinhas *buffer, int *estado) // Retorna caractere armazenado na memoria,
                                // Manipula os ponteiros de posição de acordo
{

    char C;
    C=buffer->Linha[posColuna];
    if(C=='\0')
    {
        estado=66;
        return C;
    }
    while(C=='\n')
    {
        posColuna=0;
        posLinha++;
        C=buffer->Linha[posColuna];
    }
    posColuna++;
    return C;
}

char *AN_LEX(NodoLinhas *buffer)
{
    int estado=0;
    /*-- Estados:
     0 = LEX (Inicial)
     1 = 'q1' (R)
     2 = 'q2' (e)
     3 = 'q3' (a)
     4 = 'q4' (d)
     5 = 'Read' [FINAL]
     6 = 'q6' (P)
     7 = 'q7' (r)
     8 = 'q8' (i)
     9 = 'q9' (n)
     10 = 'q10' (t)
     11 = 'Print' [FINAL]
     12 = 'EVAR'
     13 = 'var' [FINAL]
     14 = 'q14' (i)
     15 = 'q15' (f)
     16 = 'if' [FINAL]
     17 = 'q17' (e)
     18 = 'q18' (l)
     19 = 'q19' (s)
     20 = 'q20' (e)
     21 = 'else' [FINAL]
     22 = '{' [FINAL]
     23 = '}' [FINAL]
     66 = Quebra laço
    */
    do
    {
        char Caractere=Enviar(buffer, *estado);
        switch(estado)
        {
            case 0:
            {
                switch(Caractere)
                {
                    case 'R': estado=1; break;
                    case 'P': estado=6; break;
                    case 'i': estado=14; break;
                    case 'e': estado=17; break;
                    case '{': estado=22; break;
                    case '}': estado=23; break;
                    case '_': estado=12; break;
                    default:
                        if(isblank(Caractere)) { estado=0; break; } // Se for espaço ou tabulação, fica no mesmo estado
                        if(isalpha(Caractere) || Caractere=='_') estado=12 else return "ERROR"; //Se for digito alphabetico vai para EVAR
                }
                break;
            }
            case 1:
            {
                switch(Caractere)
                {
                    case 'e': estado=2; break;
                    default:
                        if(isalnum(Caractere) || Caractere=='_') estado=12 else return "ERROR"; //Se for Digito ou Numero (alphanumerico) vai para EVAR
                }
                break;
            }
            case 2:
            {
                switch(Caractere)
                {
                    case 'a': estado=3; break;
                    default:
                        if(isalnum(Caractere) || Caractere=='_') estado=12 else return "ERROR"; //Se for Digito ou Numero vai para EVAR
                }
                break;
            }
            case 3:
            {
                switch(Caractere)
                {
                    case 'd': estado=3; break;
                    default:
                        if(isalnum(Caractere) || Caractere=='_') estado=12 else return "ERROR"; //Se for Digito ou Numero vai para EVAR
                }
                break;
            }
            case 4:
            {
                if(isalnum(Caractere) || Caractere=='_') //Se for Digito ou Numero vai para EVAR
                    estado=12;
                else
                    if(isblank(Caractere)) //Se for espaço retorna token
                    {
                        return "tokenREAD"; //estado = 5
                    }
                    else return "ERROR";
                break;
            }
            default: break;

            case 12:
            {
                switch(Caractere)
                {
                    case ';': return "tokenVAR"; //estado = 13
                    case ',': return "tokenVAR"; //estado = 13
                    default:
                        if(isalnum(Caractere) || Caractere=='_') //Se for Digito ou Numero vai para EVAR
                            estado=12;
                        else
                            if (isblank(Caractere)) return "tokenVAR" //Se for espaço, estado = 13
                            else return "ERROR";
                }

            }
        }
    }while(estado!=66);
    return "ERROR";
}
#endif // LEXICO_H
