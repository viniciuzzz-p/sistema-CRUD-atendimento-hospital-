#include "atendimento.h"
#include "paciente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *file;

Lista *atendimento;

void alocar_inicial_atendimento()
{
    if (atendimento == NULL) {
        atendimento = malloc(sizeof (Lista));
        atendimento->inicio = NULL;
    }
}

void inserir_atendimentos(){
    int codig;
    informacoes *aux, *novo_atendimento = malloc(sizeof (informacoes));
    printf("digite o codigo do paciente: \n");
    scanf("%d", &codig);
    int verificador = verificar_paciente(codig);
    if(verificador) {
        if (novo_atendimento) {
            //prrencher dados
            novo_atendimento->prox = NULL;
            tipo_atendimento(&novo_atendimento->tipo_atendimento);
            ler_preco(&novo_atendimento->preco);
            status_atendimento(novo_atendimento);
            ler_data(&novo_atendimento);
            codigo_atendimento(&novo_atendimento->codigo_atendimento);
            novo_atendimento->livre = 0;
            novo_atendimento->alterado = 0;
            novo_atendimento->salvo = 0;
            novo_atendimento->substituido=0;
            novo_atendimento->codigo_paciente_atendimento = codig;
            pegar_nome(novo_atendimento,codig);
            // novo_atendimento->codigo_atendimento = codigo;

            //é o primeiro?
            if (atendimento->inicio == NULL) {
                atendimento->inicio = novo_atendimento;
            } else {
                aux = atendimento->inicio;
                while (aux->prox != NULL) {
                    aux = aux->prox;
                }
                aux->prox = novo_atendimento;
            }

        } else
            printf("erro ao alocar memoria\n");
    }else {
        printf("codigo invalido!\n\n\n");
    }
}

void pegar_nome(informacoes *consulta, int code){
    Paciente *aux = pacientes;

    while (aux !=NULL){
        if(code == aux->codigo_paciente){
            strcpy(consulta->nome, aux->nome);
            break;
        }else{
            aux=aux->prox;
        }
    }
}

int verificar_paciente(int code){
    int flag =0;
    Paciente *aux = pacientes;

    while (aux != NULL){
        if(aux->codigo_paciente == code){
            printf("codigo valido!\n");
            flag =1;
            break;
        }else{
            aux = aux->prox;
        }
    }

    return flag;
}

void ler_preco(int *preco){
    int valor;
    char input[20];
    while (1) {
        printf("Informe o valor da consulta (ou deixe em branco se nao quiser informar.)\n");
        fflush(stdin);
        if(fgets(input, sizeof (input), stdin)!= NULL){
            if(input[0]== '\n'){
                printf("Valor nao informado.\n");
                *preco =0;
                break;
            } else if (sscanf(input, "%d", &valor)==1 && valor >=0){
                *preco = valor;
                break;
            }else{
                printf("Insira um valor numerico ou deixe em branco\n");

            }
        }else {
            printf("Erro na leitura.\n");
        }
    }
}

int status_atendimento(informacoes *consulta){
    int stts = 0;
//função que solicita o status do paciente, utilizando um loop infinito que so para se for selecionada uma das opções disponiveis

    while (1) {
        printf("Informe o status do atendimento\n");
        printf(" 1-Agendado\n 2-Espera\n 3-Em Atendimento\n 4-Atendido\n 5- Nao informar\n");
        fflush(stdin);
        if (scanf("%d", &stts) == 1){
            switch (stts) {
                case 1:
                    printf("Status: Agendado\n");
                    consulta->status = stts;
                    return stts;

                case 2:
                    printf("Status: Espera\n");
                    consulta->status= stts;
                    return stts;

                case 3:
                    printf("Status: Em Atendimento\n");
                    consulta->status = stts;
                    return stts;

                case 4:
                    printf("Status: Atendido\n");
                    consulta->status = stts;
                    return stts;

                case 5:
                    printf("Status nao informado\n");
                    consulta->status = stts;
                    return stts;

                default:
                    printf("Valor inserido invalido\n");
                    break;

            }
        }else {
            printf("Entrada invalida, por favor digite uma das opçoes disponiveis.\n");
            fflush(stdin);
        }
    }

}

