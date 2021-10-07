//pasta criada para organizar todas as funções que estão relacionadas aos autores dos livros.
#include "../headers/tAutor.h"
//struct com todas as informações necessárias para armazenar os autores dos livros.
struct tautor{
  char nome[64];
};
//variáveis globais que serão utilizadas nas funções "inicializarAutores", "getAutor", "getAutorNome", "deletarAllAutores", "criarAutor", "deletarAutorList".
tAutor** allAutores;
int allAutores_size;
int autoresCounter = 0;

//cria uma estrutura em formato de vetor com todos os autores.
void inicializarAutores(int size) {
  if (allAutores == NULL) {
    allAutores = realloc(allAutores, size*sizeof(struct tautor));
    for (int i = 0; i < size; i++) {
      allAutores[i] = NULL;
    }
  } else {
    allAutores = realloc(allAutores, size*sizeof(struct tautor));
  }
  allAutores_size = size;
}
//função que procura um autor entre todos os que já foram cadastrados para descobrir qual é compatível com o que o usuário está buscando.
tAutor* getAutor(char nome[]) {
  for (int i = 0; i < autoresCounter; i++){
    if (allAutores[i] == NULL) return NULL;
    if (strcmp(getNomeAutor(allAutores[i]), nome) == 0) {
      return allAutores[i];
    }
  }
  return NULL;
}

//função que cadastra um autor no sistema por meio do nome.
tAutor* criarAutor(char nome[]) {
  tAutor* autor = getAutor(nome);
  if (autor == NULL) {
    if (autoresCounter == allAutores_size) {
      inicializarAutores(autoresCounter + 1);
    }
    autor = (tAutor*) malloc(sizeof(struct tautor));
    strcpy(autor->nome, nome);
    allAutores[autoresCounter] = autor;
    autoresCounter++;
  }
  return autor;
}
//função que descobre o nome do autor ao chamar a struct inteira.
char* getNomeAutor(tAutor* autor){
  return autor->nome;
}
//função que deleta um determinado autor cadastrado no sistema.
void deletarAutor(tAutor* autor){
  free(autor);
  autor = NULL;
}
//funcao que recebe um vetor de autores e deleta todos os clientes presentes no vetor.
void deletarAutorList(tAutor** autores){
  int i = 0;
  tAutor *t = autores[i];
  tAutor* aux = autores[i+1];
  while(t != NULL){
    deletarAutor(t);
    if (i == (autoresCounter-1)) {
      break;
    }
    aux = autores[i+1];
    t = aux;
    i++;
  }
  free(autores);
}
//função que deleta todos os autores do sistema.
void deletarAllAutores() {
  deletarAutorList(allAutores);
}
//transforma uma string com nomes de autores separados por "/" em um vetor de tAutores
void registrarAutores(char nomes[], tAutor* autores[]) {

  int counter = 0;
  char aux_autores[64] = " ", dest[1024] = "";
  

  while (strlen(nomes) != strlen(aux_autores)) {
    aux_autores[0] = '\0';
    sscanf(nomes, "%[^/]", aux_autores);
    
    if (getAutor(aux_autores) == NULL) {
      autores[counter] = criarAutor(aux_autores);
    } else {
      autores[counter] = getAutor(aux_autores);
    }

    if (strcmp(nomes, aux_autores) == 0) break; 

    strncpy(dest, nomes + strlen(aux_autores) + 1, (strlen(nomes)-strlen(aux_autores)));
    strcpy(nomes, dest);
    counter++;
  }
}
//verifica se existe um determinado autor escolhido pelo usuário dentro de um vetor de autores.
bool autorListContains(tAutor* autores[], tAutor* autor){
  int i = 0;
  if (autores == NULL) return 0;
  tAutor* aux = autores[i];
  while (aux != NULL) {
    if (strcmp(autor->nome, aux->nome) == 0) {
      return 1;
      break;
    } else {
      aux = autores[i+1];
      i++;
    }
  }
  return 0;
}
