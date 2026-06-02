#pragma once
#ifndef TRANZACTIE_H
#define TRANZACTIE_H

#include <iostream>
#include <string>
#include <type_traits>

class Intrare {};
class Iesire {};

template<typename TP>
class Tranzactie {
    int idProdus;
    int cantitate;
public:
    Tranzactie(int id, int c) : idProdus(id), cantitate(c) {}

    void afiseaza() const {
        if (std::is_same<TP, Intrare>::value) {
            std::cout << "[INTRARE/RESTOCK] ";
        }
        else {
            std::cout << "[IESIRE/VANZARE] ";
        }
        std::cout << "Produs ID: " << idProdus << " | Cantitate: " << cantitate << "\n";
    }
};

#endif