#include "../headers/tLocacao.h"
//struct que possui todas as informações necessárias para a locação.
struct tlocacao {
  tData* data_locacao;
  tData* data_devolucao;

  tCliente* cliente;
  tLivro* livro;
  
  int dias_locado;
};
//variáveis globais que serão usadas em "inicializarLocacoes", "cadastrar_locacao", "deletarAllLocacoes", "getLocacao", "getLocacoes", "deletarLocacaoList".
tLocacao** allLocacoes;
int allLocacoes_size;
int locacoesCounter = 0;

//cria uma estrutura em formato de vetor com todas as locações.
void inicializarLocacoes(int size) {
  allLocacoes_size = size;
  if (allLocacoes == NULL) {
    allLocacoes = realloc(allLocacoes, size*sizeof(struct tlocacao));
    for (int i = 0; i < size; i++) {
      allLocacoes[i] = NULL;
    }
  } else {
      allLocacoes = realloc(allLocacoes, size*sizeof(struct tlocacao));
  }
}

//função que cadastra os dados de uma locação no sistema.
tLocacao* cadastrar_locacao(tData* dataLocacao, int diasLocados, tCliente* cliente, tLivro* livro) {
  if (locacoesCounter == allLocacoes_size) {
    inicializarLocacoes(locacoesCounter + 1);
  }
  tLocacao* locacao = malloc(sizeof(struct tlocacao));
  locacao->data_locacao = dataLocacao;
  locacao->data_devolucao = NULL;
  locacao->dias_locado = diasLocados;
  locacao->cliente = cliente;
  locacao->livro = livro;

  setDisponibilidade(livro, false);

  allLocacoes[locacoesCounter] = locacao;
  locacoesCounter++;

  adicionarLocacoes(livro, 1);
  return locacao;
}
//adiciona uma locação no arquivo "relatorio_locacao.csv" e salva na base de dados.
void adicionarLocacaoFile(tLocacao* t, char* arq) {
  FILE *temp;
  char aux_cpf[13];
  int i = 0;
  temp = fopen(arq, "a");

  tData* dataDevolucao = t->data_devolucao;
  char aux_dataDevolucao[] = "00/00/0000";
  if (t->data_devolucao != NULL) {
    sprintf(aux_dataDevolucao, "%d/%d/%d", getMes(dataDevolucao), getDia(dataDevolucao), getAno(dataDevolucao));
  }

  tData* dataLocacao = t->data_locacao;
  char aux_dataLocacao[] = "00/00/0000";

  sprintf(aux_dataLocacao, "%d/%d/%d", getMes(dataLocacao), getDia(dataLocacao), getAno(dataLocacao));

  for (i = 0; i < 11; i++) {
    int digit = getCPF(t->cliente)[i];
    aux_cpf[i] = digit + '0';
  }


  fprintf(temp, "%s,%s,%d,%s,%i\n", aux_dataLocacao, aux_dataDevolucao, t->dias_locado, aux_cpf, getId(t->livro));

  fclose(temp);
}

//função que chama todas as locações que ja foram relizadas.
tLocacao** getAllLocacoes(){
  return allLocacoes;
}

//função que atualiza o arquivo "locacoes.csv" toda vez que um livro é cadastrado.
void atualizaLocacaoFile() {
  int i = 0;
  FILE *alterado;
  alterado = fopen("./db/alt_locacoes.csv", "w");

  if (alterado == NULL) {
    printf("Erro ao abrir o arquivo ./db/alt_locacoes.csv\n");
  }
  
  tLocacao* t = allLocacoes[i];
  fprintf(alterado,"data_locacao, data_devolucao, dias_locados, cliente_cpf, livro_id\n");
  fclose(alterado);

  while (t!=NULL) {
    adicionarLocacaoFile(t, "./db/alt_locacoes.csv");
    i++;
    t = allLocacoes[i];
  }
  unlink("./db/locacoes.csv");
  rename("./db/alt_locacoes.csv", "./db/locacoes.csv");
}
////função que permite que o cliente devolva o livro para a biblioteca e ele se torne disponível para ser alugado novamente.
void devolverLivro(tLocacao* loc, tData* dataDevolucao) {
  setDisponibilidade(loc->livro , true);
  loc->data_devolucao = dataDevolucao;
}
//função que deleta as informacoes de uma locação.
void deletarLocacao(tLocacao* locacao) {
  deletarData(locacao->data_locacao);
  deletarData(locacao->data_devolucao);
  free(locacao);
  locacao = NULL;
}

