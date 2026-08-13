/// @author Erik Min Soo Chung

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

#define up unique_ptr

typedef struct data_ {
    int dia;
    int mes;
    int ano;
}Data;

enum estadoCivil {SOLTEIRO, CASADO, SEPARADO, DIVORCIADO, VIUVO};

/**
 * @brief Classe mãe, Contatos podem ser de Pessoas físicas ou Pessoas jurídicas
 *
 */
class Contato {
protected:
    string documento;
    string nome;
    string endereco;
    string email;

public:

    Contato(string documento, string nome, string endereco, string email) {
        this->documento = documento;
        this->nome = nome;
        this->endereco = endereco;
        this->email = email;
    }

    virtual ~Contato() {}

    // getters
    virtual string getDocumento() = 0; // implementado especificamente para formatação específica do documento
    string getRawDocumento() {return this->documento;}
    string getNome() {return this->nome;}
    string getEndereco() {return this->endereco;}
    string getEmail() {return this->email;}
};

class PessoaFisica : public Contato {
private:
    Data nascimento;
    estadoCivil estado_civil;

public:
    PessoaFisica(string cpf, string nome, string endereco, Data nascimento, string email, estadoCivil estado_civil) 
    : Contato(cpf, nome, endereco, email) {
        this->nascimento = nascimento;
        this->estado_civil = estado_civil;
    }

    ~PessoaFisica() {}

    // -------- GETTERS -----------------

    // retorna o documento formatado, ex: "123.456.789-01"
    string getDocumento() override {
        string str_cpf = "";
        if(this->documento.length() != 11) return this->documento;

        for (int i=0; i<11; i++) {
            if(i%3==0 && i!=0) str_cpf.append(".");
            else if(i==9) str_cpf.append("-");
            str_cpf.push_back(this->documento[i]);
        }
        return str_cpf;
    }

    // retorna a data de aniversario formatado, ex: 01/02/2026
    string getData() {return to_string(nascimento.dia).append("/").append(to_string(nascimento.mes)).append("/").append(to_string(nascimento.ano));}
    // retorna o estado civil no tipo enum (int)
    estadoCivil getEstadoCivil() {return this->estado_civil;}
};

class PessoaJuridica : public Contato {
private:
    string inscricao_estadual;
    string razao_social;

public:
    PessoaJuridica(string cnpj, string nome, string endereco, string email, string inscricao_estadual, string razao_social) 
    : Contato(cnpj, nome, endereco, email) {
        this->inscricao_estadual = inscricao_estadual;
        this->razao_social = razao_social;
    }

    ~PessoaJuridica() {}

    // ---------- GETTERS --------------

    // retorna o CNPJ formatado, ex: "-12.345.678/0001-99"
    string getDocumento() override {
        string str_cnpj = "";
        if(this->documento.length() != 14) return this->documento;

        for (int i=0 ; i<14 ; i++) {
            if (i==2 || i==5) str_cnpj.append(".");
            else if (i==8) str_cnpj.append("/");
            else if (i==12) str_cnpj.append("-");
            str_cnpj.push_back(this->documento[i]);
        }
        return str_cnpj;
    }
    string getInscricaoEstadual() {return this->inscricao_estadual;}
    string getRazaoSocial() {return this->razao_social;}
};

// ------------------------------------------------------
// Interface para o usuário das funcionalidades da agenda
// ------------------------------------------------------
class IAgenda {
public:
    virtual bool agendaVazia() = 0;
    virtual int agendaSize() = 0;
    virtual void addContato(up<Contato> c) = 0;
    virtual void removerContato(string termo) = 0; // Remocao por nome ou documento
    virtual void pesquisarContato(string termo) = 0; // Pesquisa por nome ou documento
    virtual void visualizarContatos() = 0;
    // Pessoas Físicas aparecem antes das Pessoas Jurídicas. A ordenação é por CPF/CNPJ
    virtual void ordenarContatos() = 0;

    virtual ~IAgenda() {}
};

class Agenda : public IAgenda {
private:
    vector<up<Contato>> lista_contatos;

    // usado apenas no Algoritmo de Ordenacao. Fins didáticos, no código abaixo será usado a função swap() nativa do C++.
    void swapContatos(up<Contato>& c1, up<Contato>& c2) {
        up<Contato> temp;
        temp = move(c1);
        c1 = move(c2);
        c2 = move(temp);
    }

