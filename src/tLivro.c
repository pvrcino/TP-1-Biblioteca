//pasta criada para organizar todas as funções que estão relacionadas aos livros da biblioteca.
#include "../headers/tLivro.h"
//struct que contém todas as informações necessárias para cadastrar e armazenar os livros no sistema.
struct tlivro {
  int id;
  char titulo[128];
  double valor;
  int ano;
  int unidades;
  int locacoes;

  bool disponivel;

  tAutor* autores[60];
  tEditora* editora;
};
//variáveis globais que serão utilizadas nas funções "inicializarLivros", "getLivro", "getLivroByTitulo", "retorna_livros", "cadastrar_livro", "deletarAllLivros", "deletarLivrosList".
tLivro** allLivros;
int allLivros_size;
int livrosCounter = 0;

//cria uma estrutura em formato de vetor com todos os livros.
void inicializarLivros(int size) {
  allLivros = realloc(allLivros, size*sizeof(struct tlivro));
  allLivros_size = size;
  if (allLivros == NULL) {
    allLivros = realloc(allLivros, size*sizeof(struct tlivro));
    for (int i = 0; i < size; i++) {
      allLivros[i] = NULL;
    }
  }
}

//função que procura uma id de um livro entre todas as que já foram cadastradas para descobrir qual é compatível com a que o usuário está buscando.
tLivro* getLivro(int id) {
  for (int i = 0; i < livrosCounter; i++){
    if (allLivros[i] == NULL) return NULL;
    if (allLivros[i]->id == id) {
      return allLivros[i];
    }
  }
  return NULL;
}

//função que procura um título de um livro entre todos as que já foram cadastrados para descobrir qual é compatível com o que o usuário está buscando.
tLivro* getLivroByTitulo(char* titulo) {
  tLivro* t = NULL;
  for(int i = 0; i < livrosCounter; i++){
    if (allLivros[i] == NULL) return NULL;
    if (strcmp(allLivros[i]->titulo, titulo) == 0) {
      t = allLivros[i];
    }
  }
  return t;
}
//função que retorna apenas o título do livro ao receber todas as suas informações por meio da struct tLivro.
char* getTitulo(tLivro* livro) {
  return livro->titulo;
}
//função que retorna apenas o autor do livro ao receber todas as suas informações por meio da struct tLivro.
tAutor** retorna_autores(tLivro* livro) {
  return livro->autores;
}
//função que preenche um vetor com todos os livros de um autor
void retorna_livros(tAutor* autor, tLivro* livros[]) {
  int i = 0,j = 0;
  tLivro* aux = NULL;
  for (i = 0; i<livrosCounter; i++) {
    aux = allLivros[i];
    if (aux == NULL) return;
    if (autorListContains(aux->autores, autor)) {
      livros[j] = aux;
      j+=1;
    }
  }
}

//função que chama todos os livro.
tLivro** getAllLivros() {
  return allLivros;
}

