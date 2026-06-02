#include "Depozit.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iomanip> // Necesar pentru formatarea coloanelor din tabel

// Re-definim culorile local pentru rapoarte
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

void Depozit::adauga(const Produs& p) {
    if (produse.count(p.getId())) {
        throw std::runtime_error("Produsul cu acest ID exista deja!");
    }
    produse[p.getId()] = p;
}

void Depozit::actualizeaza(int id, int diferenta) {
    if (!produse.count(id)) {
        throw std::invalid_argument("Produsul nu a fost gasit (ID invalid).");
    }
    if (diferenta >= 0) {
        produse[id] += diferenta;
        istoricIntrari.push_back(Tranzactie<Intrare>(id, diferenta));
    }
    else {
        produse[id] -= (-diferenta);
        istoricIesiri.push_back(Tranzactie<Iesire>(id, -diferenta));
    }
}

void Depozit::raportSubPrag() const {
    std::cout << RED << BOLD << "\n===================================================================\n";
    std::cout << "               RAPORT: PRODUSE SUB PRAGUL DE ALERTA                \n";
    std::cout << "===================================================================\n" << RESET;

    // Header Tabel
    std::cout << BOLD << std::left
        << std::setw(8) << "ID"
        << std::setw(30) << "Nume Produs"
        << std::setw(12) << "Stoc Curent"
        << std::setw(12) << "Prag Alerta" << RESET << "\n";
    std::cout << "-------------------------------------------------------------------\n";

    bool gasit = false;
    for (const auto& x : produse) {
        if (x.second.getCantitate() < x.second.getPrag()) {
            std::cout << std::left
                << std::setw(8) << x.second.getId()
                << std::setw(30) << x.second.getNume()
                << RED << std::setw(12) << x.second.getCantitate() << RESET
                << std::setw(12) << x.second.getPrag() << "\n";
            gasit = true;
        }
    }

    if (!gasit) {
        std::cout << GREEN << " Toate stocurile sunt optime! Niciun produs nu este sub prag.\n" << RESET;
    }
    std::cout << "-------------------------------------------------------------------\n";
}

void Depozit::recomandari() const {
    std::vector<Produs> deRecomandat;
    for (const auto& x : produse) {
        if (x.second.getCantitate() < x.second.getPrag()) {
            deRecomandat.push_back(x.second);
        }
    }

    std::sort(deRecomandat.begin(), deRecomandat.end(), [](const Produs& a, const Produs& b) {
        return a.getCantitate() < b.getCantitate();
        });

    std::cout << CYAN << BOLD << "\n=================================================================================\n";
    std::cout << "               SUGESTII REAPROVIZIONARE (SORTATE DUPA URGENTA)                   \n";
    std::cout << "=================================================================================\n" << RESET;

    std::cout << BOLD << std::left
        << std::setw(25) << "Nume Produs"
        << std::setw(12) << "Stoc Ramas"
        << std::setw(20) << "Furnizor"
        << std::setw(25) << "Email Contact" << RESET << "\n";
    std::cout << "---------------------------------------------------------------------------------\n";

    for (const auto& p : deRecomandat) {
        std::cout << std::left
            << std::setw(25) << p.getNume()
            << RED << std::setw(12) << p.getCantitate() << RESET
            << std::setw(20) << p.getFurnizor().getNume()
            << std::setw(25) << p.getFurnizor().getEmail() << "\n";
    }

    if (deRecomandat.empty()) {
        std::cout << GREEN << " Nu exista produse care necesita recomandare de comanda.\n" << RESET;
    }
    std::cout << "---------------------------------------------------------------------------------\n";
}

void Depozit::afiseazaTranzactii() const {
    std::cout << BLUE << BOLD << "\n=========================================\n";
    std::cout << "        ISTORIC TRANZACTII DEPOZIT       \n";
    std::cout << "=========================================\n" << RESET;

    if (istoricIntrari.empty() && istoricIesiri.empty()) {
        std::cout << " Nicio tranzactie efectuata in aceasta sesiune.\n";
    }

    for (const auto& t : istoricIntrari) t.afiseaza();
    for (const auto& t : istoricIesiri) t.afiseaza();

    std::cout << "=========================================\n";
}

void Depozit::incarcaCSV(std::string fisier) {
    std::ifstream f(fisier);
    if (!f.is_open()) return;
    std::string linie;
    getline(f, linie);

    while (getline(f, linie)) {
        if (linie.empty()) continue;
        std::stringstream ss(linie);
        std::string colID, nume, colCant, colPret, colPrag, fNume, fEmail;

        if (getline(ss, colID, ',') && getline(ss, nume, ',') &&
            getline(ss, colCant, ',') && getline(ss, colPret, ',') &&
            getline(ss, colPrag, ',')) {

            if (!getline(ss, fNume, ',')) fNume = "Distribuitor Central";
            if (!getline(ss, fEmail, ',')) fEmail = "comenzi@depozit.ro";

            int id = std::stoi(colID);
            produse[id] = Produs(id, nume, std::stoi(colCant), std::stod(colPret), std::stoi(colPrag), Furnizor(fNume, fEmail));
        }
    }
    f.close();
}

void Depozit::salveazaCSV(std::string fisier) {
    std::ofstream g(fisier);
    g << "ID,Nume,Cantitate,Pret,Prag,FurnizorNume,FurnizorEmail\n";
    for (const auto& x : produse) {
        g << x.second.getId() << ","
            << x.second.getNume() << ","
            << x.second.getCantitate() << ","
            << x.second.getPret() << ","
            << x.second.getPrag() << ","
            << x.second.getFurnizor().getNume() << ","
            << x.second.getFurnizor().getEmail() << "\n";
    }
    g.close();
}