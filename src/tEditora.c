//pasta criada para organizar todas as funções que estão relacionadas às editoras dos livros.
#include "../headers/tEditora.h"
//struct que recebe todas as informacoes necessárias da editora.
struct teditora {
  char nome[128];
};
//variáveis globais que seram usadas nas funções "inicializarEditoras", "getEditora", "cadastrar_editora", "deletarAllEditoras", "deletarEditoraList".
tEditora** allEditoras;
int allEditoras_size;
int editoraCounter = 0;

//cria uma estrutura em formato de vetor com todas as editoras.
void inicializarEditoras(int size) {
  if (allEditoras == NULL) {
    allEditoras = realloc(allEditoras, size*sizeof(struct teditora));
    for (int i = 0; i < size; i++) {
      allEditoras[i] = NULL;
    }
  } else {
    allEditoras = realloc(allEditoras, size*sizeof(struct teditora));
  }
  allEditoras_size = size;
}

//função que procura um nome de editora entre todos os que já foram cadastrados para descobrir qual é compatível com o que o usuário está buscando.
tEditora* getEditora(char nome[]) {
  for (int i = 0; i < editoraCounter; i++){
    if (allEditoras[i] == NULL) return NULL;
    if (strcmp(allEditoras[i]->nome, nome) == 0) {
      return allEditoras[i];
    }
  }
  return NULL;
}
//função que descobre o nome da editora ao chamar a struct inteira.
char* getNomeEditora(tEditora* editora) {
  return editora->nome;
}

//função que cadastra uma editora no sistema.
tEditora* cadastrar_editora(char nome[]) {
  tEditora* editora = getEditora(nome);
  if (editora == NULL) {
    if (editoraCounter == allEditoras_size) {
      inicializarEditoras(editoraCounter + 1);
    }
    editora = (tEditora*) malloc(sizeof(struct teditora));
    strcpy(editora->nome, nome);
    allEditoras[editoraCounter] = editora;
    editoraCounter++;
  }
  return editora;
}

//função que deleta uma editora.
void deletarEditora(tEditora* editora) {
  free(editora);
  editora = NULL;
}

//funcao que recebe um vetor de editoras e deleta todas as editoras presentes no vetor.
void deletarEditoraList(tEditora** editoras) {
  int i = 0;
  tEditora *t = editoras[i];
  tEditora *aux = editoras[i+1];
  while(t != NULL){
    deletarEditora(t);
    if (i == (editoraCounter-1)) {
      break;
    }
    aux = editoras[i+1];
    t = aux;
    i++;
  }
  free(editoras);
}

//função que deleta todas as editoras do sistema.
void deletarAllEditoras() {
  deletarEditoraList(allEditoras);
}