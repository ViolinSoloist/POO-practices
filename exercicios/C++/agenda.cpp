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

    virtual ~Contato() {} // destrutor

    // getters
    virtual string getDocumento() = 0;
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

    // getters (getData() é uma gororoba mas funciona e cabe em uma linha então é isso)
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
    string getData() {return to_string(nascimento.dia).append("/").append(to_string(nascimento.mes)).append("/").append(to_string(nascimento.ano));}
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

    // getters
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

// ---------------------------------------------------
// todas as ações possíveis de se fazer na agenda
// ---------------------------------------------------
class IAgenda {
public:
    virtual void addContato(up<Contato> c) = 0;
    virtual void removerContato(string termo) = 0;
    virtual void pesquisarContato(string termo) = 0;
    virtual void visualizarContatos() = 0;

    virtual ~IAgenda() {}
};

class Agenda : public IAgenda {
private:
    vector<up<Contato>> lista_contatos;
public:
    Agenda() {
        lista_contatos.clear();
    }

    ~Agenda() {}

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
    
};

int main() {
    Agenda minhaAgenda;

    // Criando dados de exemplo
    Data dataNasc = {15, 8, 1990};
    
    minhaAgenda.addContato(make_unique<PessoaFisica>("12345678901", "Ana Silva", "Rua A", dataNasc, "ana@email.com", SOLTEIRO));
    minhaAgenda.addContato(make_unique<PessoaJuridica>("12345678000199", "Tech Solutions", "Rua B", "contato@tech.com", "12345", "Tech Solutions Ltda"));

    // Visualizar
    minhaAgenda.visualizarContatos();

    // Pesquisar
    minhaAgenda.pesquisarContato("Ana Silva");

    // Remover
    minhaAgenda.removerContato("Ana Silva");
    
    // Visualizar novamente para confirmar remoção
    minhaAgenda.visualizarContatos();

    return 0;
}
