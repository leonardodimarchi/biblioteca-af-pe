/*
    CP116TIM2 - Programação Estruturada (Matutino)

    Nome: Joao Vitor Pires Pinto - RA: 200782
    Nome: Leonardo Dimarchi Souza da Cruz - RA: 200109
    Nome: Luiz Fernando Toquetto - RA: 200359
*/

// 1 - Etapa (Cadastro e Gravação dos alunos e dos livros): Completa.
// 2 - Etapa (Fazer rotina para emprÃ©stimo/reserva): Completa.
// 3 - Etapa (Devolução completa): Fazendo.

/* 3.1 - Etapa (Ponto extra/Deletar livro e arrumar 
rotina do cadastro livros - *opcional*): A fazer?.*/

//Bibliotecas
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>

//Estrutura: Informacoes do livro
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

//Estrutura: Informacoes do aluno.
struct info_aluno{
    char sigla;
    char RA[7];
    int dia_ret; // Dia –> max=31
    int mes_ret; // Mes = fev = 28 dias
    int dia_dev; // Mes = abril || jun || set || nov = 30 dias
    int mes_dev; // O resto é = 31 dias
};

//Estrutura: Informacoes do livro.
typedef struct livro{
    int reg; // Gerado automaticamente
    char titulo[80];
    char autor[80];
    struct info_aluno status[2];
} livro;

//Funcoes padrao.
void emprestimo_reserva(aluno *pAluno, livro *pLivro);
void efetivar_emprestimo_reserva( aluno *pAluno,livro *pLivro,int posicao_ra);
void alterar_status_emprestimo(aluno *pAluno, livro *pLivro, char opc, int cc);
void devolucao_livro(aluno *pAluno, livro *pLivro);
void multar(int auxDia, int auxMes, livro *pLivro);
void printar_menu();


//Funcoes aluno.
void aloca_aluno(aluno **p);
void cadastro_aluno(aluno *p);
void escrever_arquivo_aluno(aluno *p);
void consulta_aluno(aluno *p, int opc);
void printar_aluno(aluno *p);
int buscar_ra(aluno *p, char *ra_colocado);
int atualizar_aluno(aluno *pAluno,int posicao_ra);
int verificar_qtd_alunos();
        
//Funcoes livro.
void aloca_livro(livro **p);
void cadastro_livro(livro *p);
void escrever_arquivo_livros(livro *p);
void consulta_livro(livro *p, int opc);
void printar_livro(livro *p);
int buscar_status(livro *p, char status_colocado);
int buscar_titulo(livro *p, char *titulo_colocado);
int atualizar_livro(livro *pTitulo,int posicao_titulo);
int verificar_reservas(livro *p);
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

        //Menu
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
            case 8:
                emprestimo_reserva(estudante, book);
                break;
            case 9:
                devolucao_livro(estudante, book);
        }
    }while(opc != 0);
}

//---------INICIO -> FUNCOES LIVRO---------

//Printar um livro.
void printar_livro(livro *p){
    int f;

    printf("\nRegistro: %i",p->reg);
    printf("\n\tTitulo: %s",p->titulo);
    printf("\n\tAutor: %s\n",p->autor);

    for(f=0; f<2; f++){
        printf("\n\t%i\xA7 Status: ",f+1);

        if((p->status+f)->sigla == 'L'){
            printf("Disponivel");
        }else{
            if((p->status+f)->sigla == 'E'){
                printf("Emprestado");
            }else{
                printf("Reservado");
            }
            
            printf("\n\t\tRA: %s",(p->status+f)->RA);

            if((p->status+f)->dia_ret == -1){
                printf("\n\t\tRetirado: ---");
                printf("\n\t\tDevolucao: ---");
            }else{
                printf("\n\t\tRetirado: %i/%i",(p->status+f)->dia_ret,(p->status+f)->mes_ret);
                printf("\n\t\tDevolucao: %i/%i",(p->status+f)->dia_dev,(p->status+f)->mes_dev);
            }
        }
    }

    printf("\n-------------------------------------------\n");
}

