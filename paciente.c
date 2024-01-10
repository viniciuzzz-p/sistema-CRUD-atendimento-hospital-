#include "paciente.h"
#include "atendimento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

long int qtd_structs;
int indice = -1;
FILE *dados_pacientes = NULL;
Paciente *pacientes = NULL;
Paciente *ultima_struct = NULL;

//Tenta abrir um arquivo se existir. Se não, cria um arquivo novo. Caso contrário, notifica erro e encerra o programa.
int abrir_arquivo()
{
    int flag;

    dados_pacientes = fopen("dados.bin", "rb+"); //Tenta abrir o arquivo.

    //Arquivo não existe.
    if (dados_pacientes == NULL) {
        dados_pacientes = fopen("dados.bin", "wb+");

        //Arquivo não pode ser criado.
        if(dados_pacientes == NULL) {
            flag = 1;
        } else {
            flag = 0;
            printf("Novo arquivo criado com sucesso.\n\n");
        }

    } else {
        flag = 0;
    }

    return flag;
}

//Lê os dados do arquivo para a memória.
int carregamento_dados()
{
    pacientes = malloc(sizeof(Paciente));
    Paciente *aux = pacientes;

    if (abrir_arquivo(dados_pacientes)) {
        printf("ERRO: Não foi possível abrir o banco de dados dos pacientes.\n\n");
        free(dados_pacientes);
        return 1;
    } else {
        fseek(dados_pacientes, 0 , SEEK_END);
        qtd_structs = ftell(dados_pacientes) / sizeof(struct paciente);

        if (!qtd_structs) {
            pacientes = NULL;
            return 0;
        } else {
            //Procura individualmente a struct no arquivo e lê pro ponteiro no endereço correto.
            for (int i = 0; i < qtd_structs; i++) {
                fseek(dados_pacientes, sizeof(Paciente) * i, SEEK_SET);
                fread(aux, sizeof(Paciente), 1, dados_pacientes);
                if(i != qtd_structs - 1) {
                    aux->prox = malloc(sizeof(Paciente));
                    aux = aux->prox;
                } else {
                    aux->prox = NULL;
                    ultima_struct = aux;
                }
            }
        }
        printf("Dados dos pacientes carregados.\n\n");
    }
    return 0;
}

void alocar_structs()
{
    Paciente *aux = pacientes;
    int valor = 1;

    //Se houver um espaço logicamente livre ao longo da lista, não há necessidade de fazer malloc no final da lista.
    while (aux != NULL)
    {
        if (aux->livre) {
            valor = 0;
            break;
        }

        aux = aux->prox;
    }

    if (pacientes == NULL) {
        pacientes = malloc(sizeof(struct paciente));
        pacientes->prox = NULL;
        pacientes->livre = 1;
        pacientes->status = rascunho;
        ultima_struct = pacientes;
        qtd_structs++;
    } else {
        if (valor) {
            ultima_struct->prox = malloc(sizeof(struct paciente));
            ultima_struct = ultima_struct->prox;
            ultima_struct->prox = NULL;
            ultima_struct->status = rascunho;
            ultima_struct->livre = 1;
            qtd_structs++;
        }
    }
}

int ler_inteiro(char *texto)
{
    int valor = 0;

    printf("%s", texto);
    fflush(stdin);
    scanf("%d", &valor);
    fflush(stdin);

    //limpar_buffer(); (utilizar apenas no linux.)

    return valor;
}

int codigo_ja_existe(int codigo)
{
    Paciente *aux = pacientes;

    while (aux != NULL)
    {
        if(aux->livre == 0 && aux->codigo_paciente == codigo) {
            return 1;
        }
        aux = aux->prox;
    }

    return 0;
}

