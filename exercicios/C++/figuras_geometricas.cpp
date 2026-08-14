#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <sstream>
#include <string>

using namespace std;

#define pb push_back

enum tipo_ {CIRCULO, RETANGULO, QUADRADO};
/**
 * @brief Toda figura geométrica tem cor e indicação se ela é preenchida ou não.
 * Além disso, todas elas têm métodos para calcular a área e o perímetro,
 * mas que serão implementadas especificamente de acordo com a figura em si.
 */
class FiguraGeometrica {
protected:
    string cor;
    bool filled;

    int tipo;

public:
    FiguraGeometrica(string cor, bool filled) {
        this->cor = cor;
        this->filled = filled;
    }
    virtual ~FiguraGeometrica() {}

    bool isFilled() {return filled;}
    virtual int getTipo() = 0;
    virtual float area() = 0;
    virtual float perimetro() = 0;
};

class Circulo : public FiguraGeometrica {
private:
    float raio;
    static constexpr const float PI = 3.14159265358979323846;

public:
    Circulo(float raio, string cor, bool filled) : FiguraGeometrica(cor, filled) {
        this->raio = raio;
        this->tipo = CIRCULO;
    }
    ~Circulo() {}

    float area() override {return PI * raio * raio;}
    float perimetro() override {return 2 * PI * raio;}
    int getTipo() override {return this->tipo;}
};

class Retangulo : public FiguraGeometrica {
protected:
    float lado1;
    float lado2;

public:
    Retangulo(float l1, float l2, string cor, bool filled) : FiguraGeometrica(cor, filled) {
        lado1 = l1;
        lado2 = l2;
        this->tipo = RETANGULO;
    }
    virtual ~Retangulo() {}

    virtual float area() override {return lado1 * lado2;}
    virtual float perimetro() override {return 2*(lado1 + lado2);}
    virtual int getTipo() override {return this->tipo;}
};

class Quadrado : public Retangulo {
public:
    Quadrado(float lado, string cor, bool filled) : Retangulo(lado, lado, cor, filled) {this->tipo = QUADRADO;}
    ~Quadrado() {}
};

int main() {
    vector<unique_ptr<FiguraGeometrica>> lista_formas;

    lista_formas.pb(make_unique<Circulo>(2.5, "rosa", true));
    lista_formas.pb(make_unique<Retangulo>(3, 4, "vermelho", false));
    lista_formas.pb(make_unique<Quadrado>(3, "azul", true));

    for (int i=0 ; i<lista_formas.size() ; i++) {
        stringstream ss;
        FiguraGeometrica* fg = lista_formas[i].get();
        
        string fig;
        switch (fg->getTipo()) {
        case CIRCULO:
            fig += "circulo";
            break;
        case RETANGULO:
            fig += "retangulo";
            break;
        case QUADRADO:
            fig += "quadrado";
            break;
        default:
            fig += "desenho indeterminado";
            break;
        }
        
        if (fg->isFilled()) {ss << "O " << fig <<" preenchido tem Area de " << fg->area() << " unidades ao quadrado." << endl;}
        else {ss << "O " << fig << " vazio tem Perimetro de " << fg->perimetro() << " unidades." << endl;}

        cout << ss.str();
    }

    return 0;
}