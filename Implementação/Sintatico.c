#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "Lexico.h" //MOVER as funções AN_LEX() e Enviar() para este header.

int TotalLinhas=0; // Numero total de linhas em armazenamento.
int posLinha=0; // em que Linha o analisador está.
int posColuna=0; // em que Coluna o analisador está, vai de 0 a 2000.

typedef struct NodoLinhas
{
    char Linha[2000]; //Caracteres armazenadaos na Linha
    int Tamanho;//Numero de caracteres na Linha.
    struct NodoLinhas *ProximaLinha;
} NodoLinhas;

char Enviar(NodoLinhas *buffer, int *estado) // Retorna caractere armazenado na memoria "buffer",
                                // Manipula os ponteiros de posição de acordo
{
    char C;
    printf("Linha atual: %d\nColuna atual: %d\n", posLinha, posColuna);
    C=buffer->Linha[posColuna]; //Lê o caractere na fita.

    if(C=='\n')
    {
        printf(" quebra linha \n");
        getchar();
        if(buffer->ProximaLinha!=NULL)
        {
            buffer=buffer->ProximaLinha; /*L-- Problema de ponteiro...
                                                Não está atualizando a linha do buffer fora desta função, que gera loop*/

            posLinha++; /*L -- Com esse bug, o buffer nunca avança na lista, mas
                                posLinhas é incrementada mesmo assim,
                                que dá impressão que existem infinitos nodos */
            posColuna=0;
        }
        printf("Debug: Retornando /n \n", C);
        posColuna++;
        return C;
    }
    posColuna++;

    printf("Debug: Retornando %c\n\n", C);
    return C;
}

char *AN_LEX(NodoLinhas *buffer)
{
    int estado=0;
    int *pt=&estado; //ponteiro para conteudo de estado
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
        char Caractere=Enviar(buffer, pt);
        switch(estado)
        {
            case 0:
            {
                printf("Debug: estado %d com char %c\n", estado, Caractere);
                switch(Caractere)
                {
                    case 'R': estado=1; break;
                    case 'P': estado=6; break;
                    case 'i': estado=14; break;
                    case 'e': estado=17; break;
                    case '{': estado=22; break;
                    case '}': estado=23; break;
                    case '_': estado=12; break;
                    case '\0': return "EOF";
                    case '\n': estado=0; break;
                        default:
                        if(isblank(Caractere)) { estado=0; break; }// Se for espaço ou tabulação, fica no mesmo estado
                        if(isalnum(Caractere) || Caractere=='_') estado=12;
                            else return "ERROR"; //Se for Digito, Numero ou espaço vai para EVAR
                }
                break;
            }
            case 1:
            {
                printf("Debug: estado 1 com char %c\n", Caractere);
                switch(Caractere)
                {
                    case 'e': estado=2; break;
                    default:
                        if(isalnum(Caractere) || Caractere=='_') estado=12;
                        else
                            if(isblank(Caractere)) return "tokenVAR"; //estado = 13
                            else return "ERROR"; //Se for Digito ou Numero vai para EVAR
                }
                break;
            }
            case 2:
            {
                printf("Debug: estado %d com char %c\n", estado, Caractere);
                switch(Caractere)
                {
                    case 'a': estado=3; break;
                    default:
                        if(isalnum(Caractere) || Caractere=='_') estado=12;
                        else
                            if(isblank(Caractere)) return "tokenVAR"; //estado = 13
                            else return "ERROR"; //Se for Digito ou Numero vai para EVAR
                }
                break;
            }
            case 3:
            {
                printf("Debug: estado %d com char %c\n", estado, Caractere);
                switch(Caractere)
                {
                    case 'd': estado=4; break;
                    default:
                        if(isalnum(Caractere) || Caractere=='_') estado=12;
                        else
                            if(isblank(Caractere)) return "tokenVAR"; //estado = 13
                            else return "ERROR"; //Se for Digito ou Numero vai para EVAR; //Se for Digito ou Numero vai para EVAR
                }
                break;
            }
            case 4:
            {
                printf("Debug: estado %d com char %c\n", estado, Caractere);
                if(isalnum(Caractere) || Caractere=='_') //Se for Digito ou Numero vai para EVAR
                    estado=12;
                else
                    if(isblank(Caractere)) //Se for espaço retorna token
                    {
                        return "tokenREAD"; //estado = 5
                    }
                    else return "BLANK ERROR";
                break;
            }
            default: break;

            case 12:
            {
                printf("Debug: estado %d com char %c\n", estado, Caractere);
                switch(Caractere)
                {
                    case ';': return "tokenVAR"; //estado = 13
                    case ',': return "tokenVAR"; //estado = 13
                    default:
                        if(isalnum(Caractere) || Caractere=='_') //Se for Digito ou Numero vai para EVAR
                            estado=12;
                        else
                            if (isblank(Caractere)) return "tokenVAR"; //Se for espaço, estado = 13
                            else return "BLANK ERROR";
                }

            }
        }

    }while(estado!=66);
    return "EOF";
}

/*L -- As funções acima devem ser colocadas em um .h! */
int Carregar_Arquivo(char *NomeArquivo, NodoLinhas *buffer);
int Inserir_Linha(char *linha, int tamanho, NodoLinhas *buffer); //Insere Linha na lista.
int Checar_Lista(NodoLinhas *buffer);
//int Remover_Quebra(char *entrada);
char PROG(NodoLinhas *buffer);

