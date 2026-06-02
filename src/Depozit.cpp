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

// Adaugă aceste două funcții la finalul fișierului src/Depozit.cpp

void Depozit::afiseazaStatistici() const {
    double valoareTotalaStoc = 0;
    int totalUnitatiFizice = 0;
    int produseSubPrag = 0;
    const Produs* celMaiScump = nullptr;
    const Produs* celMaiIeftin = nullptr;

    for (const auto& x : produse) {
        const Produs& p = x.second;
        valoareTotalaStoc += p.getCantitate() * p.getPret();
        totalUnitatiFizice += p.getCantitate();

        if (p.getCantitate() < p.getPrag()) {
            produseSubPrag++;
        }

        if (!celMaiScump || p.getPret() > celMaiScump->getPret()) {
            celMaiScump = &p;
        }
        if (!celMaiIeftin || p.getPret() < celMaiIeftin->getPret()) {
            celMaiIeftin = &p;
        }
    }

    std::cout << "\n\033[32m\033[1m==================================================\n";
    std::cout << "          INDICATORI FINANCIARI SI STATISTICI     \n";
    std::cout << "==================================================\033[0m\n";
    std::cout << "  Valoarea totala a marfii:  \033[32m" << valoareTotalaStoc << " RON\033[0m\n";
    std::cout << "  Numar total de produse distincte: " << produse.size() << "\n";
    std::cout << "  Numar total de unitati fizice (stoc): " << totalUnitatiFizice << "\n";
    std::cout << "  Produse critically low (sub prag): \033[31m" << produseSubPrag << "\033[0m\n";
    if (celMaiScump) {
        std::cout << "  Cel mai valoros articol:   " << celMaiScump->getNume() << " (" << celMaiScump->getPret() << " RON)\n";
    }
    if (celMaiIeftin) {
        std::cout << "  Cel mai ieftin articol:    " << celMaiIeftin->getNume() << " (" << celMaiIeftin->getPret() << " RON)\n";
    }
    std::cout << "\033[32m--------------------------------------------------\033[0m\n";
}

void Depozit::exportaRaportHTML(std::string fisier) const {
    std::ofstream h(fisier);
    if (!h.is_open()) {
        throw std::runtime_error("Nu s-a putut crea fisierul HTML raport.");
    }

    // Scriem scheletul HTML cu CSS încorporat (clean look, culori pastelate corporative)
    h << "<!DOCTYPE html>\n<html>\n<head>\n<meta charset='UTF-8'>\n"
        << "<title>Raport Managerial Stocuri</title>\n"
        << "<style>\n"
        << "  body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin: 30px; background-color: #f8f9fa; color: #333; }\n"
        << "  h1 { color: #2c3e50; border-bottom: 2px solid #34495e; padding-bottom: 10px; }\n"
        << "  .meta { font-size: 14px; color: #7f8c8d; margin-bottom: 20px; }\n"
        << "  table { border-collapse: collapse; width: 100%; background: #fff; box-shadow: 0 2px 5px rgba(0,0,0,0.1); border-radius: 5px; overflow: hidden; }\n"
        << "  th, td { padding: 12px 15px; text-align: left; border-bottom: 1px solid #edf2f7; }\n"
        << "  th { background-color: #34495e; color: white; font-weight: 600; }\n"
        << "  tr:nth-child(even) { background-color: #f8f9fa; }\n"
        << "  tr:hover { background-color: #f1f3f5; }\n"
        << "  .status-ok { color: #27ae60; font-weight: bold; }\n"
        << "  .status-alert { background-color: #fee2e2 !important; color: #dc2626; font-weight: bold; }\n"
        << "  .badge { padding: 4px 8px; border-radius: 4px; font-size: 12px; }\n"
        << "  .badge-red { background: #fecaca; color: #991b1b; }\n"
        << "  .badge-green { background: #d1fae5; color: #065f46; }\n"
        << "</style>\n</head>\n<body>\n";

    h << "<h1>Raport de Audit Imobiliar & Stocuri Depozit</h1>\n";
    h << "<div class='meta'>Generat automat de SistemDepozit v1.2 | Sesiune securizata</div>\n";

    h << "<table>\n<thead>\n<tr>\n"
        << "  <th>ID</th><th>Denumire Produs</th><th>Stoc Curent</th><th>Pret Unitar</th><th>Prag Minim</th><th>Status Stoc</th>\n"
        << "</tr>\n</thead>\n<tbody>\n";

    for (const auto& x : produse) {
        const Produs& p = x.second;
        bool subPrag = p.getCantitate() < p.getPrag();

        h << "  <tr" << (subPrag ? " class='status-alert'" : "") << ">\n"
            << "    <td>" << p.getId() << "</td>\n"
            << "    <td>" << p.getNume() << "</td>\n"
            << "    <td>" << p.getCantitate() << " unitati</td>\n"
            << "    <td>" << p.getPret() << " RON</td>\n"
            << "    <td>" << p.getPrag() << "</td>\n";

        if (subPrag) {
            h << "    <td><span class='badge badge-red'>CRITICAL / REAPROVIZIONARE</span></td>\n";
        }
        else {
            h << "    <td><span class='badge badge-green'>OPTIM</span></td>\n";
        }
        h << "  </tr>\n";
    }

    h << "</tbody>\n</table>\n</body>\n</html>\n";
    h.close();
}