//Consulta de livros (Opc 5 - 6 - 7)
void consulta_livro(livro *p, int opc){
    FILE *arquivo = NULL;
    int cc, posicao, qtdLivros = verificar_qtd_livros();
    char tituloAux[80], statusAux;

    system("cls");

    if((arquivo = fopen("livros.bin","rb")) == NULL){
        printf("\nFalha ao abrir o arquivo dos livros (Consulta).\n\t-> Possivelmente nao ha livros cadastrados!\n\n");
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

            buscar_status(p,statusAux);

        }else if(opc == 7){ //BUSCA PELO TITULO
            printf("\nTitulo do livro para buscar: ");
            gets(tituloAux);
            fflush(stdin);

            posicao = buscar_titulo(p,tituloAux);

            if(posicao == -1){
                printf("\nTitulo Invalido. Tente novamente!\n");
            }else{
                printar_livro(p);
            }
        }

        printf("\n");
        system("PAUSE");

        fclose(arquivo);
    }
}

//Buscar livro atraves do TITULO.
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

//Buscar livro atraves do STATUS.
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
                if((p->status+f)->sigla == toupper(status_colocado)){     
                    printar_livro(p);
                    check = 1;   /// SUSPEEEEITO  --> CHECK = CC ?
                    break;
                }
            }
        }

        if (check == -1){
            printf("\nStatus Invalido. Tente novamente!\n");
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
        (p->status+f)->sigla='L';
        strcpy((p->status+f)->RA,"---");
        (p->status+f)->mes_ret=-1;
        (p->status+f)->mes_dev=-1;
        (p->status+f)->dia_ret=-1;
        (p->status+f)->dia_dev=-1;
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

//Atualiza o arquivo de livros.
int atualizar_livro(livro *pLivro,int posicao_titulo){
    FILE *arquivo = NULL;

    if((arquivo = fopen("livros.bin","rb+")) == NULL){
        printf("\nFalha ao abrir o arquivo de livros (Atualizar o arquivo)\n\n");
        system("PAUSE");
    }else{
        fseek(arquivo,posicao_titulo*sizeof(livro),0);
        fwrite(pLivro,sizeof(livro),1,arquivo);
    }

    fclose(arquivo);
}

//Retorna 1 caso nao tenha reservas
int verificar_reservas(livro *p){
    int cc;

    for ( cc = 0; cc < 2; cc++){
        if((p->status+cc)->sigla == 'R'){
            return 0;
        }
    }

    return 1;
    
}

//---------FIM -> FUNCOES LIVRO---------

//---------INICIO -> FUNCOES ALUNO---------

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

    for(cc=0;cc<4;cc++){
        (p->tabela+cc)->sigla = 'L';
        (p->tabela+cc)->reg = -1;
    }   
    
    printf("\n");

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
        printf("\nFalha ao abrir o arquivo dos alunos (Consulta).\n\t-> Possivelmente nao ha alunos cadastrados!\n\n");
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
                printf("\nRA Invalido. Tente novamente!\n");
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

    //Printar os emprestimos e reservas apenas se forem > 0.
    if(p->emprestado > 0){
        printf("\n\tQTD Emprestado: %i",p->emprestado);
    }
    
    if(p->reservado > 0){
        printf("\n\tQTD Reservado: %i",p->reservado);
    }
    

    for(f = 0; f < 4; f++){
        if((p->tabela+f)->reg != -1){
            printf("\n\tTabela %i: %c -- %i",f,(p->tabela+f)->sigla, (p->tabela+f)->reg);
        }
    }
    
    printf("\n\n-------------------------------------------\n");
}

//Atualiza o arquivo de alunos.
int atualizar_aluno(aluno *pAluno,int posicao_ra){
    FILE *arquivo = NULL;

    if((arquivo = fopen("alunos.bin","rb+")) == NULL){
        printf("\nFalha ao abrir o arquivo de alunos (Atualizar o arquivo)\n\n");
        system("PAUSE");
    }else{
        fseek(arquivo,posicao_ra*sizeof(aluno),0);
        fwrite(pAluno,sizeof(aluno),1,arquivo);
    }
    
    fclose(arquivo);
}

//Alocacao dinamica do aluno
void aloca_aluno(aluno **p){
    if((*p = (aluno*) realloc(*p,sizeof(aluno))) == NULL){
        printf("\nErro na alocacao dinamica do aluno\n\n");
        exit(1);
    }
}
//---------FIM -> FUNCOES ALUNO---------

//---------INICIO -> FUNCOES PADRAO---------

