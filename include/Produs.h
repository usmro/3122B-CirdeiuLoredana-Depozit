#pragma once
#ifndef PRODUS_H
#define PRODUS_H

#include <string>
#include "Furnizor.h"

class Produs {
    int id;
    std::string nume;
    int cantitate;
    double pret;
    int prag;
    Furnizor furnizor;
public:
    Produs();
    Produs(int i, std::string n, int c, double p, int pr, Furnizor f = Furnizor());

    int getId() const;
    std::string getNume() const;
    int getCantitate() const;
    double getPret() const;
    int getPrag() const;
    Furnizor getFurnizor() const;

    Produs& operator+=(int q);
    Produs& operator-=(int q);
};

#endif