int codigo_paciente()
{
    Paciente *aux = pacientes;
    int i = 0;

    int codigo = 0; //Evitar erro por lixo de memória.

    while (1) {
        codigo = ler_inteiro("Insira um numero inteiro positivo para o codigo do paciente:\n");

        if (codigo_ja_existe(codigo)) {
            printf("O codigo ja existe. Por favor, escolha um valor inteiro diferente.\n\n");
        } else if (codigo <= 0) {
            printf("Deve-se inserir um numero inteiro positivo.\n\n");
        }   //Se o usuário colocar uma letra ou um valor não positivo.
        else
            break;
    }

    while (aux != NULL) {
        if (aux->livre) {
            aux->codigo_paciente = codigo;
            printf("Codigo do paciente: %03d.\n\n", aux->codigo_paciente);
            aux->livre = 0;  //Linha do paciente reservada.
            aux->indice = i;
            indice = i; //'indice' será usado para acessar a posição da struct na lista encadeada.
            break;
        }

        i++;
        aux = aux->prox;
    }

    return 0;
}

void alterar_codigo_paciente()
{
    Paciente *aux = pacientes;
    int codigo;

    while (aux != NULL) {
        if (!(aux->livre) && aux->indice == indice) {
            while (1) {
                codigo = ler_inteiro("Insira o novo codigo:\n");

                if (codigo_ja_existe(codigo)) {
                    printf("O codigo ja existe. Por favor, escolha um valor inteiro diferente.\n\n");
                }   //Se o código já existir em uma das structs da lista.
                else if (codigo <= 0) {
                    printf("Deve-se inserir um numero inteiro positivo.\n\n");
                }   //Se o usuário colocar uma letra ou um valor não positivo.
                else
                    break;
            }

            aux->codigo_paciente = codigo;
            break;
        }

        aux = aux->prox;
    }
}

//Função utilizada para limpar o buffer em LINUX.
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Imprime uma mensagem e lê uma string para dentro de um vetor especificado.
void ler_string(char *texto, char *str, int tam_max)
{
    printf("%s", texto);
    fflush(stdin);
    fgets(str, tam_max, stdin);
    fflush(stdin);

    // Verifica se o último caractere é um '\n' e substitui ele por '\0'
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
//    } else {
//        limpar_buffer(); usado apenas em sistema Linux para limpar buffer de entrada, pois o fflush(stdin) não funciona.
    }
}

int validar_alpha(struct paciente *str) {
    int num = 1;

    for (int i = 0; str->nome[i] != '\0'; ++i) {
        if (!isalpha(str->nome[i]) && str->nome[i] != ' ') {

            num = 0;
            return num;
        }
    }

    return num;
}

void string_maiusculo(char *str)
{
    int i = 0;
    while (str[i] != '\0') {
        str[i] = toupper(str[i]);
        i++;
    }
}

//Imprime uma mensagem e lê o nome do paciente.
void ler_nome()
{
    Paciente *aux = pacientes;
    int valor;

    while (aux != NULL) {
        if (!(aux->livre) && aux->indice == indice) {
            do {
                valor = 0;
                ler_string("Insira o nome do paciente:\n", aux->nome, 245);

                if (!strcmp(aux->nome, "")) {
                    printf("Nome em branco. Por favor, insira um nome.\n\n");
                    continue;
                }

                if(!validar_alpha(aux)) {
                    printf("Caractere invalido. Insira apenas letras alfabeticas.\n\n");
                } else {
                    valor = 1;
                }

            } while (!valor);

            string_maiusculo(aux->nome);

            printf("Nome: %s\n\n", aux->nome);
        }
        aux = aux->prox;
    }
}

void ler_RG() {
    Paciente *aux = pacientes;
    int valor = 0;

    while (aux != NULL) {
        if (!(aux->livre) && aux->indice == indice) {
            while (!valor) {
                ler_string("Insira o RG do paciente com todos os numeros presentes no documento (campo opcional):\n",
                           aux->rg,
                           21); //Lê o RG do paciente.

                if (aux->rg[0] == '\0') {
                    printf("RG nao inserido.\n\n");
                    return;
                }

                for (int i = 0; aux->rg[i] != '\0'; ++i) {
                    valor = isdigit(aux->rg[i]);
                    if (!valor) {
                        printf("Caracteres invalidos. Use apenas numeros decimais.\n\n");
                        break;
                    }
                }
            }
            printf("RG do paciente: %s\n\n", aux->rg);
            break;
        }
        aux = aux->prox;
    }
}