void devolucao_livro(aluno *pAluno, livro *pLivro){
    char auxRa[7], auxTitulo[80];
    int posicao_ra, posicao_titulo, auxDia, auxMes, maxDias;

    system("cls");

    printf("\nRA do aluno: ");
    gets(auxRa);
    fflush(stdin);

    posicao_ra = buscar_ra(pAluno,auxRa);

    if(posicao_ra == -1){
        printf("\nRA Invalido. Tente novamente!\n");
        system("PAUSE");
        return;
    }

    printf("\nTitulo do livro a ser devolvido: ");
    gets(auxTitulo);
    fflush(stdin);

    posicao_titulo = buscar_titulo(pLivro,auxTitulo);

    if(posicao_titulo == -1){
        printf("\nTitulo do livro invalido!\n");
        system("PAUSE");
        return;
    }

    do{
        system("cls");

        printf("\nData da devolucao (Atual)");

        printf("\n\tDia: ");
        scanf("%i",&auxDia);
        fflush(stdin);

        printf("\tMes: ");
        scanf("%i",&auxMes);
        fflush(stdin);

        maxDias = 31;
        if(auxMes == 2) maxDias = 28;
        if(auxMes == 4 || auxMes == 6 || auxMes == 8 || auxMes == 11) maxDias = 30;

        if(auxDia <= 0 || auxDia > maxDias || auxMes <= 0 || auxMes > 12 || auxMes < (pLivro->status+0)->mes_dev){
            printf("\nData invalida. Tente novamente\n\n");
            system("PAUSE");
        }

    }while(auxDia <= 0 || auxDia > maxDias || auxMes <= 0 || auxMes > 12 || auxMes < (pLivro->status+0)->mes_dev);
    
    if( (pLivro->status+0)->sigla == 'E' && strcmp((pLivro->status+0)->RA, auxRa) == 0){
        multar(auxDia, auxMes, pLivro);
    }
}

//Multar
void multar(int auxDias, int auxMes, livro *pLivro){
    int cc,auxDia, auxMes, maxDias,auxDiaMulta, auxMesMulta, auxMulta = 0, multa = 0;

    auxDiaMulta = auxDia - (pLivro->status+0)->dia_dev;
    auxMesMulta = auxMes - (pLivro->status+0)->mes_dev;

    auxMes = (pLivro->status+0)->mes_dev;

    maxDias = 31;
    if(auxMes == 2) maxDias = 28;
    if(auxMes == 4 || auxMes == 6 || auxMes == 8 || auxMes == 11) maxDias = 30;

    if(auxMesMulta == 0){
        auxMulta = auxDiaMulta;
    }else{
        if(auxMesMulta == 1){
            auxMulta = auxDia + (maxDias - (pLivro->status+0)->dia_dev);
        }else{
            auxMulta = auxDia + (maxDias - (pLivro->status+0)->dia_dev);

            for(cc = 1; cc <= auxMesMulta; cc++){
                auxMes++;

                maxDias = 31;
                if(auxMes == 2) maxDias = 28;
                if(auxMes == 4 || auxMes == 6 || auxMes == 8 || auxMes == 11) maxDias = 30;

                auxMulta += maxDias;                
            }

            auxMulta -= maxDias;
        }
    }

    for(cc = 0; cc < auxMulta; cc++){
        multa += 3;
    }

    printf("\nValor da multa por atraso = %i\n\n",multa);
    system("PAUSE");
}

//Emprestimo e Reserva de livros
void emprestimo_reserva(aluno *pAluno, livro *pLivro){
    char auxRa[7];
    int posicao_ra;

    system("cls");

    printf("\nRA do aluno: ");
    gets(auxRa);
    fflush(stdin);

    posicao_ra = buscar_ra(pAluno,auxRa);

    if(posicao_ra == -1){
        printf("\nRA Invalido. Tente novamente!\n");
        system("PAUSE");
    }else{
        if(pAluno->emprestado < 3 || pAluno->reservado < 1){
            efetivar_emprestimo_reserva(pAluno, pLivro, posicao_ra);
        }else{
            printf("\nO aluno ja esta com o limite de emprestimo e reservas.\n\n");
            system("PAUSE");
        }
    }
}

