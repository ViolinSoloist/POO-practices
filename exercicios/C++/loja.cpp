#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;

#define Pbp pair<bool, Produto*>

enum TipoProduto {LIVRO, CD, DVD};
enum Categoria {LEITURA, MIDIA};

/// @brief Classe a ser herdada. Produtos podem ser Livros, CDs e DVDs, fazendo parte das categorias Leitura e Midia.
/// Todos eles têm nome, preço e código de barras único para o tipo de produto, (passados no construtor).
class Produto {
protected:
    string nome;
    float preco;
    int codigo_barras;

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
    
    // Funções virtuais puras
    virtual TipoProduto getTipo() = 0;
    virtual Categoria getCategoria() = 0;
    virtual string getNomeCategoria() = 0;
};

class Livro : public Produto {
public:
    Livro(string nome, float preco, int codigo) : Produto(nome, preco, codigo) {}
    ~Livro() {}

    TipoProduto getTipo() override {return LIVRO;}
    Categoria getCategoria() override {return LEITURA;}
    string getNomeCategoria() override {return "Leitura";}
};

class Cd : public Produto {
public:
    Cd(string nome, float preco, int codigo) : Produto(nome, preco, codigo) {}
    virtual ~Cd() {}

    TipoProduto getTipo() override {return CD;}
    Categoria getCategoria() override {return MIDIA;}
    string getNomeCategoria() override {return "Midia (CD)";}
};

class Dvd : public Cd {
public:
    Dvd(string nome, float preco, int codigo) : Cd(nome, preco, codigo) {}
    ~Dvd() {}

    TipoProduto getTipo() override {return DVD;}
    Categoria getCategoria() override {return MIDIA;}
    string getNomeCategoria() override {return "Midia (DVD)";}
};

// -------------------------------- Loja ---------------------------------------------------


/// @brief Estrutura (pair) auxiliar para guardar o produto e a sua quantidade no stock
struct ItemEstoque {
    unique_ptr<Produto> produto;
    int quantidade;
};

/// @brief Interface para o usuário sobre as funcionalidades públicas da Loja.
class InterfaceLoja {
public:
    ~InterfaceLoja(){}

    /// @brief Procura um produto por código. Retorna um ponteiro bruto (nullptr se não encontrar).
    virtual Produto* buscarPorCodigo(int codigo) = 0;
    /// @brief O mesmo princípio da busca por código, mas pesquisa por nome.
    virtual Produto* buscarPorNome(string nome) = 0;

    /// @brief Adiciona um novo produto ao catálogo com uma quantidade inicial.
    virtual void adicionarProduto(unique_ptr<Produto> pd, int quantidade) = 0;

    /// @brief Vende uma unidade do produto, se houver stock
    virtual void venderProduto(int codigo) = 0;

    /// @brief Mostra todo o catálogo, a quantidade de cada um, e faz a soma por categoria
    virtual void relatorioEstoque() = 0;
};
/// @anchor Interface

/// @brief Classe principal Loja, responsável por armazenar (num vetor) e gerenciar Produtos 
class Loja : public InterfaceLoja {
private:
    vector<ItemEstoque> estoque;
    float ganhos = 0.0; // Quanto, em reais, a loja vendeu

    /// @returns Ponteiro para a estrutura ItemEstoque, se tiver correspondência.
    ItemEstoque* buscarItemEstoque(int codigo) {
        for (auto& item : estoque) {
            if (item.produto->getCodigo() == codigo) return &item; 
        }
        return nullptr; 
    }

public:
    Loja() {estoque.clear();}
    ~Loja() {}

    // -------------- MÉTODOS DE BUSCA --------------------
    
    Produto* buscarPorCodigo(int codigo) override {
        ItemEstoque* item = buscarItemEstoque(codigo);
        if (item != nullptr) return item->produto.get();
        return nullptr;
    }

    Produto* buscarPorNome(string nome) override {
        for (auto& item : estoque) {
            if (item.produto->getNome() == nome) return item.produto.get();
        }
        return nullptr;
    }
    
