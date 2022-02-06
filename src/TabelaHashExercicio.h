#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

template <typename K, typename V>
class Tupla
{
private:
    K chave;
    V valor;
    Tupla *prox;

public:
    Tupla(K c, V v)
    {
        chave = c;
        valor = v;
        prox = NULL;
    }

    K getChave()
    {
        return chave;
    }

    V getValor()
    {
        return valor;
    }

    Tupla *getProx()
    {
        return prox;
    }

    void setProx(Tupla *prox)
    {
        Tupla::prox = prox;
    }
};

template <typename Chave, typename Valor>
class TabelaHash
{

private:
    Tupla<Chave, Valor> **tabela;

    // tamanho atual do array
    int qtde_buckets;
    // qtdade de elementos já inseridos na tabela hash
    int tamanho;

    int hashIndex(Chave c)
    {
        hash<string> str_hash;

        int int_hash = str_hash(c);

        int abs_hash = abs(int_hash);

        int indice = abs_hash % qtde_buckets;

        return indice;
    }

    /**
     * Função para inserir a tupla <c,v> na tabela.
     * É preciso calcular o código hash a partir da chave c.
     * Em seguida, usamos qtde_buckets para descobrir o índice
     * dessa tupla. Prestar atenção na alocação de tupla quando
     * há colisões. A função hash às vezes retorna valores
     * negativos: use abs() para evitar índices negativos.
     **/
    void inserir(Chave c, Valor v, Tupla<Chave, Valor> **tabela)
    {

        int indice = hashIndex(c);

        if (tabela[indice] == NULL)
        {
            Tupla<Chave, Valor> *tupla = new Tupla(c, v);
            tabela[indice] = tupla;
        }
        else
        {
            Tupla<Chave, Valor> *aux = tabela[indice];

            while (aux->getProx() != NULL)
            {
                aux = aux->getProx();
            }
            aux->setProx(new Tupla(c, v));
        }
        tamanho++;
    }

    /**
     * Função para aumentar o tamanho do array quando o
     * fator de carga for >= 1. O tamanho do array
     * (qtde_buckets) deve ser multiplicado por 8 para
     * que essa operação seja feita com pouca frequência.
     * Por fim, precisamos reposicionar as tuplas, considerando
     * que a posição nesse novo array com maior tamanho
     * será diferente.
     **/
    void aumentaArray()
    {
        int old_qtde_buckets = qtde_buckets;
        qtde_buckets *= 8;

        TabelaHash<string, int> tabelaAntiga;
        tabelaAntiga.tabela = tabela;
        tabelaAntiga.qtde_buckets = old_qtde_buckets;

        tabela = (Tupla<Chave, Valor> **)calloc(qtde_buckets, sizeof(Tupla<Chave, Valor>));
        tamanho = 0;

        for (int i = 0; i < old_qtde_buckets; i++)
        {
            if (tabelaAntiga.tabela[i] != NULL)
            {
                Tupla<Chave, Valor> *aux = new Tupla<Chave, Valor>(tabelaAntiga.tabela[i]->getChave(), tabelaAntiga.tabela[i]->getValor());
                aux->setProx(tabelaAntiga.tabela[i]->getProx());
                Chave old_chave = tabelaAntiga.tabela[i]->getChave();
                inserir(old_chave, tabelaAntiga.tabela[i]->getValor());

                while (aux->getProx() != NULL)
                {
                    aux = aux->getProx();
                    inserir(aux->getChave(), aux->getValor());
                }
            }
        }
    }

public:
    /**
     * Inicializar o array de tuplas com capacidade = qtde_buckets.
     * Lembrar de setar todas as posições do array inicializado
     * para NULL.
     **/
    TabelaHash()
    {
        int capacidade;
        if (qtde_buckets == 0)
        {
            capacidade = 8;
            qtde_buckets = 8;
        }
        else
        {
            capacidade = qtde_buckets;
        }

        tabela = (Tupla<Chave, Valor> **)calloc(capacidade, sizeof(Tupla<Chave, Valor>));
    }

    /**
     * Essa é a função pública que nos permite inserir
     * uma tupla <c,v> na tabela. Nessa função nós
     * verificamos se é preciso aumentar o array de acordo
     * com o load factor. Essa função pode reusar sua versão
     * sobrecarregada para inserir a tupla na tabela.
     * Essa função também incrementa a quantidade de elementos
     * na tabela (variável tamanho).
     **/
    void inserir(Chave c, Valor v)
    {
        double lf = load_factor();
        if (lf >= 1)
        {
            aumentaArray();
        }
        TabelaHash::inserir(c, v, tabela);
    }

