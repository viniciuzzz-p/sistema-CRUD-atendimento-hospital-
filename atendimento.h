#ifndef PROJETO2AJALMAR_ATENDIMENTO_H
#define PROJETO2AJALMAR_ATENDIMENTO_H

typedef struct atendimentos{
    int codigo_atendimento;
    int substituido;
    int codigo_paciente;
    int preco;
    int status;
    int tipo_atendimento;
    int codigo_paciente_atendimento;
    int dia;
    int mes;
    int ano;
    int salvo;
    int alterado;
    int livre;
    char nome[255];
    struct atendimentos *prox;

}informacoes;

typedef struct Lista{
    informacoes *inicio;
}Lista;

extern Lista *atendimento;

void alocar_inicial_atendimento();

void tipo_atendimento(int *tipo);

void inserir_atendimentos();

void codigo_atendimento(int *codigo);

int status_atendimento(informacoes *consulta);

void ler_preco(int *preco);

int verificar_codigo(int cod);

void ler_data(informacoes **consulta);

void imprimir(informacoes *consulta);

void alterar_atendimento();

void excluir_atendimento();

void mostrar_dados_codigo_atendimento();

int verificar_code_atendimento(int code);

void mostrar_ordenado();

int mystrcmp(informacoes data1, informacoes data2 );

int verificar_paciente(int code);

void pegar_nome(informacoes *consulta, int code);

void mostrar_codigo();

void soma_paciente();

void download();

void soma_periodo();

void soma_data();

Lista* upload_arquivo();

void desalocar_structs_atendimento();

void fechar_aquivo_atendimentos();

#endif //PROJETO2AJALMAR_ATENDIMENTO_H
