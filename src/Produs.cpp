#include "Produs.h"
#include <stdexcept>

Produs::Produs() : id(0), cantitate(0), pret(0.0), prag(0) {}
Produs::Produs(int i, std::string n, int c, double p, int pr, Furnizor f) :
    id(i), nume(n), cantitate(c), pret(p), prag(pr), furnizor(f) {
}

int Produs::getId() const { return id; }
std::string Produs::getNume() const { return nume; }
int Produs::getCantitate() const { return cantitate; }
double Produs::getPret() const { return pret; }
int Produs::getPrag() const { return prag; }
Furnizor Produs::getFurnizor() const { return furnizor; }

Produs& Produs::operator+=(int q) {
    cantitate += q;
    return *this;
}

Produs& Produs::operator-=(int q) {
    if (q > cantitate) throw std::runtime_error("Stoc insuficient.");
    cantitate -= q;
    return *this;
}