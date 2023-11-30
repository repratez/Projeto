#include <iostream>
#include <string.h>
#include <Windows.h>
#include <locale>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PRODUTOS 100

struct Periferico {
    char nome_do_produto[50];
    float preco;
    int quantidade;
};

void salvarEmArquivo(struct Periferico *estoque, int numProdutos) {
    FILE *arquivo;
    arquivo = fopen("dados1.dat", "wb");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        exit(1);
    }

    fwrite(estoque, sizeof(struct Periferico), numProdutos, arquivo);

    fclose(arquivo);
}

void registrarProduto(struct Periferico *estoque, int *numProdutos) {
    if (*numProdutos < MAX_PRODUTOS) {
        printf("Digite o nome do produto: ");
        scanf(" %[^\n]s", estoque[*numProdutos].nome_do_produto);
        printf("Digite o preco do produto: ");
        scanf("%f", &estoque[*numProdutos].preco);
        printf("Digite a quantidade em estoque: ");
        scanf("%d", &estoque[*numProdutos].quantidade);

        (*numProdutos)++;

        // Salvar automaticamente após o registro do produto
        salvarEmArquivo(estoque, *numProdutos);
    } else {
        printf("O estoque está cheio. Não é possível adicionar mais produtos.\n");
    }
}

void venderProduto(struct Periferico *estoque, int numProdutos) {
    char nomeProduto[50];
    int quantidadeVendida;

    printf("Digite o nome do produto que deseja vender: ");
    scanf(" %[^\n]s", nomeProduto);

    for (int i = 0; i < numProdutos; i++) {
        if (strcmp(nomeProduto, estoque[i].nome_do_produto) == 0) {
            printf("Digite a quantidade que deseja vender: ");
            scanf("%d", &quantidadeVendida);

            if (quantidadeVendida <= estoque[i].quantidade) {
                estoque[i].quantidade -= quantidadeVendida;
            } else {
                printf("Quantidade insuficiente em estoque.\n");
            }

            // Salvar automaticamente após a venda do produto
            salvarEmArquivo(estoque, numProdutos);
            return;
        }
    }

    printf("Produto não encontrado no estoque.\n");
}

void colocarNoEstoque(struct Periferico *estoque, int numProdutos) {
    char nomeProduto[50];
    int quantidadeColocada;

    printf("Digite o nome do produto que deseja colocar no estoque: ");
    scanf(" %[^\n]s", nomeProduto);

    for (int i = 0; i < numProdutos; i++) {
        if (strcmp(nomeProduto, estoque[i].nome_do_produto) == 0) {
            printf("Digite a quantidade que deseja colocar no estoque: ");
            scanf("%d", &quantidadeColocada);

            estoque[i].quantidade += quantidadeColocada;

            // Salvar automaticamente após colocar no estoque
            salvarEmArquivo(estoque, numProdutos);
            return;
        }
    }

    printf("Produto não encontrado no estoque.\n");
}

void consultarProdutos(struct Periferico *estoque, int numProdutos) {
    printf("\n-----------------------------------\n");
    printf("Produtos em estoque:\n");
    for (int i = 0; i < numProdutos; i++) {
        printf("Nome: %s\n", estoque[i].nome_do_produto);
        printf("Preco: %.2f\n", estoque[i].preco);
        printf("Quantidade em estoque: %d\n\n", estoque[i].quantidade);
    }
}

void carregarDoArquivo(struct Periferico *estoque, int *numProdutos, int *dadosCarregados) {
    FILE *arq;

    if ((*dadosCarregados == 0) && ((arq = fopen("dados1.dat", "rb+")) != NULL)) {
        *numProdutos = fread(estoque, sizeof(struct Periferico), MAX_PRODUTOS, arq);
        fclose(arq);

        // Mensagem de carregamento com animação de barra de progresso e porcentagem
        printf("Dados Carregados: [");
        fflush(stdout);

        int progresso = 0;
        int totalProgresso = *numProdutos * 100 / MAX_PRODUTOS; // Calcula a porcentagem de progresso
        for (int i = 0; i <= totalProgresso; i += 2) {
            printf("%c", 219); // Imprime um bloco
            fflush(stdout);
            progresso += 2;
            Sleep(100); // Aguarda 100 milissegundos para criar o efeito de animação
        }

        // Imprime os blocos restantes para manter o 100% estático
        for (int i = progresso; i < 100; i += 2) {
            printf("%c", 219);
            fflush(stdout);
            Sleep(10);
        }

        printf("] 100%%\n");

        *dadosCarregados = 1;
    }
}

void salvarEmArquivoTexto(struct Periferico *estoque, int numProdutos) {
    FILE *arquivo;
    arquivo = fopen("estoque.txt", "w");  // Modificado para "w" para recriar o arquivo

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        exit(1);
    }

    for (int i = 0; i < numProdutos; i++) {
        fprintf(arquivo, "Nome: %s\n", estoque[i].nome_do_produto);
        fprintf(arquivo, "Preco: %.2f\n", estoque[i].preco);
        fprintf(arquivo, "Quantidade em estoque: %d\n\n", estoque[i].quantidade);
    }

    fclose(arquivo);

    // Realiza o backup automaticamente
    arquivo = fopen("backup.txt", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de backup para escrita.\n");
        exit(1);
    }

    for (int i = 0; i < numProdutos; i++) {
        fprintf(arquivo, "Nome: %s\n", estoque[i].nome_do_produto);
        fprintf(arquivo, "Preco: %.2f\n", estoque[i].preco);
        fprintf(arquivo, "Quantidade em estoque: %d\n\n", estoque[i].quantidade);
    }

    fclose(arquivo);
}

int main() {
    struct Periferico estoque[MAX_PRODUTOS];
    int numProdutos = 0;
    int opcao;
    int dadosCarregados = 0; // Adicionada variável para controlar se os dados foram carregados

    // Carregar dados do arquivo antes de iniciar o programa
    carregarDoArquivo(estoque, &numProdutos, &dadosCarregados);

    do {
        printf("\n------------------------------------\n");

        printf("\nEscolha uma opcao:\n");
        printf("1 - Registrar produto\n");
        printf("2 - Vender produto\n");
        printf("3 - Consultar produtos\n");
        printf("4 - Colocar no estoque\n");  // Nova opção para colocar no estoque
        printf("5 - Salvar em arquivo de texto\n");  // Nova opção para salvar em arquivo de texto
        printf("0 - Sair\n");

        printf("\n");

        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                registrarProduto(estoque, &numProdutos);
                break;
            case 2:
                venderProduto(estoque, numProdutos);
                break;
            case 3:
                consultarProdutos(estoque, numProdutos);
                break;
            case 4:
                colocarNoEstoque(estoque, numProdutos);  // Chama a nova função para colocar no estoque
                break;
            case 5:
                salvarEmArquivoTexto(estoque, numProdutos);  // Chama a nova função para salvar em arquivo de texto
                break;
            case 0:
                printf("Encerrando o programa e salvando os dados.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