    /**
     * @brief compara de @param c1 > @param c2 , em outras palavras, se c2 deve vir antes de c1 na ordenacao.
     * Pessoa física (CPF de tamanho 11) é considerado "menor" que Pessoa Juridica (CNPJ de tamanho 14),
     * ou seja, aquela vem antes desta.
     * 
     * @return true se c1 > c2 (se c2 vem antes de c1 quando ordenado)
     * @return false se for ao contrário.
     */
    bool isMaior(Contato* c1, Contato* c2) {
        string doc1 = c1->getRawDocumento();
        string doc2 = c2->getRawDocumento();

        // CPF (tam 11) vem antes de CNPJ (tam 14)
        if (doc1.length() != doc2.length()) {return doc1.length() > doc2.length();}
        
        // se for o mesmo tamanho (logo, o mesmo tipo), compara lexicograficamente
        return doc1.compare(doc2) > 0;
    }

    // recursivamente pega um elemento de um vetor e faz trocas com seus filhos
    // se necessário para manter a relação pai > filho1, filho2 de um elemento de uma heap.
    void heapfy(int tam, int i) {
        int maior = i;
        int l = 2*i + 1;
        int r = 2*i + 2;

        if (l < tam && isMaior(lista_contatos[l].get(), lista_contatos[maior].get()))
            maior = l;

        if (r < tam && isMaior(lista_contatos[r].get(), lista_contatos[maior].get()))
            maior = r;

        if (maior != i) { // se maior for direrente de i, quer dizer que o maior agora é um dos filhos
            swap(lista_contatos[maior], lista_contatos[i]);
            heapfy(tam, maior);
        }
    }

    // pega um vetor comparável e transforma numa heap, usando  heapfy
    void makeHeapContatos() {for(int i=agendaSize()/2 - 1 ; i>=0 ; i--) heapfy(agendaSize(), i);}

    // assume que o vetor já está orginalmente em heap para ordenar usando HeapSort
    void heapSort(int n) {
        if (n<2) return;
        // logo, no início dessa função, o início do vetor já é o maior e deve ser trocando com o último.
        swap(lista_contatos[0], lista_contatos[n-1]); 
        heapfy(n-1, 0); //organizamos a árvore reduzida, pelo início, que agora tem um valor menor.
        heapSort(n-1);       
    }

public:
    Agenda() {
        lista_contatos.clear();
    }

    ~Agenda() {}

    int agendaSize() override {return lista_contatos.size();}
    bool agendaVazia() override {return (agendaSize() == 0 ? true : false);}

    void addContato(up<Contato> c) override {
        lista_contatos.push_back(move(c));
        cout << "Contato adicionado com sucesso!" << endl;
    }

    void removerContato(string termo) override {
        for (auto it = lista_contatos.begin(); it != lista_contatos.end(); ++it) {
            if ((*it)->getDocumento() == termo || (*it)->getNome() == termo) {
                lista_contatos.erase(it);
                cout << "Contato '" << termo << "' removido com sucesso!" << endl;
                return;
            }
        }
        cout << "Erro: Contato '" << termo << "' não encontrado para remover!" << endl;
    }

    // pesquisa é por nome ou documento
    void pesquisarContato(string termo) override {
        for (auto& contato : lista_contatos) {
            if (contato->getDocumento() == termo || contato->getNome() == termo) {
                cout << "Contato encontrado: " << contato->getNome() << endl;
                return;
            }
        }
        cout << "Erro: Contato não encontrado!" << endl;
    }

    void visualizarContatos() override {
        cout << "===================\n" << "Lista de contatos\n" << "===================\n" << endl;
        for (auto& contato : lista_contatos) {
            cout << "Nome: " << contato->getNome() << ", Documento: " << contato->getDocumento() << endl;
        }
    }

    void ordenarContatos() override { // usa HeapSort
        if (agendaVazia()) {cout << "Aviso: A agenda está vazia." << endl; return;}
        if (agendaSize() == 1) return;

        makeHeapContatos();
        heapSort(agendaSize());
    }
    
};

int main() {
    Agenda minhaAgenda;
    Data dataNasc = {15, 8, 1990};
    
    minhaAgenda.addContato(make_unique<PessoaJuridica>("99999999000199", "Z Tech", "Rua C", "z@email.com", "999", "Z Ltda"));
    minhaAgenda.addContato(make_unique<PessoaFisica>("99999999999", "Bruno", "Rua B", dataNasc, "bruno@email.com", SOLTEIRO));
    minhaAgenda.addContato(make_unique<PessoaJuridica>("12345678000199", "A Solutions", "Rua A", "a@email.com", "123", "A Ltda"));
    minhaAgenda.addContato(make_unique<PessoaFisica>("12345678901", "Ana", "Rua A", dataNasc, "ana@email.com", SOLTEIRO));

    cout << "\n--- ANTES DA ORDENACAO ---";
    minhaAgenda.visualizarContatos();
    
    // teste de ordenação
    minhaAgenda.ordenarContatos();

    cout << "\n--- DEPOIS DA ORDENACAO ---";
    minhaAgenda.visualizarContatos();

    return 0;
}