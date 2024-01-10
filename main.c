#include <stdio.h>
#include <stdlib.h>
#include "paciente.h"
#include "atendimento.h"

int main() {

    int flag;
    int valor = 0;

    //Abre o arquivo e lê os dados dos pacientes. Em caso de falha, encerra o programa.
    valor = carregamento_dados();
    if (valor) {
        return -2;
    }

    atendimento = upload_arquivo();
    alocar_inicial_atendimento();

    do {
        valor = -1;
        printf("########################MENU########################\n"
               "1 - Menu de paciente.\n"
               "2 - Menu de atendimento\n"
               "3 - Sair do programa.\n"
               "####################################################\n\n");
        valor = ler_inteiro("Selecione uma funcionalidade do menu de paciente:\n");
        printf("\n");

        switch (valor) {
            case 1:
                do {
                    valor = -1; //Evitar erro por lixo de memória.

                    printf("########################MENU DE PACIENTE########################\n");
                    printf("1 - Inserir paciente.\n"
                           "2 - Alterar dados do paciente.\n"
                           "3 - Excluir um paciente.\n"
                           "4 - Mostrar dados do paciente com base no codigo.\n"
                           "5 - Mostrar dados de pacientes que tenham tipo sanguineo.\n"
                           "6 - Mostrar dados de pacientes com consultas marcadas.\n"
                           "7 - Mostrar todos os pacientes.\n"
                           "8 - Salvar alteracoes nos dados do paciente.\n"
                           "9 - Sair do menu de paciente.\n");//provavelmente, já incluir o mostrar pacientes pela ordem alfabética com strcmp, provavelmente.
                    printf("################################################################\n\n");
                    valor = ler_inteiro("Selecione uma funcionalidade do menu de paciente:\n");
                    printf("\n");

                    switch (valor) {
                        case inserir_paciente:
                            alocar_structs();
                            codigo_paciente();
                            ler_nome();
                            ler_RG();
                            ler_CPF();
                            tipo_sanguineo_paciente();
                            ler_endereco();
                            ler_data_nascimento();
                            break;
                        case alterar_paciente:
                            alterar_pacientes();
                            break;
                        case excluir_paciente:
                            excluir_pacientes();
                            break;
                        case mostrar_dados_codigo:
                            mostrar_dados_base_codigo();
                            break;
                        case mostrar_dados_tipo_sanguineo:
                            mostrar_dados_base_tipo_sanguineo();
                            break;
                        case mostrar_pacientes_consultas:
                            mostrar_pacientes_consulta();
                            break;
                        case mostrar_todos_pacientes:
                            mostrar_pacientes_ordem_alfabetica();
                            break;
                        case salvar_alteracoes_pacientes:
                            salvar_dados_paciente();
                            break;
                        case sair_paciente:
                            flag = 1;
                            while (flag) {
                                valor = ler_inteiro("Deseja sair do menu paciente?\n"
                                                    "1 - Sim.\n"
                                                    "2 - Nao.\n");
                                if (valor == 1) {
                                    valor = 9;
                                    flag = 0;
                                    printf("\nSaindo do menu paciente...\n\n");
                                } else if (valor == 2) {
                                    flag = 0;
                                    printf("\nVoltando ao menu paciente...\n\n");
                                } else {
                                    printf("\nValor inserido invalido. Tente novamente.\n\n");
                                }
                            }
                            break;
                        default:
                            printf("Digite um valor valido.\n\n");
                    }
                } while (valor != sair_paciente);

                continue;

            case 2:
                do {
                    valor = -1;

                    printf("########################MENU DO ATENDIMENTO########################\n");
                    printf("1 - Inserir novo atendimento.\n"
                           "2 - Alterar atendimento\n"
                           "3 - Excluir atendimento\n"
                           "4 - Imprimir dados de um atendimento\n"
                           "5 - Imprimir dados do atendimento de um paciente\n"
                           "6 - Imprimir todos os atendimentos\n"
                           "7 - Imprimir pagamentos de um paciente\n"
                           "8 - Imprimir pagamentos de uma determinada data\n"
                           "9 - Imprimir pagamentos de um determinado periodo\n"
                           "10 - Salvar alteracoes dos atendimentos.\n"
                           "11 - Sair do menu de atendimento.\n");//provavelmente, já incluir o mostrar pacientes pela ordem alfabética com strcmp, provavelmente.
                    printf("###################################################################\n\n");
                    valor = ler_inteiro("Selecione uma funcionalidade no menu de atendimento:\n");
                    printf("\n");

                    switch (valor) {
                        case 1:
                            inserir_atendimentos();
                            break;
                        case 2:
                            alterar_atendimento();
                            break;
                        case 3:
                            excluir_atendimento();
                            break;
                        case 4:
                            mostrar_dados_codigo_atendimento();
                            break;
                        case 5:
                            mostrar_codigo();
                            break;
                        case 6:
                            mostrar_ordenado();
                            break;
                        case 7:
                            soma_paciente();
                            break;
                        case 8:
                            soma_data();
                            break;
                        case 9:
                            soma_periodo();
                            break;
                        case 10:
                            download();
                            break;
                        case 11:
                            flag = 1;
                            while (flag) {
                                valor = ler_inteiro("Deseja sair do menu atendimento?\n"
                                                    "1 - Sim.\n"
                                                    "2 - Nao.\n");
                                if (valor == 1) {
                                    valor = 11;
                                    flag = 0;
                                    printf("\nSaindo do menu atendimento...\n\n");
                                } else if (valor == 2) {
                                    flag = 0;
                                    printf("\nVoltando ao menu atendimento...\n\n");
                                } else {
                                    printf("\nValor inserido invalido. Tente novamente.\n\n");
                                }
                            }
                            break;
                        default:
                            printf("Digite um valor valido.\n\n");
                            break;
                    }
                } while (valor != 11);

                continue;

            case 3:
                flag = 1;
                while (flag) {
                    valor = ler_inteiro("Deseja encerrar o programa?\n"
                                        "1 - Sim.\n"
                                        "2 - Nao.\n");
                    if (valor == 1) {
                        valor = 3;  //Necessário para sair do do-while.
                        flag = 0;
                        desalocar_structs_paciente();
                        desalocar_structs_atendimento();
                        salvar_dados_paciente();    //Salva dados dos pacientes ao sair do programa.
                        download(); //Salva dados do atendimento ao sair do programa.
                        fechar_aquivo_pacientes();  //Fecha o arquivo dos pacientes.
                        fechar_aquivo_atendimentos();   //Fecha o arquivo dos atendimentos
                        printf("\nSaindo do programa...\n\n");
                    } else if (valor == 2) {
                        flag = 0;
                        printf("\nVoltando ao menu principal...\n\n");
                    } else {
                        printf("\nValor inserido invalido. Tente novamente.\n\n");
                    }
                }
                break;
            default:
                printf("Digite um valor valido.\n\n");
                break;
        }
    } while (valor != 3);

    return 0;
}