int CPF_ja_existe(char *str)
{
    Paciente *aux = pacientes;

    while (aux != NULL) {
        if (!(aux->livre) && !strcmp(str, aux->cpf) && aux->indice != indice) {
            return 0;   //O CPF já existe.
        }

        aux = aux->prox;
    }
    return 1;   //O CPF é único.
}

void ler_CPF()
{
    Paciente *aux = pacientes;
    int valor = 0;
    char string[CPF];

    while (aux != NULL) {
        if (!(aux->livre) && aux->indice == indice) {
            while (!valor) {
                ler_string("Insira o CPF do paciente com todos os numeros presentes no documento:\n", string, 12);

                //Confere se o CPF inserido está em branco.
                if (!strcmp(string, "")) {
                    printf("CPF em branco. Por favor, insira um CPF valido.\n\n");
                    continue;
                }

                //Confere se todos os caracteres são dígitos até o '\0'.
                for (int i = 0; string[i] != '\0'; ++i) {
                    valor = isdigit(string[i]);
                    if (!valor) {
                        printf("Caracteres invalidos. Use apenas numeros decimais.\n\n");
                        break;
                    }
                }
                if (!valor) {
                    continue;
                }

                //Confere se CPF já existe na lista.
                valor = CPF_ja_existe(string);
                if(!valor) {
                    printf("CPF ja cadastrado. Tente novamente.\n\n");
                }
            }

            strcpy(aux->cpf,  string);
            printf("CPF do paciente: %s.\n\n", aux->cpf);
            break;

        }

        aux = aux->prox;
    }
}

void ler_tipo_sanguineo()
{
    Paciente *aux = pacientes;
    int opc;

    while (aux != NULL) {
        if (!(aux->livre) && aux->indice == indice) {
            printf("Tipo sanguineo:\n"
                   "0 - Nao especificar.\n"
                   "1 - Tipo O;\n"
                   "2 - Tipo A;\n"
                   "3 - Tipo B;\n"
                   "4 - Tipo AB;\n");   //Menu do tipo sanguíneo.

            do {
                opc = ler_inteiro("Selecione o tipo sanguineo do paciente:\n");
                switch (opc) {
                    case nao_especificado:
                        strcpy(aux->tipo_sanguineo, "");
                        strcpy(aux->fator_rh,"");   //Se não houver tipo sanguíneo, não será necessário inserir o fator RH.
                        break;
                    case O:
                        strcpy(aux->tipo_sanguineo, "O");
                        break;
                    case A:
                        strcpy(aux->tipo_sanguineo, "A");
                        break;
                    case B:
                        strcpy(aux->tipo_sanguineo, "B");
                        break;
                    case AB:
                        strcpy(aux->tipo_sanguineo, "AB");
                        break;
                    default:
                        printf("Tipo sanguineo invalido.\n\n");
                        break;
                }
            } while (opc != nao_especificado && opc != O && opc != A && opc != B &&
                     opc != AB);    //Continua no loop se não entrar em nenhum case.


            if (!strcmp(aux->tipo_sanguineo, "")) {
                printf("Tipo sanguineo e fator RH nao especificado.\n\n");
            } else {
                printf("Tipo sanguineo: %s\n\n", aux->tipo_sanguineo);
            }

            break;
        }

        aux = aux->prox;
    }
}