void codigo_atendimento(int *codigo){
    int valor = 1; // Inicializa o código como 1 por padrão
    informacoes *aux = atendimento->inicio;

    while(aux != NULL){
        if(aux->codigo_atendimento == valor){
            // Se o código já existe na lista, incremente e reinicie a verificação
            valor++;
            aux = atendimento->inicio; // Reinicia a verificação na lista
        } else {
            aux = aux->prox;
        }
    }

    *codigo = valor;
    printf("codigo do atendimento: %03d\n", valor);
}


int verificar_codigo(int cod){
    int flag =0;
    informacoes *aux;

    aux = atendimento->inicio;
    while (aux->prox && aux->codigo_paciente_atendimento != cod){// AQUI É PRA SER CODIGO PACIENTE
        aux = aux->prox;
    }if(aux->prox->codigo_atendimento == cod){
        flag =1;
    }else{
        printf("codigo inexistente\n");
    }
    return flag;
}

void ler_data(informacoes **consulta){
    int dia = 0, mes = 0, ano = 0;

    printf("digite o dia do atendimento: \n");
    fflush(stdin);

    while(1){
        if(scanf("%d", &dia)==1 && dia >= 1 && dia <=31){
            fflush(stdin);
            (*consulta)->dia = dia;
            break;
        }else{
            printf("Dia invalido.\n\n");
            return;
        }
    }
    printf("digite o mes do atendimento: \n");
    fflush(stdin);

    while(1){
        if(scanf("%d", &mes)==1 && mes >= 1 && mes <=12){
            fflush(stdin);
            (*consulta)->mes = mes;
            break;
        }else{
            printf("Mes invalido.\n\n");
            return;
        }
    }
    printf("digite o ano do atendimento: \n");
    fflush(stdin);

    while(1){
        if(scanf("%d", &ano)==1 && ano >= 2000 && dia <=2024){
            fflush(stdin);
            (*consulta)->ano = ano;
            break;
        }else{
            printf("Ano invalido\n");
            return;
        }
    }
}

void imprimir(informacoes *consulta){

    while(consulta ) {
        if (consulta->livre == 0){
            printf("\n------------------------------------------------------------\n");
            printf("NOME DO PACIENTE\n");
            printf("codigo atendimento: %03d\n", consulta->codigo_atendimento);
            if (consulta->tipo_atendimento == 1) {
                printf("tipo de atendimento: consulta\n");
            } else {
                printf("tipo de atendimento: retorno\n");
            }
            printf("valor do atendimento: R$%d\n", consulta->preco);
            printf("data do atendimento: %02d/%02d/%04d\n", consulta->dia, consulta->mes, consulta->ano);
            switch (consulta->status) {
                case 1:
                    printf("status: agendado\n");
                    break;
                case 2:
                    printf("status: em espera\n");
                    break;
                case 3:
                    printf("status: em atendimento\n");
                    break;
                case 4:
                    printf("status: atendido\n");
                    break;
                case 5:
                    printf("status nao informado\n");
                    break;
                default:
                    printf("status nao encontrado\n");
                    break;

            }
            printf("\n------------------------------------------------------------\n");
        }
        consulta = consulta->prox;

    }
    printf("\n\n");
}
void tipo_atendimento(int *tipo){
    int tipe = 0;
    fflush(stdin);
    printf("1-consulta\n"
           "2-retorno\n"
           "selecione o tipo de atendimento: \n");
    while(1){
        if(scanf("%d", &tipe)==1){
            fflush(stdin);
            if (tipe == 0) {
                printf("Inserimento invalido.\n\n");
                return;
            }
            if (tipe==1 ||tipe ==2){
                *tipo = tipe;
                break;
            }else {
                printf("selecione uma opcao valida\n");
            }
        }else {
            printf("digite uma opcao valida (1 ou 2)\n");
            break;
        }
    }

}

