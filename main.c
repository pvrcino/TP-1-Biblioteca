#include "headers/Menu.h"

void leLivros();
void leClientes();
void leLocacoes();

int main() {
  inicializarAutores(500);
  inicializarEditoras(500);
  inicializarClientes(1);
  inicializarLocacoes(1);
  inicializarLivros(11000);

  leLivros();
  leClientes();
  leLocacoes();
  
  menu0();
  finalizar();
}

//Função criada para ler todas as informações do arquivo books_-_books.csv".
void leLivros(){
  char linha[1024], temp_titulo[256], temp_id[15], temp_autores[768], temp_nota[10], temp_valor[20], temp_data[20], temp_editora[128], temp_units[10];

  tAutor* autores[60];

  int i = -1, trash, ano;
  FILE *temp;

  //comando para abrir o arquivo "books_-_books.csv".
  temp = fopen("db/books_-_books.csv", "r");

  //verificando abertura do arquivo
  if (temp == NULL) {
    printf("Erro ao abrir o arquivo /db/books_-_books.csv");
  }

  //enquanto o arquivo não acabar, o programa deverá ler o que está escrito nele.
  while (!feof(temp)) {
    fscanf(temp, "%[^\n]\n", linha);
    if (i >= 0) {
      //em cada linha do arquivo, o programa separa em partes, de acordo com sua utilidade.
      sscanf(linha, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", temp_id, temp_titulo, temp_autores, temp_nota, temp_valor,temp_data, temp_editora, temp_units);
      sscanf(temp_data, "%d/%d/%d", &trash, &trash, &ano);

      registrarAutores(temp_autores, autores);

      tEditora* editora = cadastrar_editora(temp_editora);

      tLivro* livro = cadastrar_livro(atoi(temp_id), temp_titulo, autores, editora, atof(temp_valor), ano);

      adicionarUnidades(livro, atoi(temp_units)-1);

    }
    i=0;
  }
  //fechando o arquivo "books_-_books.csv".
  fclose(temp);
}

//Função criada para ler todas as informações do arquivo locacoes.csv".
void leLocacoes() {
  char linha[1024],aux_dataLocacao[20], aux_dataDevolucao[20];
  int i = -1, diasLocados, idLivro;
  FILE *temp;
  
  //abertura do arquivo "locacoes.csv".
  temp = fopen("db/locacoes.csv", "r");

  //verificando abertura do arquivo
  if (temp == NULL) {
    printf("Erro ao abrir o arquivo /db/locacoes.csv");
  }
  //enquanto o arquivo não acabar, o programa deverá ler o que está escrito nele.
  while (!feof(temp)) {
    int j = 0, cpf[11];
    char aux_cpf[13];
    fscanf(temp, "%[^\n]\n", linha);
    if (i >= 0) {
      //em cada linha do arquivo, o programa separa em partes, de acordo com sua utilidade.
      sscanf(linha, "%[^,],%[^,],%d,%[^,],%d",aux_dataLocacao,aux_dataDevolucao, &diasLocados,aux_cpf,&idLivro);
      while (aux_cpf[j] != '\0') {
        if (aux_cpf[j] == '\n') {
          aux_cpf[j] = '\0';
          break;
        }
        cpf[j] = aux_cpf[j]-'0';
        j++;
      }
      tData* dataLocacao = filtroDeData(aux_dataLocacao);
      tData* dataDevolucao = filtroDeData(aux_dataDevolucao);

      tCliente* t = getCliente(cpf);
      tLivro* livro = getLivro(idLivro);
      tLocacao* loc = cadastrar_locacao(dataLocacao, diasLocados, t, livro);

      if (getAno(dataDevolucao) == 0) {
        deletarData(dataDevolucao);
      } else {
        devolverLivro(loc, dataDevolucao);
      }
    }
    i = 0;
    j = 0;
  }
  //fechando o arquivo "locacoes.csv".
  fclose(temp);
}

//Função criada para ler todas as informações do arquivo clientes.csv".
void leClientes() {
  char linha[512], nome[128], sexo, telefone[128], email[128];

  int j = 0;

  int i = -1;
  FILE *temp;
  
  //abertura do arquivo "clientes.csv".
  temp = fopen("db/clientes.csv", "r");

  //verificando abertura do arquivo
  if (temp == NULL) {
    printf("Erro ao abrir o arquivo /db/clientes.csv");
  }

  //enquanto o arquivo não acabar, o programa deverá ler o que está escrito nele.
  while (!feof(temp)) {
    fscanf(temp, "%[^\n]\n", linha);
    if (i >= 0) {
      int cpf[11];
      char aux_cpf[13];
      //em cada linha do arquivo, o programa separa em partes, de acordo com sua utilidade.
      sscanf(linha, "%[^,],%[^,],%c,%[^,],%[^,]", nome, aux_cpf, &sexo, telefone, email);
      while (aux_cpf[j] != '\0') {
        if (aux_cpf[j] == '\n') {
          aux_cpf[j] = '\0';
          break;
        }
        cpf[j] = aux_cpf[j]-'0';
        j++;
      }
      cadastrar_cliente(nome, cpf, sexo, telefone, email);
    }
    i=0;
    j=0;
  }
  //fechando o arquivo "clientes.csv".
  fclose(temp);
}