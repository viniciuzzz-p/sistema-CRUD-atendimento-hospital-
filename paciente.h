#ifndef PROJETO2AJALMAR_PACIENTE_H
#define PROJETO2AJALMAR_PACIENTE_H

#include <stdio.h>

#define RG 30
#define CPF 30

typedef struct paciente {
    int status; //1 - Rascunho, 2 - Salvo.
    int livre;
    int indice;
    int codigo_paciente;
    char nome[255];
    char rg[RG];
    char cpf[CPF];
    char tipo_sanguineo[3];
    char fator_rh[2];
    char endereco[310];
    char data_nascimento[11];
    struct paciente *prox;
} Paciente;

enum {
    //Menu do paciente.
    inserir_paciente = 1,
    alterar_paciente = 2,
    excluir_paciente = 3,
    mostrar_dados_codigo = 4,
    mostrar_dados_tipo_sanguineo = 5,
    mostrar_pacientes_consultas = 6,
    mostrar_todos_pacientes = 7,
    salvar_alteracoes_pacientes = 8,
    sair_paciente = 9,

    //Tipo sanguíneo do paciente.
    nao_especificado = 0,
    O = 1,
    A = 2,
    B = 3,
    AB = 4,

    //Fator RH do paciente.
    positivo = 1,
    negativo = 2,

    //Status de controle dos dados do paciente.
    rascunho = 1,
    salvo = 2,
};

extern long int qtd_structs;
extern int indice;
extern Paciente *pacientes;

int abrir_arquivo();

int carregamento_dados();

void alocar_structs();

int ler_inteiro(char *texto);

int codigo_ja_existe(int codigo);

int codigo_paciente();

void alterar_codigo_paciente();

void limpar_buffer();

void ler_string(char *texto, char *str, int tam_max);

int validar_alpha(struct paciente *str);

void string_maiusculo(char *str);

void ler_nome();

void ler_RG();

int CPF_ja_existe(char *str);

void ler_CPF();

void ler_tipo_sanguineo();

void ler_fator_rh();

void tipo_sanguineo_paciente();

void  ler_endereco();

void ler_data_nascimento();

int buscar_indice_codigo(int codigo);

void alterar_pacientes();

void excluir_pacientes();

void mostrar_dados_base_codigo();

void mostrar_dados_base_tipo_sanguineo();

int check_int (int num, int *str, int tam);

void mostrar_pacientes_consulta();

void mostrar_pacientes_ordem_alfabetica();

void salvar_dados_paciente();

void desalocar_structs_paciente();

void fechar_aquivo_pacientes();

#endif //PROJETO2AJALMAR_PACIENTE_H