void alterar_atendimento(){
    int codigo = 0;
    printf("digite o codigo do atendimento que deseja alterar: \n");
    scanf("%d", &codigo);
    fflush(stdin);

    if (codigo == 0) {
        printf("Codigo invalido.\n\n");
        return;
    }

    int verificador =0;
    verificador = verificar_code_atendimento(codigo);
    if(verificador==1) {
        informacoes *aux;
        aux = atendimento->inicio;
        while (aux->prox && aux->codigo_atendimento != codigo) {
            aux = aux->prox;
        }
        //achei em qual posição da lista esta o codigo
        int option;
        printf("o que deseja alterar?\n");
        printf("1- tipo de atendimento\n"
               "2- valor da atendimento\n"
               "3- status da atendimento\n"
               "4- data da atendimento\n"
               "5-retornar ao menu anterior\n");
        scanf("%d", &option);
        switch (option) {
            case 1:
                printf("tipo atendimento\n");
                if (aux->tipo_atendimento == 1) {
                    printf("tipo salvo: consulta\n");
                } else {
                    printf("tipo salvo: retorno\n");
                }
                tipo_atendimento(&aux->tipo_atendimento);
                aux->alterado = 1;
                aux->salvo =0;

                break;
            case 2:
                printf("valor da consulta\n");
                printf("valor salvo: R$%d\n", aux->preco);
                ler_preco(&aux->preco);
                aux->alterado = 1;
                aux->salvo =0;
                break;

            case 3:
                printf("status do atendimento\n");
                switch (aux->status) {
                    case 1:
                        printf("status: agendado\n");
                        break;

                    case 2:
                        printf("status: em espera\n");
                        break;

                    case 3:
                        printf("status: em atendimento\n");
                        break;
                    case 4:
                        printf("status: atendido\n");
                        break;

                    case 5:
                        printf("status nao informado\n");
                        break;
                    default:
                        printf("erro no status do atendimento\n\n\n");
                        break;
                }
                status_atendimento(&aux);
                aux->alterado = 1;
                aux->salvo =0;
                break;
            case 4:
                printf("data do atendimento\n");
                printf("data salva: %02d/%02d/%04d\n", aux->dia, aux->mes, aux->ano);
                ler_data(&aux);
                aux->alterado = 1;
                aux->salvo =0;
                break;

            case 5:
                printf("retornando ao menu anterior\n");
                break;

            default:
                printf("opcao invalida\n");
                break;


        }
    }else{
        printf("codigo invalido!\n");
    }
    // free(aux);
}
void excluir_atendimento(){
    int codigo = 0;
    printf("digite o codigo do atendimento que deseja excluir: \n");
    scanf("%d", &codigo);
    fflush(stdin);

    if (codigo == 0) {
        printf("Valor inserido invalido.\n\n");
        return;
    }

    Lista *aux2 = atendimento;
    int verificador = verificar_code_atendimento(codigo);
    if (verificador) {
        informacoes *aux;
        aux = atendimento->inicio;
        while (aux->prox && aux->codigo_atendimento != codigo) {
            aux = aux->prox;
        }
        int option;
        printf("nome: %s\n", aux->nome);
        printf("codigo atendimento: %d\n", aux->codigo_atendimento);
        printf("data: %02d/%02d/%04d\n", aux->dia, aux->mes, aux->ano);
        printf("voce realmente deseja excluir esse atendimento?\n");
        printf("1-sim\n2-nao\n");
        scanf("%d", &option);
        switch (option) {
            case 1:
                aux->livre = 1;
                aux->alterado = 1;
                aux->salvo =0;
                aux->status = 0;

                aux->preco = -1;
                aux->dia = 0;
                aux->mes = 0;
                aux->ano = 0;
                printf("excluido com sucesso!\n");
                break;
            case 2:
                break;
            default:
                printf("opcao invalida\n");
                break;
        }
    }
}