    /**
     * Essa função retorna o fator de carga da Tabela Hash.
     **/
    double load_factor()
    {

        return (double)tamanho / (double)qtde_buckets;
    }

    /**
     * Há apenas um valor associado a uma chave.
     * Essa função retorna esse valor, caso a chave exista.
     * Se a chave não existir a função retorna NULL.
     * Lembre-se: em caso de colisão, é preciso navegar
     * no bucket (lista ligada) para ter certeza se a chave
     * existe ou não.
     **/
    Valor getValor(Chave chave)
    {
        int indice = hashIndex(chave);

        if (tabela[indice] != NULL)
        {
            Chave chaveBucket = tabela[indice]->getChave();
            if (chaveBucket == chave)
            {
                return tabela[indice]->getValor();
            }
            else if (tabela[indice]->getProx() != NULL)
            {
                Chave prox = (tabela[indice]->getProx())->getChave();
                Tupla<Chave, Valor> *aux = tabela[indice];

                while (aux->getProx() != NULL)
                {

                    aux = aux->getProx();
                    Chave chaveAux = aux->getChave();
                    if (chaveAux == chave)
                    {
                        return aux->getValor();
                    }
                }
            }
        }
        return 0;
    }

    /**
     * Essa função retorna true caso a chave exista,
     * false caso contrário.
     * Lembre-se: em caso de colisão, é preciso navegar
     * no bucket (lista ligada) para ter certeza se a chave
     * existe ou não.
     **/
    bool contemChave(Chave chave)
    {

        int indice = hashIndex(chave);

        if (tabela[indice] != NULL)
        {
            Chave chaveBucket = tabela[indice]->getChave();
            if (chaveBucket == chave)
            {
                return true;
            }
            else if (tabela[indice]->getProx() != NULL)
            {
                Chave prox = (tabela[indice]->getProx())->getChave();
                Tupla<Chave, Valor> *aux = tabela[indice];

                while (aux->getProx() != NULL)
                {

                    aux = aux->getProx();
                    Chave chaveAux = aux->getChave();
                    if (chaveAux == chave)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    /**
     * Essa função retorna um vetor com todas as chaves
     * já inseridas na tabela.
     **/
    vector<Chave> getChaves()
    {

        vector<Chave> chaves;
        for (int i = 0; i < qtde_buckets; i++)
        {
            if (tabela[i] != NULL)
            {
                Chave chaveBucket = tabela[i]->getChave();
                chaves.push_back(chaveBucket);

                Tupla<Chave, Valor> *aux = tabela[i];

                while (aux->getProx() != NULL)
                {
                    Chave chaveProx = aux->getProx()->getChave();
                    chaves.push_back(chaveProx);
                    aux = aux->getProx();
                }
            }
        }
        return chaves;
    }

    /**
     * Essa função desaloca os nós previamente alocados, e muda
     * o tamanho do array de tuplas para 8.
     **/
    void clear()
    {

        free(tabela);
        qtde_buckets = 0;
        tamanho = 0;
    }

    /**
     * Há apenas um nó associado com uma mesma chave.
     * Essa função remove esse nó da tabela, caso a chave exista.
     * Se a chave não existir a função não faz nada.
     * Lembre-se: em caso de colisão, é preciso navegar
     * no bucket (lista ligada) para ter certeza se a chave
     * existe ou não.
     * Dica: olhar algoritmo de remoção em lista ligada, pois
     * após a remoção de um nó a lista precisa permanecer íntegra,
     * ou seja, navegável.
     **/
    void remover(Chave chave)
    {

        int indice = hashIndex(chave);

        if (tabela[indice] != NULL)
        {
            Chave chaveBucket = tabela[indice]->getChave();
            if (chaveBucket == chave)
            {
                tabela[indice] = tabela[indice]->getProx();
                tamanho--;
            }
            else if (tabela[indice]->getProx() != NULL)
            {
                Chave prox = (tabela[indice]->getProx())->getChave();
                Tupla<Chave, Valor> *aux = tabela[indice];

                while (aux->getProx() != NULL)
                {

                    aux = aux->getProx();
                    Chave chaveAux = aux->getChave();
                    if (chaveAux == chave)
                    {
                        aux = aux->getProx();
                    }
                }
            }
        }
    }

    /**
     * Essa função retorna a quantidade de pares
     * que já foram inseridos na Tabela Hash.
     **/
    int size()
    {
        return tamanho;
    }

    /**
     * Essa função retorna o tamanho atual do
     * array usado para armazenar a Tabela Hash.
     **/
    int bucket_count()
    {
        return qtde_buckets;
    }
};