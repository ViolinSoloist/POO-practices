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
// Todos eles têm nome, preco e código de barras único (passados no construtor).
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
    
    // verificadores
    int getSize() {return catalogo.size();}
    bool isEmpty() {return getSize() == 0;}
    bool isFull() {return getSize() >= MAX_TOTAL;}
    
    // funcionalidades e procedimentos/métodos da loja 

    /// @brief @returns true e o produto se acha correspondência e false e um ponteiro nulo se acontecer o oposto
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
        catalogo.erase(remove(catalogo.begin(), catalogo.end(), res.second), catalogo.end());
        return res.first;
    }

    bool vendeProduto(int codigo) {
        Pbp res = buscaProduto(codigo);
        if (!res.first) {return false;}

        ganhos += res.second->getPreco();
        removeProduto(codigo); // solução O (2n)
    }

    // Procedimento de feedbacks (Checagem das funções via terminal)
    void printBuscaProduto(int codigo) {
        Pbp res = buscaProduto(codigo);

        if (!res.first) {cout << "Produto não encontrado." << endl << endl; return;}

        cout << "Produto encontrado:" << endl << endl;
        cout << "Nome: " << res.second->getNome() << endl << "Preco: R$" << res.second->getPreco() << endl
            << "Categoria: " << res.second->getCategoria() << endl << "Produto: " << res.second->getTipo() << endl << endl;
    }

    void printRemoveProduto(int codigo) {
        if (removeProduto(codigo)) cout << "Produto removido com sucesso." << endl;
        else cout << "Falha na remocao: produto não encontrado." << endl;
    }

};

// ------------------- MAIN -------------------------
int main() {

    return 0;
}