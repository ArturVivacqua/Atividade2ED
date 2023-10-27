#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Musica {
    char artista[100];
    char nome[100];
    struct Musica* prox;
    struct Musica* ant;
};

struct Musica* criarMusica(const char* artista, const char* nome) {
    struct Musica* novaMusica = (struct Musica*)malloc(sizeof(struct Musica));
    strcpy(novaMusica->artista, artista);
    strcpy(novaMusica->nome, nome);
    novaMusica->prox = novaMusica;
    novaMusica->ant = novaMusica;
    return novaMusica;
}

void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void inserirMusica(struct Musica** inicio, const char* artista, const char* nome) {
    struct Musica* novaMusica = criarMusica(artista, nome);

    if (*inicio == NULL) {
        *inicio = novaMusica;
    } else {
        novaMusica->prox = *inicio;
        novaMusica->ant = (*inicio)->ant;
        (*inicio)->ant->prox = novaMusica;
        (*inicio)->ant = novaMusica;
    }
}

void exibirPlaylist(struct Musica* inicio) {
    if (inicio == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }

    struct Musica* atual = inicio;

    printf("Playlist pela ordem de cadastro:\n");
    do {
        printf("Artista: %s - Música: %s\n", atual->artista, atual->nome);
        atual = atual->prox;
    } while (atual != inicio);
}

void exibirPlaylistOrdenada(struct Musica* inicio) {
    if (inicio == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }

    struct Musica* atual = inicio;
    struct Musica* temp;

    do {
        temp = atual->prox;
        do {
            if (strcmp(atual->nome, temp->nome) > 0) {
                char tempNome[100];
                char tempArtista[100];
                strcpy(tempNome, atual->nome);
                strcpy(tempArtista, atual->artista);
                strcpy(atual->nome, temp->nome);
                strcpy(atual->artista, temp->artista);
                strcpy(temp->nome, tempNome);
                strcpy(temp->artista, tempArtista);
            }
            temp = temp->prox;
        } while (temp != inicio);
        atual = atual->prox;
    } while (atual != inicio);

    printf("Playlist ordenada pelo nome das músicas:\n");
    do {
        printf("Artista: %s - Música: %s\n", atual->artista, atual->nome);
        atual = atual->prox;
    } while (atual != inicio);
}

void removerMusica(struct Musica** inicio, const char* nome) {
    if (*inicio == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }

    struct Musica* atual = *inicio;
    struct Musica* temp;

    do {
        temp = atual;
        atual = atual->prox;

        if (strcmp(temp->nome, nome) == 0) {
            if (temp == *inicio && temp->prox == temp) {
                free(temp);
                *inicio = NULL;
            } else {
                temp->ant->prox = temp->prox;
                temp->prox->ant = temp->ant;

                if (temp == *inicio) {
                    *inicio = temp->prox;
                }

                free(temp);
            }

            printf("A música '%s' foi removida da playlist.\n", nome);
            return;
        }
    } while (atual != *inicio);

    printf("A música '%s' não foi encontrada na playlist.\n", nome);
}

void avancarMusica(struct Musica** inicio) {
    if (*inicio == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }

    *inicio = (*inicio)->prox;
}

void retornarMusica(struct Musica** inicio) {
    if (*inicio == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }

    *inicio = (*inicio)->ant;
}

int main() {
    struct Musica* inicio = NULL;
    struct Musica* musicaAtual = NULL;
  
    FILE* arquivo = fopen("musicas.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo 'musicas.txt'. Certifique-se de que o arquivo existe.\n");
        return 1;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        char artista[100], nome[100];
        sscanf(linha, "%99[^;];%99[^\n]", artista, nome);
        inserirMusica(&inicio, artista, nome);
    }

    fclose(arquivo);

    int escolha;
    char nomeMusica[100];

      while (1) {
        printf("\nMenu:\n");
        printf("Música atual: %s - %s\n", musicaAtual ? musicaAtual->artista : "Nenhuma", musicaAtual ? musicaAtual->nome : "Nenhuma");
        printf("1. Exibir a playlist pela ordem de cadastro\n");
        printf("2. Exibir a playlist ordenada pelo nome das músicas\n");
        printf("3. Inserir uma nova música\n");
        printf("4. Remover uma música\n");
        printf("5. Avançar para a próxima música\n");
        printf("6. Retornar à música anterior\n");
        printf("7. Sair\n");

        printf("Escolha uma opção: ");
        scanf("%d", &escolha);
        printf("\033c");

        switch (escolha) {
            case 1:
                exibirPlaylist(inicio);
                printf("\n");
                printf("Digite qualquer caracter para continuar\n");
                flush_stdin();
                getchar();
                printf("\033c");
                break;
            case 2:
                exibirPlaylistOrdenada(inicio);
                printf("\n");
                printf("Digite qualquer caracter para continuar\n");
                flush_stdin();
                getchar();
                printf("\033c");
                break;
          case 3:
                printf("Digite o nome do artista: ");
                flush_stdin();
                fgets(nomeMusica, sizeof(nomeMusica), stdin);
                nomeMusica[strcspn(nomeMusica, "\n")] = 0;
                printf("Digite o nome da música: ");
                fgets(linha, sizeof(linha), stdin);
                linha[strcspn(linha, "\n")] = 0;
                inserirMusica(&inicio, nomeMusica, linha);
      
                arquivo = fopen("musicas.txt", "a");
                fprintf(arquivo, "%s;%s\n", nomeMusica, linha);
                fclose(arquivo);
      
                printf("A música foi inserida na playlist.\n");
                printf("\n");
                sleep(2);
                printf("\033c");
                break;
            case 4:
                printf("Digite o nome da música a ser removida: ");
                flush_stdin();
                fgets(nomeMusica, sizeof(nomeMusica), stdin);
                nomeMusica[strcspn(nomeMusica, "\n")] = 0;
                removerMusica(&inicio, nomeMusica);

                arquivo = fopen("musicas.txt", "w");
                struct Musica* atual = inicio;
                if (inicio != NULL) {
                    do {
                        fprintf(arquivo, "%s;%s\n", atual->artista, atual->nome);
                        atual = atual->prox;
                    } while (atual != inicio);
                }
                fclose(arquivo);
                
                break;
          case 5:
              avancarMusica(&inicio);
              musicaAtual = inicio;
              printf("\033c");
              break;
          case 6:
              retornarMusica(&inicio);
              musicaAtual = inicio;
              printf("\033c");
              break;
            case 7:
                while (inicio != NULL) {
                    struct Musica* temp = inicio;
                    inicio = inicio->prox;
                    free(temp);
                }
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}