void mostrar_dados_codigo_atendimento() {
    int codigo;
    informacoes *aux = atendimento->inicio;
    printf("digite o codigo do atendimento para mostrar os dados\n");
    scanf("%d", &codigo);
    int verificador = verificar_code_atendimento(codigo);
    if (verificador) {
        while (aux) {

            if (aux->livre == 0 && aux->codigo_atendimento == codigo) {
                printf("\n------------------------------------------------------------\n");
                printf("NOME DO PACIENTE\n");
                printf("codigo atendimento: %03d\n", aux->codigo_atendimento);
                if ((atendimento->inicio)->tipo_atendimento == 1) {
                    printf("tipo de atendimento: consulta\n");
                } else {
                    printf("tipo de atendimento: retorno\n");
                }
                printf("valor do atendimento: R$%d\n", aux->preco);
                printf("data do atendimento: %02d/%02d/%04d\n", aux->dia, aux->mes, aux->ano);
                switch (aux->status) {
                    case 1:
                        printf("status: agendado\n");
                        break;
                    case 2:
                        printf("status: em espera\n");
                        break;
                    case 3:
                        printf("status: em atendimento\n");
                        break;
                    case 4:
                        printf("status: atendido\n");
                        break;
                    case 5:
                        printf("status nao informado\n");
                        break;
                    default:
                        printf("status nao encontrado\n");
                        break;

                }
                printf("\n------------------------------------------------------------\n");
            }
            aux = aux->prox;
        }
    }else{
        printf("codigo inexistente\n");
    }
}

int verificar_code_atendimento(int code){
    informacoes *aux;
    aux = atendimento->inicio;
    int flag =0;

    while(aux != NULL){
        if(aux->codigo_atendimento == code && aux->livre == 0){
            printf ("codigo verificado!\n");
            flag=1;
            break;
        }else{
            aux=aux->prox;
        }
    }
    return flag;
}

void mostrar_ordenado(){
    informacoes *aux;
    aux = atendimento->inicio;
    int count = 0;
    while(aux != NULL){
        count++;
        aux = aux->prox;
    }
    if(count == 0){
        printf("lista vazia!\n");
        return;
    }

    informacoes *vetor[count];
    // malloc (count * sizeof (informacoes ));
    aux = atendimento->inicio;
    for (int i = 0; i < count; ++i) {

        vetor[i] = aux;
        aux = aux->prox;
    }
    int indices [count];

    for (int k = 0; k < count; ++k) {
        indices[k] = k;
    }

    int sinal;
    int help;
    for (int i = 0; i < count-1; ++i) {
        for (int j = i+1; j < count; ++j) {
            sinal = mystrcmp(*vetor[indices[i]], *vetor[indices[j]]);
            if(sinal < 0){
                help = indices[i];
                indices[i] = indices[j];
                indices[j] = help;
            }
        }
    }

    //MOSTRAR OS DADOS

    for (int i = 0; i < count; ++i) {
        int index = indices[i];
        if(vetor[index]->livre==0) {
            printf("\n------------------------------------------------------------\n");

            printf("nome paciente: %s\n", vetor[index]->nome);
            printf("codigo do paciente: %d\n", vetor[index]->codigo_paciente_atendimento);
            printf("data do atendimento: %02d/%02d/%04d\n", vetor[index]->dia, vetor[index]->mes, vetor[index]->ano);
            printf("codigo atendimento: %03d\n", vetor[index]->codigo_atendimento);
            if (vetor[index]->tipo_atendimento == 1) {
                printf("tipo de atendimento: consulta\n");
            } else {
                printf("tipo de atendimento: retorno\n");
            }
            printf("valor da consulta: R$%d\n", vetor[index]->preco);
            switch (vetor[index]->status) {
                case 1:
                    printf("status atendimento: agendado\n");
                    break;
                case 2:
                    printf("status atendimento: em espera\n");
                    break;
                case 3:
                    printf("status atendimento: em atendimento\n");
                    break;
                case 4:
                    printf("status atendimento: atendido\n");
                    break;
                case 5:
                    printf("status nao informado\n");
                    break;
                default:
                    printf("status nao encontrado\n");
                    break;
            }
            printf("\n------------------------------------------------------------\n");
        }
        //printf("ERRO");
    }
}


int mystrcmp(informacoes data1, informacoes data2){

    if(data1.ano > data2.ano){
        return 1;
    }else if(data1.ano < data2.ano){
        return -1;
    }else if(data1.mes > data2.mes){
        return 1;
    }else if(data1.mes < data2.mes){
        return -1;
    }else if(data1.dia >data2.dia){
        return 1;
    }else if(data1.dia < data2.dia){
        return -1;
    }
    return 0;
}