    // ----------------- MÉTODOS DA LOJA -----------------

    void adicionarProduto(unique_ptr<Produto> pd, int quantidade) override {
        if (quantidade <= 0) return;

        // Verifica se já existe para evitar duplicações de código de barras
        if (buscarPorCodigo(pd->getCodigo()) != nullptr) {
            cout << "Erro: O Produto com codigo " << pd->getCodigo() << " ja existe!=." << endl;
            return;
        }

        ItemEstoque novoItem;
        novoItem.produto = move(pd);
        novoItem.quantidade = quantidade;
        
        estoque.push_back(move(novoItem));
        cout << "[SUCESSO] Adicionados " << quantidade << " itens do produto '" << estoque.back().produto->getNome() << "'." << endl;
    }

    void venderProduto(int codigo) override {
        ItemEstoque* item = buscarItemEstoque(codigo);

        
        if (item == nullptr) throw invalid_argument("Codigo " + to_string(codigo) + " nao existe no catalogo.\n");
        if (item->quantidade <= 0) throw out_of_range("O produto '" + item->produto->getNome() + " está esgotado.");

        item->quantidade--;
        ganhos += item->produto->getPreco();
        cout << "[VENDA] Vendido 1x '" << item->produto->getNome() << "' por R$" << item->produto->getPreco() << "." << endl;
    }

    void relatorioEstoque() override {
        cout << "\n================ RELATÓRIO DE ESTOQUE ================" << endl;
        
        int totalLeitura = 0;
        int totalMidia = 0;

        for (auto& item : estoque) {
            cout << "Cod: " << item.produto->getCodigo() 
                 << " | Nome: " << item.produto->getNome() 
                 << " | Cat: " << item.produto->getNomeCategoria() 
                 << " | Qtd: " << item.quantidade << " unidades" << endl;
            
            // Somatório por Categoria
            if (item.produto->getCategoria() == LEITURA) totalLeitura += item.quantidade;
            if (item.produto->getCategoria() == MIDIA) totalMidia += item.quantidade;
        }

        cout << "\n--- Resumo por Categoria ---" << endl;
        cout << "Itens de Leitura (Livros): " << totalLeitura << " unidades." << endl;
        cout << "Itens de Mídia (CDs/DVDs): " << totalMidia << " unidades." << endl;
        cout << "Total ganho em vendas: R$" << ganhos << endl;
        cout << "======================================================\n" << endl;
    }
};

// ------------------- MAIN -------------------------
int main() {
    /// Verificar a @ref Interface para saber os métodos/funcionalidades da Loja.
    Loja cudeLoja;

    // Adicionar Produtos (com quantidades)
    cudeLoja.adicionarProduto(make_unique<Livro>("O Senhor dos Aneis", 59.90, 101), 10);
    cudeLoja.adicionarProduto(make_unique<Cd>("The Dark Side of the Moon", 29.90, 201), 5);
    cudeLoja.adicionarProduto(make_unique<Dvd>("Matrix", 39.90, 301), 2);

    // Buscas
    cout << "\n--- Teste de Busca ---" << endl;
    Produto* p = cudeLoja.buscarPorNome("Matrix");
    if (p != nullptr) cout << "Encontrado pelo nome: " << p->getNome() << " (Cod: " << p->getCodigo() << ")" << endl;

    // Vendas e Stock
    
    /// @brief Representa o "carrinho de compras" com os códigos dos produtos que o cliente deseja comprar.
    vector<int> carrinhoDeCompras = {101, 301, 301, 301, 101, 999};

    for (int i = 0; i < carrinhoDeCompras.size(); i++) {
        int codigoAtual = carrinhoDeCompras[i];
        
        try {cudeLoja.venderProduto(codigoAtual);} 
        catch (const out_of_range& erro) {cout << "[FALHA NA VENDA - ESTOQUE] " << erro.what() << endl;}
        catch (const invalid_argument& erro) {cout << "[FALHA NA VENDA - SISTEMA] " << erro.what() << endl;}
    }

    // Relatório formatado
    cudeLoja.relatorioEstoque();

    return 0;
}