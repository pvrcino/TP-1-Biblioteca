//pasta gerada para agrupar todas as funções relacionadas aos relatórios.
#include "../headers/Relatorio.h"
//função criada para realizar os relatórios dos clientes no menu4.
void relatorioClientes() {
  int i = 0, j = 0, digito;
  tCliente* c =  getAllClientes()[i];
  char aux_cpf[13] = "";
  tLocacao* locacoes[100];

//abertura do arquivo "relatorio_clientes.txt"
  FILE *temp;
  temp = fopen("./relatorios/relatorio_clientes.txt", "w");
  //printa os dados do cliente
  while (c != NULL) {
    for (j = 0; j < 11; j++) {
      digito = getCPF(c)[j];
      aux_cpf[j] = digito + '0';
    }
    fprintf(temp, "Nome: %s\n", getNomeCliente(c));
    fprintf(temp, "CPF: %s\n", aux_cpf);
    fprintf(temp, "Locações:\n");

    //procura todas locacoes do cliente c
    getLocacoes(c, locacoes);

    j = 0;
    tLocacao* loc = locacoes[j];
    //varre pelas locacoes e imprime no arquivo de saida
    while (loc != NULL) {

      fprintf(temp,"    Titulo do Livro: %s\n", getTitulo(getLivroLocacao(loc)));

      fprintf(temp, "   (V) Data de Locação: %i/%i/%i\n", getDia(getDataLocacao(loc)), getMes(getDataLocacao(loc)), getAno(getDataLocacao(loc)));

      tData* data_devolucao = getDataDevolucao(loc);
      //caso o livro não tenha sido devolvido, printa a data de entrega com um (X), caso contrario, com um (V)
      if (data_devolucao == NULL) {
        int diaEntrega = diaDoAno(getDataLocacao(loc)) + getDiasLocados(loc);
        data_devolucao = getData(diaEntrega, getAno(getDataLocacao(loc)));
        fprintf(temp, "   (X) ");
        fprintf(temp, "Data de Devolução: %i/%i/%i\n", getDia(data_devolucao), getMes(data_devolucao), getAno(data_devolucao));
        deletarData(data_devolucao);
      } else {
        fprintf(temp, "   (V) ");
        fprintf(temp, "Data de Devolução: %i/%i/%i\n", getDia(data_devolucao), getMes(data_devolucao), getAno(data_devolucao));
      }

      fprintf(temp,"    Valor total: %.2lf\n", retorna_valor_locacao(loc, data_devolucao));

      fprintf(temp,"\n");
      j++;
      loc = locacoes[j];
    }

    fprintf(temp,"\n");
    c = getAllClientes()[i+1];
    i++;
  }
  fclose(temp);
}

void relatorioLocacoes() {
  FILE *temp;
  //abertura do arquivo "relatorio_locacoes.txt"
  temp = fopen("./relatorios/relatorio_locacoes.txt", "w");

  int i = 0;
  tLocacao* t = getAllLocacoes()[i];
  while (t != NULL) {

    fprintf(temp, "Nome do Cliente: %s\n", getNomeCliente(getClienteLocacao(t)));
    fprintf(temp, "Título do Livro: %s\n", getTitulo(getLivroLocacao(t)));
    fprintf(temp, "ID do Livro: %d\n", getId(getLivroLocacao(t)));
    fprintf(temp, "(V) Data de Locação: %i/%i/%i\n", getDia(getDataLocacao(t)), getMes(getDataLocacao(t)), getAno(getDataLocacao(t)));

    tData* data_devolucao = getDataDevolucao(t);

    if (data_devolucao == NULL) {
      int diaEntrega = diaDoAno(getDataLocacao(t)) + getDiasLocados(t);
      data_devolucao = getData(diaEntrega, getAno(getDataLocacao(t)));
      fprintf(temp, "(X) ");
      fprintf(temp, "Data de Devolução: %i/%i/%i\n", getDia(data_devolucao), getMes(data_devolucao), getAno(data_devolucao));

    
    } else {
      fprintf(temp, "(V) ");
      fprintf(temp, "Data de Devolução: %i/%i/%i\n", getDia(data_devolucao), getMes(data_devolucao), getAno(data_devolucao));
    }

    fprintf(temp, "Valor total: %.2lf\n", retorna_valor_locacao(t, data_devolucao));

    fprintf(temp, "Atrasado: ");

    time_t timer = time(NULL);
    struct tm tm = *localtime(&timer);
    tData* data_atual = criarData(tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    if (isAtrasado(t, data_atual)) {
      fprintf(temp,"sim\n\n");
    } else {
      fprintf(temp,"não\n\n");
    }
    deletarData(data_atual);

    i++;
    t = getAllLocacoes()[i];
  }

  fclose(temp);
}

void relatorioLivros() {
  int i = 0, j = 0;
  FILE *temp;
  temp = fopen("./relatorios/relatorio_livros.txt", "w");
  tLivro* t = getAllLivros()[i];
  while (t != NULL) {

    fprintf(temp, "Título: %s\n", getTitulo(t));
    fprintf(temp, "Autores: ");

    tAutor* autor = retorna_autores(t)[j];
    tAutor* aux;
    while (autor != NULL) {
      fprintf(temp, "%s", getNomeAutor(autor));
      j++;
      aux = retorna_autores(t)[j];
      if (aux != NULL) {
        fprintf(temp,", ");
      }
      autor = aux;
    }
    fprintf(temp,"\n");
    fprintf(temp, "Valor: %.2lf\n", getValorLivro(t));
    fprintf(temp, "Locado por %d vezes\n\n", getLocacoesQnt(t));

    i++;
    j = 0;
    t = getAllLivros()[i];
  }

  fclose(temp);
}