//função que retorna apenas a editora do livro ao receber todas as suas informações por meio da struct tLivro.
tEditora* retorna_editora(tLivro* livro) {
  return livro->editora;
}
//função que verifica a disponibilidade do livro para locação.
void setDisponibilidade(tLivro* t, bool b) {
  t->disponivel = b;
}
//função que retorna apenas a disponibilidade do livro ao receber todas as suas informações por meio da struct tLivro.
bool disponivel(tLivro* t) {
  return t->disponivel;
}
//função que recebe a quantidade disponível para cadastro que um determinado livro possui e adiciona algumas unidades baseado no que o usuário pedir.
void adicionarUnidades(tLivro* livro, int units) {
  livro->unidades = livro->unidades + units;
}
//função que adiciona uma unidade a quantidade de locações que o livro foi alugado toda vez qur houver uma locação
void adicionarLocacoes(tLivro* livro, int units) {
  livro->locacoes = livro->locacoes + units;
}
//função que recebe a quantidade disponível para cadastro que um determinado livro possui e remove algumas unidades baseado no que o usuário pedir.
void removerUnidades(tLivro* livro, int units) {
  livro->unidades = livro->unidades - units;
}
//função que cadastra um livro no sistema.
tLivro* cadastrar_livro(int id, char titulo[], tAutor* autores[], tEditora* editora, double valor, int ano) {
  tLivro* livro = getLivro(id);
  if (livro == NULL) {
    if (livrosCounter == allLivros_size) {
      inicializarLivros(livrosCounter + 1);
    }
    livro = (tLivro*) malloc(sizeof(struct tlivro));

    strcpy(livro->titulo, titulo);
    livro->id = id;
    livro->valor = valor;
    livro->ano = ano;
    livro->editora = editora;
    livro->disponivel = true;
    livro->unidades = 1;
    livro->locacoes = 0;
    for (int i = 0; i < 60; i++) {
      livro->autores[i] = autores[i];
    }

    allLivros[livrosCounter] = livro;
    livrosCounter++;
  } else {
    adicionarUnidades(livro, 1);
  }
  return livro;
}
//função que converte int para string
// char *itoa ( int value, char * str ){
//     char temp;
//     int i =0;
//     while (value > 0) {
//       int digito = value % 10;

//         str[i] = digito + '0';
//         value /= 10;
//         i++;
//     }
//    i = 0;
//    int j = strlen(str) - 1;
//    while (i < j) {
//       temp = str[i];
//       str[i] = str[j];
//       str[j] = temp;
//       i++;
//       j--;
//    }
//     return str;
// }

//função que atualiza o arquivo "books_-_books.csv" toda vez que um livro é cadastrado.
void adicionarLivroFile(tLivro* livro) {
  FILE *temp;
  temp = fopen("./db/books_-_books.csv", "a");
  char aux_autores[1024] = "", data[30] = "1/01/", aux[10];
  int i = 0;

  tAutor* autor = retorna_autores(livro)[i];
  tAutor* autoraux;
  while (autor != NULL) {
    strcat(aux_autores, getNomeAutor(autor));
    i++;
    autoraux = retorna_autores(livro)[i];
    if (autoraux != NULL) {
      strcat(aux_autores, "/");
    }
    autor = autoraux;
  }
    
  itoa(livro->ano, aux, 10);
  strcat(data, aux);
  fprintf(temp, "%d,%s,%s,%s,%.2lf,%s,%s,%d\n", livro->id, livro->titulo, aux_autores, "5.00", livro->valor, data, getNomeEditora(livro->editora), getUnidades(livro));

  fclose(temp);
}
//funcao que recebe um vetor de livros e deleta todos os livros presentes no vetor.
void deletarLivrosList(tLivro** livros) {
  int i = 0;
  tLivro *t = livros[i];
  tLivro *aux = livros[i+1];
  while(t != NULL){
    deletarLivro(t);
    if (i == (livrosCounter-1)) {
      break;
    }
    aux = livros[i+1];
    t = aux;
    i++;
  }
  free(livros);
}
//função que retorna apenas a id do livro ao receber todas as suas informações por meio da struct tLivro.
int getId(tLivro* t) {
  return t->id;
}
//deleta todos os livros cadastrados.
void deletarAllLivros() {
  deletarLivrosList(allLivros);
}

//deleta um livro cadastrado no sistema
void deletarLivro(tLivro* livro) {
  free(livro);
  livro = NULL;
}

//função que retorna apenas o valor do livro ao receber todas as suas informações por meio da struct tLivro.
double getValorLivro(tLivro* livro) {
  return livro->valor;
}

//função que retorna apenas o ano de publicação do livro ao receber todas as suas informações por meio da struct tLivro.
int getAnoLivro(tLivro* livro) {
  return livro->ano;
}

//função que retorna apenas as unidades disponíveis do livro ao receber todas as suas informações por meio da struct tLivro.
int getUnidades(tLivro* livro) {
  return livro->unidades;
}

//função que retorna apenas as locações do livro ao receber todas as suas informações por meio da struct tLivro.
int getLocacoesQnt(tLivro* livro) {
  return livro->locacoes;
}