//Fazer o emprestimo e reserva propriamente dito.
void efetivar_emprestimo_reserva( aluno *pAluno,livro *pLivro,int posicao_ra){
    char auxTitulo[80];
    int posicao_titulo,cc,check = 0;

    printf("\nTitulo do livro: ");
    gets(auxTitulo);
    fflush(stdin);

    posicao_titulo = buscar_titulo(pLivro,auxTitulo);

    if(posicao_titulo == -1){
        printf("\nTitulo do livro invalido!\n");
        system("PAUSE");
    }else{

        for(cc=0; cc<2; cc++){
            //Livre -> emprestado.
            if((pLivro->status+cc)->sigla == 'L'){
                alterar_status_emprestimo(pAluno, pLivro, 'E', cc);
                
                break;
            }else{
                //Reserva
                if((pLivro->status+cc)->sigla == 'E' && verificar_reservas(pLivro) == 1){
                    alterar_status_emprestimo(pAluno, pLivro, 'R', 1);
                    break;
                }else if(verificar_reservas(pLivro) != 1){
                    printf("\nInfelizmente o livro ja esta reservado.\n");
                    printf("\nRA de Emprestimo: %s",(pLivro->status+cc)->RA);
                    printf("\nData devolucao: %i/%i\n\n",(pLivro->status+cc)->dia_dev,(pLivro->status+cc)->mes_dev);

                    check = 1;
                    break;
                }
            }
        }

        atualizar_livro(pLivro, posicao_titulo);
        atualizar_aluno(pAluno, posicao_ra);
        
        if(check != 1){
            printar_aluno(pAluno);
            printar_livro(pLivro);
        }
        
        system("PAUSE");
    }
}

//Atualizar as informacoes do livro e aluno
void alterar_status_emprestimo(aluno *pAluno, livro *pLivro, char opc, int cc){
    int f,auxDia, auxMes, maxDias;

    (pLivro->status+cc)->sigla = opc;

    strcpy((pLivro->status+cc)->RA, pAluno->RA);

    

    if(opc != 'R'){

        do{
            printf("\nMes: "); 
            scanf("%i",&auxMes); 
        }while(auxMes < 1 || auxMes > 12);

        maxDias = 31;
        if(auxMes == 2) maxDias = 28;
        if(auxMes == 4 || auxMes == 6 || auxMes == 8 || auxMes == 11) maxDias = 30;
          
        do{
            printf("\nDia: ");
            scanf("%i",&auxDia);
        }while(auxDia > maxDias);
        
        (pLivro->status+cc)->dia_ret = auxDia;
        (pLivro->status+cc)->mes_ret = auxMes;


        pAluno->emprestado++;
    }else{
        pAluno->reservado++;
        (pLivro->status+cc)->dia_ret = ((pLivro->status+0)->dia_dev) + 1;
        (pLivro->status+cc)->mes_ret = (pLivro->status+0)->mes_dev;  

        maxDias = 31;
        if((pLivro->status+cc)->mes_ret == 2) maxDias = 28;
        if((pLivro->status+cc)->mes_ret == 4 || (pLivro->status+cc)->mes_ret == 6 || (pLivro->status+cc)->mes_ret == 8 || (pLivro->status+cc)->mes_ret == 11) maxDias = 30;  
    }
    
    //Data de devolução
    (pLivro->status+cc)->dia_dev = ((pLivro->status+cc)->dia_ret) + 7;

    if((pLivro->status+cc)->dia_dev > maxDias){ //Se passar do maximo de dias do mês
            if((pLivro->status+cc)->mes_ret  == 12){
                (pLivro->status+cc)->mes_dev = 1;
            }else{
                (pLivro->status+cc)->mes_dev = (pLivro->status+cc)->mes_ret + 1;
            }

            (pLivro->status+cc)->dia_dev =  (pLivro->status+cc)->dia_dev - maxDias;
    }else{
        (pLivro->status+cc)->mes_dev = (pLivro->status+cc)->mes_ret;   
    }
         
    //Aluno
    for(f = 0; f<4; f++){
        if((pAluno->tabela+f)->sigla == 'L'){
            (pAluno->tabela+f)->sigla = opc;
            (pAluno->tabela+f)->reg = pLivro->reg;
            break;
        }
    }

}

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

    printf("\n<8> Emprestimo / Reserva");
    printf("\n<9> Devolucao");

    printf("\n--------------------");

    printf("\n<0> Sair");

    printf("\n--------------------");
    
    printf("\nOpcao: ");
}

//---------FIM -> FUNCOES PADRAO---------