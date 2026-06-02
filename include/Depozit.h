#pragma once
#ifndef DEPOZIT_H
#define DEPOZIT_H

#include <unordered_map>
#include <vector>
#include <string>
#include "Produs.h"
#include "Tranzactie.h"

class Depozit {
    std::unordered_map<int, Produs> produse;
    std::vector<Tranzactie<Intrare>> istoricIntrari;
    std::vector<Tranzactie<Iesire>> istoricIesiri;
public:
    void adauga(const Produs& p);
    void actualizeaza(int id, int diferenta);
    void raportSubPrag() const;
    void recomandari() const;
    void incarcaCSV(std::string fisier);
    void salveazaCSV(std::string fisier);
    void afiseazaTranzactii() const;
};

#endif