//Para essa função funcionar, é necessário que ler_tipo_sanguineo() seja executado antes, pois o Fator RH do paciente depende do tipo sanguíneo dele.
void ler_fator_rh()
{
    Paciente *aux = pacientes;
    int opc;

    while (aux != NULL) {
        //Entra no if se também tipo_sanguineo[indice] não for vazio.
        if (!(aux->livre) && aux->indice == indice && strcmp(aux->tipo_sanguineo, "") != 0) {
            //Menu do fator RH.
            printf("Fator RH:\n"
                   "1 - Positivo (+);\n"
                   "2 - Negativo (-);\n");

            do {
                opc = ler_inteiro("Insira o fator RH do paciente:\n");
                switch (opc) {
                    case positivo:
                        strcpy(aux->fator_rh, "+");
                        break;
                    case negativo:
                        strcpy(aux->fator_rh, "-");
                        break;
                    default:
                        printf("Fator RH invalido.\n\n");
                        break;
                }
            } while (opc != positivo && opc != negativo);   //Continua no loop se não entrar em nenhum case.

            printf("Fator RH: %s\n\n", aux->fator_rh);
            break;
        }

        aux = aux->prox;
    }
}

void tipo_sanguineo_paciente()
{
    Paciente *aux = pacientes;

    ler_tipo_sanguineo();
    ler_fator_rh();

    while(aux != NULL) {
        //Se os dois tiverem strings vazias, não entra no if. Caso contrário, entra.
        if (!(aux->livre) && aux->indice == indice && strcmp(aux->tipo_sanguineo, aux->fator_rh) != 0) {
            printf("Tipo sanguineo do paciente: %s%s\n\n", aux->tipo_sanguineo, aux->fator_rh);
            break;
        }

        aux = aux->prox;
    }
}

//Lê cada parte do endereço do paciente, juntando e armazenando tudo em apenas uma string no endereco[indice].
void  ler_endereco()
{
    Paciente *aux = pacientes;
    char cidade[100];
    char bairro[100];
    char rua[100];
    char numero[10]; //Ex.: 1234B

    while (aux != NULL) {
        if (!(aux->livre) && aux->indice == indice) {
            ler_string("Insira a cidade do paciente (campo opcional):\n", cidade, 100);   //Cidade inserida.
            string_maiusculo(cidade);
            ler_string("Insira o bairro do paciente (campo opcional):\n", bairro, 100);   //Bairro inserido.
            string_maiusculo(bairro);
            ler_string("Insira a rua do paciente (campo opcional):\n", rua, 100); //Rua inserida.
            string_maiusculo(rua);
            ler_string("Insira o numero da residencia do paciente (campo opcional):\n", numero, 10); //Número da residência inserido.
            string_maiusculo(numero);    //Deixa a letra maiúscula caso coloque alguma letra no número.

            //Coloca a string com as partes do endereço do paciente nas formatações '%s' dentro da variável endereco do paciente.
            sprintf(aux->endereco,
                    "Cidade: %s\nBairro: %s\nRua: %s\nNumero da residencia: %s",
                    cidade, bairro, rua, numero);

            printf("\nEndereco do paciente:\n%s\n\n", aux->endereco);
            break;
        }

        aux = aux->prox;
    }
}

int validar_data(int dia, int mes, int ano)
{
    if (ano < 1900 || ano > 2050) {
        printf("Ano invalido. Insira um ano na faixa de 1900 ate 2050\n\n");
        return 0;
    } else if (mes < 1 || mes > 12){
        printf("Mes invalido. Insira um mes valido\n\n");
        return 0;
    } else if (dia < 1 || dia > 31) {
        printf("Dia invalido. Insira um dia valido\n\n");
        return 0;
    } else {
        printf("Data inserida com sucesso.\n\n");
        return 1;
    }
}

void ler_data_nascimento()
{
    Paciente *aux = pacientes;
    char data[255];
    int dia, mes, ano, valor;

    while (aux != NULL) {
        if(!(aux->livre) && aux->indice == indice) {
            do {
                dia = 0, mes = 0, ano = 0;

                ler_string("Insira a data de nascimento do paciente no formato (DD/MM/AAAA):\n", data, 250);

                sscanf(data, "%d/%d/%d", &dia, &mes, &ano);

                valor = validar_data(dia, mes, ano);


            } while (!valor);

            sprintf(aux->data_nascimento, "%02d/%02d/%d", dia, mes, ano);
            printf("Data de nascimento: %s\n\n", aux->data_nascimento);
            break;
        }

        aux = aux->prox;
    }
}

