/*
    Nome: Joao Vitor Pires Pinto - RA: 200782
    Nome: Leonardo Dimarchi Souza da Cruz - RA: 200109
    Nome: Luiz Fernando Toquetto - RA: 200359
*/

// 1ª Etapa: Completa.

//Bibliotecas
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//Estrutura: Informações do livro
struct info_livro{
    char sigla; // [L]ivre, [E]mprestado, [R]eservado
    int reg; // registro do livro
};

//Estrutura: Aluno
typedef struct aluno{
    char nome[80];
    char RA[7];
    int emprestado; // qtde de livros emprestados –> Max=3
    int reservado; // qtde de livros reservados –> Max =1
    struct info_livro tabela[4];
}aluno;

//Estrutura: Informações do aluno.
struct info_aluno{
    char sigla;
    char RA[7];
    int dia_ret; // Dia –> máx=31
    int mes_ret; // Mes = fev = 28 dias
    int dia_dev; // Mes = abril || jun || set || nov = 30 dias
    int mes_dev; // O resto é = 31 dias
};

//Estrutura: Informações do livro.
typedef struct livro{
    int reg; // Gerado automaticamente
    char titulo[80];
    char autor[80];
    struct info_aluno status[2];
} livro;

//Funções padrão.
void printar_menu();

//Funcões aluno.
void aloca_aluno(aluno **p);
void cadastro_aluno(aluno *p);
void escrever_arquivo_aluno(aluno *p);
void consulta_aluno(aluno *p, int opc);
void printar_aluno(aluno *p);
int buscar_ra(aluno *p, char *ra_colocado);
int verificar_qtd_alunos();

//Funções livro.
void aloca_livro(livro **p);
void cadastro_livro(livro *p);
void escrever_arquivo_livros(livro *p);
void consulta_livro(livro *p, int opc);
void printar_livro(livro *p);
int buscar_status(livro *p, char status_colocado);
int buscar_titulo(livro *p, char *titulo_colocado);
int verificar_qtd_livros();

//Main
main(){
    aluno *estudante = NULL;
    livro *book = NULL;
    int opc;

    aloca_aluno(&estudante);
    aloca_livro(&book);

    do{
        system("cls");

        printar_menu();
        scanf("%i",&opc);

        fflush(stdin);

        switch(opc){
            case 1:
                cadastro_aluno(estudante);
                break;
            case 2:
                cadastro_livro(book);
                break;
            case 3:
            case 4:
                consulta_aluno(estudante,opc);
                break;
            case 5:
            case 6:
            case 7:
                consulta_livro(book,opc); 
                break;
        }
    }while(opc != 8);
}

//---------INICIO -> FUNÇÕES LIVRO---------

//Printar um livro.
void printar_livro(livro *p){
    int f;

    printf("\nRegistro: %i",p->reg);
    printf("\n\tTitulo: %s",p->titulo);
    printf("\n\tAutor: %s\n",p->autor);

    for(f=0; f<2; f++){
        printf("\n\tStatus: %i",f);
        printf("\n\t\tSigla: %c",p->status[f].sigla);
        printf("\n\t\tRA: %s",p->status[f].RA);

        if(p->status[f].dia_ret == -1){
            printf("\n\t\tRetirado: ---");
            printf("\n\t\tDevolucao: ---");
        }else{
            printf("\n\t\tRetirado: %i/%i",p->status[f].dia_ret,p->status[f].mes_ret);
            printf("\n\t\tDevolucao: %i/%i",p->status[f].dia_dev,p->status[f].mes_dev);
        }

    }

    printf("\n-------------------------------------------\n");
}

//Consulta de livros (Opc 5 - 6 - 7).
void consulta_livro(livro *p, int opc){
    FILE *arquivo = NULL;
    int cc, posicao, qtdLivros = verificar_qtd_livros();
    char tituloAux[80], statusAux;

    system("cls");

    if((arquivo = fopen("livros.bin","rb")) == NULL){
        printf("\nFalha ao abrir o arquivo dos livros (Consulta).\n\n");
        system("PAUSE");
    }else{

        if(opc == 5){ //BUSCA TOTAL
            printf("\nLista de livros (Total)\n");
            printf("-------------------------------------------\n");

            for(cc=0; cc < qtdLivros; cc++){
                fseek(arquivo,cc*sizeof(livro),0);
                fread(p,sizeof(livro),1,arquivo);

                printar_livro(p);
            }
        
        }else if(opc == 6){  //BUSCA PELO STATUS
            printf("\nStatus do livro para buscar (L/E/R): ");
            scanf("%c",&statusAux);
            fflush(stdin);

            posicao = buscar_status(p,statusAux);

            if(posicao == -1){
                printf("\nStatus Invalido\n");
            }else{
                printar_livro(p);
            }
        }else if(opc == 7){ //busca pelo titulo
            printf("\nTitulo do livro para buscar: ");
            gets(tituloAux);
            fflush(stdin);

            posicao = buscar_titulo(p,tituloAux);

            if(posicao == -1){
                printf("\nTitulo Invalido\n");
            }else{
                printar_livro(p);
            }
        }

        printf("\n");
        system("PAUSE");

        fclose(arquivo);
    }
}