void mostrar_codigo(){
    int codigo;
    informacoes *aux;
    printf("digite o codigo do paciente para mostrar os atendimentos:\n");
    scanf("%d", &codigo);
    int verificador = 0;
    Paciente *aux2 = pacientes;
    verificador = verificar_paciente(codigo);
    if (verificador ==1){
        aux = atendimento->inicio;
        while (aux!= NULL){
            if(aux->codigo_paciente_atendimento == codigo && aux->livre ==0){
                printf("\n------------------------------------------------------------\n");
                printf("nome do paciente: %s\n", aux->nome);
                printf("codigo paciente: %d\n", aux->codigo_paciente_atendimento);
                printf("codigo atendimento: %03d\n", aux->codigo_atendimento);
                if (aux->tipo_atendimento == 1) {
                    printf("tipo de atendimento: consulta\n");
                } else {
                    printf("tipo de atendimento: retorno\n");
                }
                printf("valor do atendimento: R$%d\n", aux->preco);
                printf("data do atendimento: %02d/%02d/%04d\n", aux->dia, aux->mes, aux->ano);
                switch (aux->status) {
                    case 1:
                        printf("status: agendado\n");
                        break;
                    case 2:
                        printf("status: em espera\n");
                        break;
                    case 3:
                        printf("status: em atendimento\n");
                        break;
                    case 4:
                        printf("status: atendido\n");
                        break;
                    case 5:
                        printf("status nao informado\n");
                        break;
                    default:
                        printf("status nao encontrado\n");
                        break;

                }
                aux=aux->prox;
                printf("\n------------------------------------------------------------\n");
            }else{
                aux=aux->prox;
            }
        }
    }else{
        printf("codigo invalido!\n");
    }

}

void soma_paciente(){
    long int soma =0;
    int codigo;
    int verificador;
    informacoes *aux;
    Paciente *aux2 = pacientes;

    printf("digite o codigo do paciente para ver a soma dos pagamentos: \n");
    scanf("%d", &codigo);
    verificador= verificar_paciente(codigo);
    if(verificador==1){
        aux = atendimento->inicio;
        printf("nome paciente: %s\n", aux->nome);
        while (aux!= NULL){
            if(aux->codigo_paciente_atendimento == codigo){
                printf("R$%d\n", aux->preco);
                soma += aux->preco;
                aux=aux->prox;
            }else{
                aux=aux->prox;
            }
        }
        printf("\nsoma dos pagamentos: R$%ld\n",soma);
    }else{
        printf("codigo invalido!\n");
    }
}



void download(){
    informacoes *aux;

    int dadosAlterados = 0;
    unsigned long long count = 0;
    int substuido=0;
    aux = atendimento->inicio;

    while (aux != NULL) {
        if (aux->alterado == 1) {
            if (aux->livre == 1) {
                // Atendimento foi excluído, busca um não salvo para substituir
                informacoes *aux2 = atendimento->inicio;
                while (aux2 != NULL) {
                    if (aux2->salvo == 0 && aux2->livre ==0) {
                        if(aux2->codigo_atendimento != aux->codigo_atendimento) {
                            aux2->salvo = 1;
                            aux2->alterado = 0;
                            fseek(file, count * sizeof(informacoes),SEEK_SET); // Move para a posição correta no arquivo
                            fwrite(aux2, sizeof(informacoes), 1, file);
                            dadosAlterados++;
                            substuido = 1;
                        }
                    }
                    aux2 = aux2->prox;
                }if (substuido ==0){

                    fseek(file, count * sizeof(informacoes), SEEK_SET); // Move para a posição correta no arquivo
                    fwrite(aux, sizeof(informacoes), 1, file);
                }

            } else {
                // Atendimento foi alterado, atualiza no arquivo

                if(aux->salvo == 0) {
                    aux->alterado = 0;
                    aux->salvo = 1;
                    fseek(file, count * sizeof(informacoes), SEEK_SET); // Move para a posição correta no arquivo
                    fwrite(aux, sizeof(informacoes), 1, file);
                    dadosAlterados++;
                }
            }
        }
        count++;
        aux = aux->prox;
    }

    // Adiciona ao final do arquivo os não salvos
    aux = atendimento->inicio;
    while (aux != NULL) {
        if (aux->salvo == 0) {
            if(aux->substituido == 0) {
                aux->salvo = 1;
                fseek(file, 0, SEEK_END); // Move para o final do arquivo
                fwrite(aux, sizeof(informacoes), 1, file);
                dadosAlterados++;
            }
        }
        aux = aux->prox;
    }
}