int buscar_indice_codigo(int codigo)
{
    Paciente *aux = pacientes;
    int i = 0;
    int index = -1;

    while (aux != NULL) {
        if (!(aux->livre) && codigo == aux->codigo_paciente) {
            index = i;
            break;
        }

        i++;
        aux = aux->prox;
    }

    return index;
}

void alterar_pacientes()
{
    Paciente *aux = pacientes;
    int num;
    int codigo;

    codigo = ler_inteiro("Insira o codigo do paciente que deseja alterar:\n");
    indice = buscar_indice_codigo(codigo);

    if (indice == -1) {
        printf("Paciente nao encontrado.\n\n");
    } else {
        while(aux != NULL) {
            if (!(aux->livre) && aux->indice == indice) {

                printf("\n####################DADOS DO PACIENTE####################\n"
                       "Codigo: %d\nNome: %s\nRG: %s\nCPF: %s\nTipo sanguineo: %s\nFator RH: %s\n###########Endereco###########\n%s\n##############################\nData de nascimento: %s\n"
                       "#########################################################\n\n",
                       aux->codigo_paciente, aux->nome, aux->rg, aux->cpf,
                       aux->tipo_sanguineo, aux->fator_rh,
                       aux->endereco, aux->data_nascimento);

                printf("Escolha o que deseja alterar no paciente:\n");
                printf("1 - Codigo.\n2 - Nome.\n3 - RG.\n4 - CPF.\n5 - Tipo sanguineo.\n6 - Endereco.\n7 - Data de nascimento.\n8 - Cancelar.\n\n");

                num = ler_inteiro("Insira o dado que deseja alterar no paciente:\n");
                if (num == 1) {
                    alterar_codigo_paciente();
                    aux->status = rascunho;
                    printf("Dado alterado com sucesso.\n\n");
                } else if (num == 2) {
                    ler_nome();
                    aux->status = rascunho;
                    printf("Dado alterado com sucesso.\n\n");
                } else if (num == 3) {
                    ler_RG();
                    aux->status = rascunho;
                    printf("Dado alterado com sucesso.\n\n");
                } else if (num == 4) {
                    ler_CPF();
                    aux->status = rascunho;
                    printf("Dado alterado com sucesso.\n\n");
                } else if (num == 5) {
                    tipo_sanguineo_paciente();
                    aux->status = rascunho;
                    printf("Dado alterado com sucesso.\n\n");
                } else if (num == 6) {
                    ler_endereco();
                    aux->status = rascunho;
                    printf("Dado alterado com sucesso.\n\n");
                } else if (num == 7) {
                    ler_data_nascimento();
                    aux->status = rascunho;
                    printf("Dado alterado com sucesso.\n\n");
                } else if (num == 8) {
                    printf("Voltando ao menu paciente...\n\n");
                } else {
                    printf("Valor inserido invalido.\n\n");
                }

                break;
            }

            aux = aux->prox;
        }
    }
}