//Buscar livro através do TITULO.
int buscar_titulo(livro *p, char *titulo_colocado){
    FILE *arquivo = NULL;
    int cc, check = -1, qtd_livros = verificar_qtd_livros();

    if((arquivo = fopen("livros.bin","rb")) == NULL){
        printf("\nErro ao abrir o arquivo de livros (Busca pelo titulo)\n\n");
    }else{

        for(cc = 0; cc < qtd_livros; cc++){
            fseek(arquivo,cc*sizeof(livro),0);
            fread(p,sizeof(livro),1,arquivo);

            if(strcmp(p->titulo,titulo_colocado) == 0){
                check = cc;
                break;
            }
        }

        fclose(arquivo);
        return check;
    }
}

//Buscar livro através do STATUS.
int buscar_status(livro *p, char status_colocado){
    FILE *arquivo = NULL;
    int cc, f, check = -1, qtd_livros = verificar_qtd_livros();

    if((arquivo = fopen("livros.bin","rb")) == NULL){
        printf("\nErro ao abrir o arquivo de livros (Busca pelo status)\n\n");
    }else{

        for(cc = 0; cc < qtd_livros; cc++){
            fseek(arquivo,cc*sizeof(livro),0);
            fread(p,sizeof(livro),1,arquivo);

            for(f=0; f<2; f++){
                if(p->status[f].sigla == status_colocado){
                    check = cc;
                    cc = qtd_livros;
                }
            }
        }

        fclose(arquivo);
        return check;
    }
}

//Alocacao dinamica do livro.
void aloca_livro(livro **p){
    if((*p = (livro*) realloc(*p,sizeof(livro))) == NULL){
        printf("\nErro na alocacao dinamica do livro\n\n");
        exit(1);
    }
}

//Cadastro do livro.
void cadastro_livro(livro *p){
    int f, num_reg_livro=(verificar_qtd_livros()+1000);

    p->reg=num_reg_livro;
    printf("\nRegistro do livro: %i",p->reg);

    printf("\nInforme o nome do livro: ");
    gets(p->titulo);
    fflush(stdin);

    printf("Informe o nome do autor: ");
    gets(p->autor);
    fflush(stdin);

    for(f=0; f<2; f++){
        p->status[f].sigla='L';
        strcpy(p->status[f].RA,"---");
        p->status[f].mes_ret=-1;
        p->status[f].mes_dev=-1;
        p->status[f].dia_ret=-1;
        p->status[f].dia_dev=-1;
    }

    printf("\n");
    system("PAUSE");

    escrever_arquivo_livros(p);
}

//Escrever no arquivo de livros.
void escrever_arquivo_livros(livro *p){
    FILE *arquivo = NULL;

    if((arquivo = fopen("livros.bin","ab")) ==  NULL){
        printf("\n\nFalha ao abrir o arquivo dos livros (Escrita).\n\n");
        system("PAUSE");
    }else{
        fwrite(p,sizeof(livro),1,arquivo);
    }

    fclose(arquivo);
}

//Verificar a quantidade de livros no arquivo.
int verificar_qtd_livros(){
    FILE *arquivo = NULL;
    long int tamanho = 0;
    
    if((arquivo = fopen("livros.bin","rb")) == NULL){
        return tamanho;
    }else{
        fseek(arquivo,0,2);

        tamanho = ftell(arquivo)/sizeof(livro);

        fclose(arquivo);

        return tamanho;
    }
}
//---------FIM -> FUNÇÕES LIVRO---------

//---------INICIO -> FUNÇÕES ALUNO---------

//Cadastro do aluno.
void cadastro_aluno(aluno *p){
    int cc;

    printf("\nNome: ");
    gets(p->nome);
    fflush(stdin);

    printf("RA: ");
    gets(p->RA);
    fflush(stdin);

    p->emprestado = 0;
    p->reservado = 0;

    printf("\nQuantidade de livros emprestados: %i",p->emprestado);
    printf("\nQuantidade de livros reservados: %i\n\n",p->reservado);

    for(cc=0;cc<4;cc++){
        p->tabela[cc].sigla = 'L';
        p->tabela[cc].reg = -1;
    }   
    
    system("PAUSE");

    escrever_arquivo_aluno(p);
}