Lista* upload_arquivo() {
    Lista *consulta = (Lista *)malloc(sizeof(Lista));
    consulta->inicio = NULL;
    informacoes temp;

    file = fopen("atendimento.bin", "rb+");
    if (file == NULL) {
        file = fopen("atendimento.bin", "wb+");
        if (file == NULL) {
            printf("\nErro ao abrir o arquivo\n");
            free(consulta);
            return NULL;
        }
    }

    while (fread(&temp, sizeof(informacoes), 1, file) == 1) {
        informacoes *novoAtendimento = (informacoes *)malloc(sizeof(informacoes));
        memcpy(novoAtendimento, &temp, sizeof(informacoes));
        novoAtendimento->prox = NULL;

        // Insere o novo atendimento na lista encadeada
        if (consulta->inicio == NULL) {
            consulta->inicio = novoAtendimento;
        } else {
            informacoes *aux = consulta->inicio;
            while (aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = novoAtendimento;
        }
    }

    return consulta;
}

void soma_periodo(){
    informacoes *data_inicial, *data_final;
    data_inicial = malloc(sizeof(informacoes));
    data_final = malloc(sizeof(informacoes));
    long int soma = 0;
    int sinal, sinal2;
    printf("digite a data inicial para ver as somas:\n");
    ler_data(&data_inicial);
    printf("digite a data final do periodo:\n");
    ler_data(&data_final);
    informacoes *aux = atendimento->inicio;
    while(aux!= NULL){
        sinal = mystrcmp(*data_inicial, *aux);
        sinal2 = mystrcmp(*data_final, *aux);
        if(sinal <= 0 && sinal2 >=0){
            printf("R$%d\n", aux->preco);
            soma+= aux->preco;
        }
        aux = aux->prox;
    }
    printf("soma dos pagamentos no periodo solicitado:\n\n R$%ld\n", soma);
}

void desalocar_structs_atendimento()
{
    informacoes *aux = atendimento->inicio;
    informacoes *aux2;

    while (aux != NULL) {
        aux2 = aux;

        if (aux2 != NULL) {
            aux = aux2->prox;
            free(aux2);
        }
    }

    free(aux);
}

void soma_data()
{
    long int soma = 0;
    informacoes *aux = atendimento->inicio;
    informacoes *data;
    data = malloc(sizeof (informacoes ));
    while(1) {
        printf("digite o dia que deseja ver as somas\n");
        int day;
        fflush(stdin);
        scanf("%d", &day);
        fflush(stdin);
        if (day>=1 && day <=31 ){
            data->dia = day;
            break;
        }else{
            printf("digite uma data valida\n");
        }
    }
    while(1) {
        printf("digite o mes que deseja ver as somas\n");
        int month;
        fflush(stdin);
        scanf("%d", &month);
        fflush(stdin);
        if (month>=1 && month <=12  ){
            data->mes = month;
            break;
        }else{
            printf("digite uma mes valido\n");
        }
    }
    while(1) {
        printf("digite o ano que deseja ver as somas\n");
        int year;
        fflush(stdin);
        scanf("%d", &year);
        fflush(stdin);
        if (year>=2000 && year <=2024  ){
            data->ano = year;
            break;
        }else{
            printf("digite uma ano valido\n");
        }
    }
    while(aux != NULL){
        int sinal = mystrcmp(*data, *aux);
        if (sinal == 0){
            printf ("R$%d", aux->preco);
            soma += aux->preco;
        }
        aux = aux->prox;
    }
    printf("soma dos pagamentos do dia: %ld\n", soma);
}

void fechar_aquivo_atendimentos()
{
    printf("Arquivo de atendimentos fechado.\n\n");
    fclose(file);
}