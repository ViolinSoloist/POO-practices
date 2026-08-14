/// @author Erik Min Soo Chung

#include <iostream>
#include <map>
#include <vector>
#include <memory>

using namespace std;

#define Pbp pair<bool, Produto*>

enum tipo_ {LIVRO, CD, DVD};
enum categoria_ {LEITURA, MIDIA};
map<Produto, int> m;


/// @brief @class mãe. Produtos podem ser @class Livros, @class CDs e @class DVDs, fazendo parte das categorias Leitura e Midia.
/// Todos eles têm nome, preco e código de barras único (passados no construtor).
class Produto {
protected:
    string nome;
    float preco;
    int codigo_barras;
    int tipo;
    int categoria;

public:
    Produto(string nome, float preco, int codigo) {
        this->nome = nome;
        this->preco = preco;
        this->codigo_barras = codigo;
    };
    virtual ~Produto() {}

    string getNome() {return this->nome;}
    float getPreco() {return this->preco;}
    int getCodigo() {return this->codigo_barras;}
    virtual int getTipo() = 0;
    virtual int getCategoria() = 0;
};

class Livro : public Produto {
public:
    Livro(string nome, float preco, int codigo) : Produto(nome, preco, codigo) {
        tipo = LIVRO;
        categoria = LEITURA;
    }
    ~Livro() {}

    int getTipo() {return tipo;}
    int getCategoria() {return categoria;}
};

class Cd : public Produto {
public:
    Cd(string nome, float preco, int codigo) : Produto(nome, preco, codigo) {
        tipo = CD;
        categoria = MIDIA;
    }
    virtual ~Cd() {}

    int getTipo() {return tipo;}
    int getCategoria() {return categoria;}
};

class Dvd : public Cd {
public:
    Dvd(string nome, float preco, int codigo) : Cd(nome, preco, codigo) {tipo = DVD;}
    ~Dvd() {}
};

// -----------------------------------------------------------------------------------------------
/// @brief @class principal Loja, responsável por armazenar (num vetor) e gerenciar @class Produtos 
class Loja {
private:
    vector<unique_ptr<Produto>> catalogo;
    float ganhos; // Quanto, em reais, a loja vendeu
    static const int MAX_PRODUTO = 30; // supondo que há um limite na quantidade de um mesmo produto
    static const int MAX_TOTAL = 100; // supondo que a loja tem uma capacidade máxima total

public:
    Loja() {catalogo.clear();}
    ~Loja() {}

    /// @brief MÉTODOS
    
    // ============ verificadores ====================
    int getSize() {return catalogo.size();}
    bool isEmpty() {return getSize() == 0;}
    bool isFull() {return getSize() >= MAX_TOTAL;}
    
    // =========== funcionalidades e procedimentos/métodos da loja  =================

    /// @return "true" e o produto, se acha correspondência, e "false" e um ponteiro nulo, se acontecer o oposto.
    /// @attention Busca apenas por código, não pelo nome. Justifica-se pelo fato de alguns comércios optarem por não usar o nome
    // por questões de incerteza e praticidade e clareza de usar apenas códigos únicos.
    Pbp buscaProduto(int codigo) {
        Pbp p = {false, NULL};
        if (isEmpty()) return p;

        for (auto& produto : catalogo) {
            Produto* pd = produto.get();
            if (pd->getCodigo() == codigo) p = {true, pd};
        }
        return p;
    }

    /// @extends buscaProduto()
    bool removeProduto(int codigo) {
        Pbp res = buscaProduto(codigo);
        if (!res.first) {return false;}
        
        /** @anchor O(2n)
         * @details itera novamente pelo vetor (essa função itera 2 vezes no total por catálogo)
         * Como continua sendo O(n) e n não tende a tamanhos grandes, o código tem dupla iteração
         * para reaproveitamento de código */
        for (auto it = catalogo.begin(); it != catalogo.end(); ++it) {
            if (it->get() == res.second) {
                catalogo.erase(it);
                break;
            }
        }
        return res.first;
    }

    /// solução @ref O(2n) 
    bool vendeProduto(int codigo) {
        Pbp res = buscaProduto(codigo);
        if (!res.first) {return false;}

        ganhos += res.second->getPreco();
        removeProduto(codigo); 
    }

    // ============== Procedimento de feedbacks (Checagem das funções via terminal) ===================
    void printBuscaProduto(int codigo) {
        Pbp res = buscaProduto(codigo);

        if (!res.first) {cout << "Produto de código" << codigo << " não encontrado." << endl << endl; return;}

        cout << "Produto de código " << codigo << " encontrado:" << endl << endl;
        cout << "Nome: " << res.second->getNome() << endl << "Preco: R$" << res.second->getPreco() << endl
            << "Categoria: " << res.second->getCategoria() << endl << "Produto: " << res.second->getTipo() << endl << endl;
    }

    void printRemoveProduto(int codigo) {
        if (removeProduto(codigo)) cout << "Produto de código " << codigo <<" removido com sucesso." << endl;
        else cout << "Falha na remocao: produto de código " << codigo << " não encontrado." << endl;
    }

    void printVendeProduto(int codigo) {
        if (vendeProduto(codigo)) cout << "Produto de código " << codigo << " vendido com sucesso." << endl;
        else cout << "Falha na venda: produto de código " << codigo << " não encontrado." << endl;
    }

};

// ------------------- MAIN -------------------------
int main() {

    return 0;
}