void excluir_pacientes()
{
    Paciente *aux = pacientes;
    informacoes *aux2 = atendimento->inicio;
    int codigo;
    int valor;

    codigo = ler_inteiro("Insira o codigo do paciente que deseja excluir:\n");
    indice = buscar_indice_codigo(codigo);

    if (indice == -1) {
        printf("Paciente nao encontrado.\n\n");
    } else {
        while (aux != NULL) {
            if (!(aux->livre) && aux->indice == indice) {
                printf("\n####################DADOS DO PACIENTE####################\n"
                       "Codigo: %d\nNome: %s\nRG: %s\nCPF: %s\nTipo sanguineo: %s\nFator RH: %s\n###########Endereco###########\n%s\n##############################\nData de nascimento: %s\n"
                       "#########################################################\n\n",
                       aux->codigo_paciente, aux->nome, aux->rg, aux->cpf,
                       aux->tipo_sanguineo, aux->fator_rh,
                       aux->endereco, aux->data_nascimento);

                while (1) {
                    valor = ler_inteiro("Deseja realmente excluir o paciente? Seus atendimentos e dados pessoais serao apagados do sistema.\n\n"
                                        "1 - Sim.\n"
                                        "2 - Nao.\n");

                    if (valor != 1 && valor != 2) {
                        printf("Valor inserido invalido. Tente novamente.\n\n");
                    } else if (valor == 2) {
                        printf("Operacao cancelada. Voltando ao menu paciente...\n\n");
                        break;
                    } else {
                        aux->livre = 1;
                        aux->status = rascunho;

                        //Caso o paciente seja excluído, todas as consultas relacionadas a ele também tem que ser excluído.
                        if (indice != -1) {
                            while (aux2 != NULL) {

                                if (aux2->codigo_paciente_atendimento ==  codigo && !(aux2->livre)) {
                                    aux2->livre = 1;
                                    aux2->alterado = 1;
                                    aux2->salvo = 0;
                                    aux2->status = 0;

                                    aux2->preco = -1;
                                    aux2->dia = 0;
                                    aux2->mes = 0;
                                    aux2->ano = 0;
                                }

                                aux2 = aux2->prox;
                            }
                        }

                        printf("Paciente excluido.\n\n");
                        break;
                    }
                }

                break;
            }

            aux = aux->prox;
        }
    }
}

void mostrar_dados_base_codigo()
{
    Paciente *aux = pacientes;
    int codigo;

    codigo = ler_inteiro("Insira o codigo do paciente que deseja mostrar os dados:\n");
    indice = buscar_indice_codigo(codigo); //Busca o índice do código inserido. Se não existir, índice possui o valor -1.

    if (indice == -1) {
        printf("Paciente nao encontrado.\n\n");
    } else {
        while (aux != NULL) {
            if (!(aux->livre) && aux->indice == indice) {
                printf("\n####################DADOS DO PACIENTE####################\n"
                       "Codigo: %d\nNome: %s\nRG: %s\nCPF: %s\nTipo sanguineo: %s\nFator RH: %s\n###########Endereco###########\n%s\n##############################\nData de nascimento: %s\n"
                       "#########################################################\n\n",
                       aux->codigo_paciente, aux->nome, aux->rg, aux->cpf,
                       aux->tipo_sanguineo, aux->fator_rh,
                       aux->endereco, aux->data_nascimento);

                break;
            }

            aux = aux->prox;
        }
    }
}