//funcao que recebe um vetor de locações e deleta todas as locações presentes no vetor.
void deletarLocacaoList(tLocacao** loc) {
  int i = 0;
  tLocacao *t = loc[i];
  tLocacao *aux = loc[i+1];
  while(t != NULL){
    deletarLocacao(t);
    if (i == (locacoesCounter-1)) {
      break;
    }
    aux = loc[i+1];

    t = aux;
    i++;
  }
  free(loc);
}
//função que deleta todas as locações que foram feitas no sistema.
void deletarAllLocacoes() {
  deletarLocacaoList(allLocacoes);
}
//funcao que recebe a struct e retorna apenas a quantidade de dias no qual o livro ficou alugado.
int getDiasLocados(tLocacao* t) {
  return t->dias_locado;
}
//confere se a devolucao do livro foi atrasada ou não.
bool isAtrasado(tLocacao* t, tData* dataAtual){
  if (tempoEntreDatas(t->data_locacao, dataAtual) > t->dias_locado) {
    return 1;
  }
  return 0;
}
//veficia todas locacoes para achar a atual locacao de um tipo livro
tLocacao* getLocacao(tLivro* livro) {
  int i = 0;
  tLocacao* t = allLocacoes[i];
  while (t != NULL) {
    if (t->data_devolucao == NULL) {
      if (t->livro == livro) {
        return t;
      }
      i++;
      t = allLocacoes[i];
    }
  }
  return NULL;
}
//retorna todas as locacoes de um cliente
tLocacao** getLocacoes(tCliente* c, tLocacao** locs) {
  int i,j, counter = 0;

  for (i = 0; i < 100; i++) {
    locs[i] = NULL;
  }

  for (i = 0; i < locacoesCounter; i++) {
    tLocacao* aux = allLocacoes[i];

    bool equals = true;
    for (j = 0; j < 11; j++) {
      if (getCPF(getClienteLocacao(aux))[j] != getCPF(c)[j]) {
        equals = false;
      }
    }
    if (equals) {
      locs[counter] = aux;
      counter++;
    }
  }
  return locs;
}
//funcao que recebe a struct e retorna apenas o nome do cliente que realizou a locação.
tCliente* getClienteLocacao(tLocacao *t) {
  return t->cliente;
}
//funcao que recebe a struct e retorna apenas a data em que ocorreu a locação.
tData* getDataLocacao(tLocacao* t) {
  return t->data_locacao;
}
//funcao que recebe a struct e retorna apenas a data de devolução esperada ao se alugar o livro.
tData* getDataDevolucao(tLocacao* t) {
  return t->data_devolucao;
}

//funcao que recebe a struct e retorna apenas o nome do livro que foi alugado.
tLivro* getLivroLocacao(tLocacao* t) {
  return t->livro;
}
//função que retorna o valor da locação baseado no valor do livro, na quantidade de dias em que ficará alugado e se estará atrasado ou não.
double retorna_valor_locacao(tLocacao* t, tData* dataAtual) {
  double valor = getValorLivro(t->livro) * tempoEntreDatas(t->data_locacao, dataAtual);
  if (isAtrasado(t, dataAtual)) {
    int diasAtrasado = tempoEntreDatas(t->data_locacao, dataAtual);
    valor += 1.1*diasAtrasado*(getValorLivro(t->livro));
  }
  return valor;
}