//Escrever no arquivo de alunos.
void escrever_arquivo_aluno(aluno *p){
    FILE *arquivo = NULL;

    if((arquivo = fopen("alunos.bin","ab")) ==  NULL){
        printf("\n\nFalha ao abrir o arquivo dos alunos (Escrita).\n\n");
        system("PAUSE");
    }else{
        fwrite(p,sizeof(aluno),1,arquivo);
    }

    fclose(arquivo);
}

//Consultar os alunos, tanto total, quanto parcial.
void consulta_aluno(aluno *p, int opc){
    FILE *arquivo = NULL;
    int cc, posicao_ra,qtdAlunos = verificar_qtd_alunos();
    char RA[7];

    system("cls");

    if((arquivo = fopen("alunos.bin","rb")) == NULL){
        printf("\nFalha ao abrir o arquivo dos alunos (Consulta).\n\n");
        system("PAUSE");
    }else{

        if(opc == 3){ //BUSCA TOTAL
            printf("\nLista de alunos (Total)\n");

            printf("-------------------------------------------\n");

            for(cc=0; cc < qtdAlunos; cc++){
                fseek(arquivo,cc*sizeof(aluno),0);
                fread(p,sizeof(aluno),1,arquivo);

                printar_aluno(p);
            }
        
        }else if(opc == 4){  //BUSCA PELO RA
            printf("\nRA para buscar: ");
            gets(RA);
            fflush(stdin);

            posicao_ra = buscar_ra(p, RA);

            if(posicao_ra == -1){
                printf("\nRA Invalido\n");
                system("PAUSE");
            }else{
                printar_aluno(p);
            }
        }

        printf("\n");
        system("PAUSE");

        fclose(arquivo);
    }
}

//Busca a posicao no arquivo atraves do RA.
int buscar_ra(aluno *p, char *ra_colocado){
    FILE *arquivo = NULL;
    int cc, check = -1, qtd_alunos = verificar_qtd_alunos();

    if((arquivo = fopen("alunos.bin","rb")) == NULL){
        printf("\nErro ao abrir o arquivo de alunos (Busca pelo RA)\n\n");
    }else{

        for(cc = 0; cc < qtd_alunos; cc++){
            fseek(arquivo,cc*sizeof(aluno),0);
            fread(p,sizeof(aluno),1,arquivo);

            if(strcmp(p->RA,ra_colocado) == 0){
                check = cc;
                break;
            }
        }

        fclose(arquivo);
        return check;
    }
}

//Verificar a quantidade de alunos no arquivo.
int verificar_qtd_alunos(){
    FILE *arquivo = NULL;
    long int tamanho = 0;
    
    if((arquivo = fopen("alunos.bin","rb")) == NULL){
        return tamanho;
    }else{
        fseek(arquivo,0,2);

        tamanho = ftell(arquivo)/sizeof(aluno);

        fclose(arquivo);

        return tamanho;
    }
}

//Printar apenas um aluno.
void printar_aluno(aluno *p){
    int f = 0;

    printf("\n\tNome: %s ",p->nome);
    printf("\n\tRA: %s",p->RA);
    printf("\n\tQTD Emprestado: %i",p->emprestado);
    printf("\n\tQTD Reservado: %i",p->reservado);

    for(f = 0; f < 4; f++){
        if(p->tabela[f].reg == -1){
            printf("\n\tTabela %i: %c -- ---",f, p->tabela[f].sigla);
        }else{
            printf("\n\tTabela %i: %c -- %i",f,p->tabela[f].sigla, p->tabela[f].reg);
        }
    }
    
    printf("\n\n-------------------------------------------\n");
}

//Alocacao dinamica do aluno
void aloca_aluno(aluno **p){
    if((*p = (aluno*) realloc(*p,sizeof(aluno))) == NULL){
        printf("\nErro na alocacao dinamica do aluno\n\n");
        exit(1);
    }
}
//---------FIM -> FUNÇÕES ALUNO---------

//---------INICIO -> FUNÇÕES PADRÃO---------
//Printar o menu
void printar_menu(){
    printf("\n<1> Cadastro do aluno");
    printf("\n<2> Cadastro de livro");

    printf("\n--------------------");

    printf("\n<3> Consultar alunos (Total)");
    printf("\n<4> Consultar aluno (RA)");

    printf("\n--------------------");
    
    printf("\n<5> Consultar livros (Total)");
    printf("\n<6> Consultar livros (Status)");
    printf("\n<7> Consultar livros (Titulo)");

    printf("\n--------------------");

    printf("\n<8> Sair");

    printf("\n--------------------");
    
    printf("\nOpcao: ");
}
//---------FIM -> FUNÇÕES PADRÃO---------