void mostrar_dados_base_tipo_sanguineo()
{
    Paciente *aux = pacientes;
    int flag = 1;   //Utilizado para detectar se há pacientes com certo tipo sanguíneo cadastrados.
    int num;

    printf("###TIPOS SANGUINEOS###\n"
           "1 - O\n"
           "2 - A\n"
           "3 - B\n"
           "4 - AB\n"
           "######################\n\n");
    num = ler_inteiro("Inserir numero para mostrar dados de pacientes que tenham determinado tipo sanguineo:\n");
    printf("\n");

    while (aux != NULL) {

        if (num != O && num != A && num != B && num != AB) {
            printf("Valor inserido invalido.\n\n");
            flag = 0;
        }

        if (num == O) {
            if (!strcmp("O", aux->tipo_sanguineo) && !(aux->livre)) {
                printf("\n####################DADOS DO PACIENTE####################\n"
                       "Codigo: %d\nNome: %s\nRG: %s\nCPF: %s\nTipo sanguineo: %s\nFator RH: %s\n###########Endereco###########\n%s\n##############################\nData de nascimento: %s\n"
                       "#########################################################\n\n",
                       aux->codigo_paciente, aux->nome, aux->rg, aux->cpf,
                       aux->tipo_sanguineo, aux->fator_rh,
                       aux->endereco, aux->data_nascimento);
                flag = 0;
            }
        } else if (num == A) {
            if (!strcmp("A", aux->tipo_sanguineo) && !(aux->livre)) {
                printf("\n####################DADOS DO PACIENTE####################\n"
                       "Codigo: %d\nNome: %s\nRG: %s\nCPF: %s\nTipo sanguineo: %s\nFator RH: %s\n###########Endereco###########\n%s\n##############################\nData de nascimento: %s\n"
                       "#########################################################\n\n",
                       aux->codigo_paciente, aux->nome, aux->rg, aux->cpf,
                       aux->tipo_sanguineo, aux->fator_rh,
                       aux->endereco, aux->data_nascimento);
                flag = 0;
            }
        } else if (num == B) {
            if (!strcmp("B", aux->tipo_sanguineo) && !(aux->livre)) {
                printf("\n####################DADOS DO PACIENTE####################\n"
                       "Codigo: %d\nNome: %s\nRG: %s\nCPF: %s\nTipo sanguineo: %s\nFator RH: %s\n###########Endereco###########\n%s\n##############################\nData de nascimento: %s\n"
                       "#########################################################\n\n",
                       aux->codigo_paciente, aux->nome, aux->rg, aux->cpf,
                       aux->tipo_sanguineo, aux->fator_rh,
                       aux->endereco, aux->data_nascimento);
                flag = 0;
            }
        } else if (num == AB) {
            if (!strcmp("AB", aux->tipo_sanguineo) && !(aux->livre)) {
                printf("\n####################DADOS DO PACIENTE####################\n"
                       "Codigo: %d\nNome: %s\nRG: %s\nCPF: %s\nTipo sanguineo: %s\nFator RH: %s\n###########Endereco###########\n%s\n##############################\nData de nascimento: %s\n"
                       "#########################################################\n\n",
                       aux->codigo_paciente, aux->nome, aux->rg, aux->cpf,
                       aux->tipo_sanguineo, aux->fator_rh,
                       aux->endereco, aux->data_nascimento);
                flag = 0;
            }
        }

        aux = aux->prox;
    }

    if (flag) {
        printf("Nao existe nenhum paciente cadastrado com esse tipo sanguineo.\n\n");
    }
}

//Verifica se há um número dentro de uma matriz especificada.
int check_int (int num, int *str, int tam) {
    int flag = 1;
    for (int i = 0; i < tam; ++i) {
        if (str[i] == num) {
            flag = 0;
            break;
        }
    }
    return flag;
}

//Lê uma data e imprime os dados dos pacientes que possuem consulta nesse dia.
void mostrar_pacientes_consulta()
{
    informacoes *aux = atendimento->inicio;
    Paciente *aux2 = pacientes;
    int indice_aux;
    char data[255], data_consulta[11], data_atendimento[11];
    int dia, mes, ano, valor, flag;

    while (1) {
        do {
            dia = 0, mes = 0, ano = 0, flag = 1;

            ler_string("Insira a data da(s) consulta(s) dos pacientes no formato (DD/MM/AAAA):\n", data, 250);

            sscanf(data, "%d/%d/%d", &dia, &mes, &ano);

            valor = validar_data(dia, mes, ano);

            if (valor == 1) {
                flag = 0;
            }

            while (flag == 1) {
                if (!valor) {
                    flag = ler_inteiro("Deseja voltar ao menu de paciente?\n"
                                       "1 - Sim\n"
                                       "2 - Nao\n");

                    if (flag == 1) {
                        return;
                    } else if (flag != 2) {
                        printf("Valor invalido. Tente novamente.\n\n");
                        flag = 1;
                    }
                }
            }
        } while (!valor);

        sprintf(data_consulta, "%02d/%02d/%d", dia, mes, ano);
        printf("Data da(s) consulta(s) inserida: %s\n\n", data_consulta);
        break;
    }

    while (aux != NULL) {
        sprintf(data_atendimento, "%02d/%02d/%d", aux->dia, aux->mes, aux->ano);

        //Se a data inserida for igual á data do atendimento, aux recebe o índice do código do paciente da matriz codigos_pacientes.
        if (strcmp(data_consulta, data_atendimento) == 0 && !(aux->livre)) {
            indice_aux = buscar_indice_codigo(aux->codigo_paciente_atendimento);

            while (aux2 != NULL) {
                if (!(aux2->livre) && aux2->indice == indice_aux) {
                    printf("\n####################DADOS DO PACIENTE####################\n"
                           "Codigo: %d\nNome: %s\nRG: %s\nCPF: %s\nTipo sanguineo: %s\nFator RH: %s\n###########Endereco###########\n%s\n##############################\nData de nascimento: %s\n"
                           "#########################################################\n\n",
                           aux2->codigo_paciente, aux2->nome, aux2->rg, aux2->cpf,
                           aux2->tipo_sanguineo, aux2->fator_rh,
                           aux2->endereco, aux2->data_nascimento);

                    return;
                }

                aux2 = aux2->prox;
            }
        }

        aux = aux->prox;
    }
}