int main()
{
    NodoLinhas *Buffer;
    Buffer->ProximaLinha==NULL;
    char entrada[50];
    printf("nome arquivo:\n");
    scanf("%s", entrada);
    getchar();

    //fgets(entrada, 49, stdin);
    //Remover_Quebra(*entrada);
    Carregar_Arquivo(entrada, Buffer);
    Checar_Lista(Buffer);
    printf("ENTER para comecar a analise\n");
    getchar();
    PROG(Buffer);

    printf("fim\n");
    return 0;
}
char PROG(NodoLinhas *buffer)
{
    /* Função descartavel pra ver se tá lendo os tokens do arquivo read.txt ... */
    char Token[5];
    if(buffer->ProximaLinha!=NULL);
    {
        buffer=buffer->ProximaLinha; //primeira linha
    }

    while(strcmp(Token, "EOF"))
    {

        sprintf(Token, AN_LEX(buffer)); //Le proximo token
        printf("Token: %s\n", Token); //printa
        printf("-----Proximo token...\n");
        getchar();
    }
    /* Fim da funçãozinha */

    //L Haverão várias funções, correspondendo a cada variavel da gramática!
    /*L--Temos que fazer o Léxico primeiro!--*/

    /* -- Tokens possíveis:
       tokenPrint
       tokenRead
       tokenVar
       tokenAtrib
       tokenIf
       expvar
       explogic
       expnot
       expand
       expor
       expeq
       expimp
       else
       tokenAbreChaves
       tokenFechaChaves
       tokenAbreParenteses
       tokenFechaParenteses
       tokenPontoVirgula
       tokenVirgula --*/


    /*int Estado=0;
    switch(Estado)
    {
        case 0:
        {
            if(!Corpo()) //se Corpo() retornar 0 fecha com erro
            {

                return 0;
            }
            Estado=1;
        }
        default: if(strcmp(Token, "EOF")) return 1;
                 else return 0;
    }*/
}
Corpo()
{
    /*int Estado=0;
    char Token[10];
    switch(Estado)
    {
        case 0:
        { }
    }*/
}
int Carregar_Arquivo(char *NomeArquivo, NodoLinhas *buffer)
{
    char linha[2000];  // Armazena o conteudo da linha
    int TamanhoLinha; // Armazena o numero de caracteres da linha
    FILE *Arquivo;
    strncat(NomeArquivo, ".txt", 4); //Coloca ".txt" no nome de arquivo entrado

    //printf("DEBUG NomeArquivo: %s", NomeArquivo);
    if((Arquivo=fopen(NomeArquivo, "r+"))==NULL) // Se não conseguir abrir o arquivo, fecha programa
    {
        exit(1);
    }
    else
    {
        //printf("\nDEBUG: Arquivo Carregado\n ---- \n");
        while(!feof(Arquivo))
        {
            fgets(linha, 1999, Arquivo);
            //printf("DEBUG Linha: %s\n", linha);
            TamanhoLinha=strlen(linha);
            Inserir_Linha(linha, TamanhoLinha, buffer);
        }
        memset(linha,0,strlen(linha));
        linha[0]=EOF; //poem uma linha extra com sinal de EOF no struct para mostrar o final
        Inserir_Linha(linha, 0, buffer); //^
        fclose(Arquivo);
        return 0;
    }
}

int Inserir_Linha(char *linha, int tamanho, NodoLinhas *buffer)
{
    //printf("Inserir_Linha\n");
    NodoLinhas *LinhaNova=malloc(sizeof(NodoLinhas));
    if(buffer->ProximaLinha==NULL) // Se não houver um nodo após a cabeça, cria um e aponta a cabeça para ele.
    {
        //LINHA 1
        LinhaNova->Tamanho=tamanho;
        strcpy(LinhaNova->Linha, linha);
        LinhaNova->ProximaLinha=NULL;

        buffer->ProximaLinha=LinhaNova;
    }
    else // Percorre até achar o último, cria novo.
    {
        NodoLinhas *Varredor=buffer;
        while(Varredor->ProximaLinha!=NULL)
        {
            Varredor=Varredor->ProximaLinha;
        }
        LinhaNova->Tamanho=tamanho;
        strcpy(LinhaNova->Linha, linha);
        //printf("DEBUG LINHA NOVA em pos %d: %s\n", TotalLinhas+1, LinhaNova->Linha);
        LinhaNova->ProximaLinha=NULL;

        Varredor->ProximaLinha=LinhaNova;
        //printf("DEBUG LINHA NOVA em pos %d: %s\n", TotalLinhas+1, Varredor->ProximaLinha->Linha);
        TotalLinhas++;
    }
}

int Checar_Lista(NodoLinhas *buffer) //* L Funcao descartavel pra ver se a lista funciona.
{
    printf("\n\nArquivo em memoria:\n");
    NodoLinhas *Varredor=buffer;
    for(posLinha=0; posLinha<=TotalLinhas-1; posLinha++) //Começa da Linha 0 até o numero total de linhas menos a linha EOF
    {
        //printf("DebugTotalLinhas: %d\n", TotalLinhas);
        //printf("\n%d/%d\n", posLinha, TotalLinhas-1);
        if(Varredor->ProximaLinha!=NULL)
        {
            Varredor=Varredor->ProximaLinha;
            //printf("DEBUG: %s", Varredor->Linha);
            do
            {
                printf("%c", Varredor->Linha[posColuna]);
                posColuna++;
            }while(Varredor->Linha[posColuna]!='\n' && Varredor->Linha[posColuna]!='\0'); //Enquanto não quebrar a linha e não for EOF, imprime caractere
            printf("\n");
        }
        else
        {
            break;
        }
        posColuna=0; //Volta o ponteiro das colunas para 0
        //printf("Debug posColuna %d", posColuna);
    }
    posLinha=0;
    posColuna=0;
    printf("\n\n");
}