void mostrar_pacientes_ordem_alfabetica()
{
    Paciente *aux;
    int indice_nomes[qtd_structs];  //Organiza em ordem alfabética os nomes dos pacientes cadastrados.
    int j = 0;

    for (int i = 0; i < qtd_structs; ++i) {
        indice_nomes[i] = -1;
    }   //Inicializando o vetor auxvetor.

    for (int i = 0; i < qtd_structs; ++i) {
        char auxnome[255] = "ZZZZZZZZZZZZZZZZZZZZ"; //Garantir que não haverá nenhum nome maior dentro da matriz nomes. Ideia semelhante a INT_MAX.
        aux = pacientes;
        j = 0;

        //Coloca em ordem alfabética os nomes dos pacientes armazenando seus índices em um vetor na ordem correta.
        while (aux != NULL) {
            if (strcmp(aux->nome, auxnome) <= 0 && check_int(j, indice_nomes, qtd_structs) && !(aux->livre)) {
                strcpy(auxnome, aux->nome);
                indice_nomes[i] = aux->indice;
            }

            j++;    //Simula o 'for(int j = 0; j < qtd_structs; j++)' utilizando while.
            aux = aux->prox;
        }
    }

    printf("CODIGO   TIPO SANGUINEO   CPF             DATA DE NASCIMENTO   NOME DO PACIENTE\n");

    j = 0;  //Necessário para percorrer o indice_nomes[i] a partir do início assim que entrar no while.

    while (1) {
        aux = pacientes;

        //Encerra o loop assim que conseguir imprimir todos os pacientes.
        if (indice_nomes[j] == -1 || j == qtd_structs) {
            break;
        }

        while (aux != NULL) {
            if (!(aux->livre) && aux->indice == indice_nomes[j]) {
                printf("%-9d%-2s%-15s%-16s%-21s%s\n",
                       aux->codigo_paciente, aux->tipo_sanguineo, aux->fator_rh, aux->cpf, aux->data_nascimento, aux->nome);
                break;
            } else {
                aux = aux->prox;
            }
        }
        j++;
    }
    printf("\n");
}

void salvar_dados_paciente()
{
    Paciente *aux = pacientes;
    unsigned long long valor = 0;

    while (aux != NULL) {

        if (aux->status == rascunho) {
            valor = fseek(dados_pacientes, (aux->indice) * sizeof(Paciente), SEEK_SET);
            if (valor != 0) {
                printf("Erro ao salvar os dados.\n\n");
                break;
            }

            valor = fwrite(aux, sizeof(Paciente), 1, dados_pacientes);
            if (valor == 0) {
                printf("Não foi possível salvar os dados. Verifique se há algum dado a ser salvo.\n\n");
                break;
            } else {
                aux->status = salvo;
            }
        }

        aux = aux->prox;
    }

    printf("Dados dos pacientes salvos com sucesso.\n\n");
}

void desalocar_structs_paciente()
{
    Paciente *aux = pacientes;
    Paciente *aux2;

    while (aux != NULL) {
        aux2 = aux;

        if (aux2 != NULL) {
            aux = aux2->prox;
            free(aux2);
        }
    }

    free(aux);
}

void fechar_aquivo_pacientes()
{
    printf("Arquivo de pacientes fechado.\n\n");
    fclose(dados_pacientes);
}