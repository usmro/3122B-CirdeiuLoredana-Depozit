#include <iostream>
#include <cassert>
#include <stdexcept>
#include "Depozit.h"

// Coduri ANSI pentru culori în consolă
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

void rulateTesteUnitare() {
    std::cout << CYAN << "[TEST] Se ruleaza testele unitare pentru logica..." << RESET << "\n";

    Produs p(999, "Test", 10, 2.5, 5);
    p += 5;
    assert(p.getCantitate() == 15);

    bool exceptieStocPrinsa = false;
    try {
        p -= 20;
    }
    catch (const std::runtime_error&) {
        exceptieStocPrinsa = true;
    }
    assert(exceptieStocPrinsa == true);

    Depozit d;
    d.adauga(p);
    bool LocOcupatPrins = false;
    try {
        d.adauga(p);
    }
    catch (const std::runtime_error&) {
        LocOcupatPrins = true;
    }
    assert(LocOcupatPrins == true);

    std::cout << GREEN << BOLD << "[SUCCES] Toate testele unitare au trecut!" << RESET << "\n\n";
}

int main() {
    rulateTesteUnitare();

    Depozit d;
    d.incarcaCSV("data/produse.csv");

    int opt;
    do {
        std::cout << YELLOW << "==================================================\n" << RESET;
        std::cout << BLUE << BOLD << "        SISTEM MONITORIZARE STOCURI DEPOZIT       \n" << RESET;
        std::cout << YELLOW << "==================================================\n" << RESET;
        std::cout << "  [" << CYAN << "1" << RESET << "] Raport produse sub pragul de alerta\n";
        std::cout << "  [" << CYAN << "2" << RESET << "] Sugestii de reaprovizionare (Sortate)\n";
        std::cout << "  [" << CYAN << "3" << RESET << "] Inregistreaza Vanzare (Iesire stoc)\n";
        std::cout << "  [" << CYAN << "4" << RESET << "] Inregistreaza Restock (Intrare stoc)\n";
        std::cout << "  [" << CYAN << "5" << RESET << "] Vizualizare istoric complet tranzactii\n";
        std::cout << "  [" << RED << "0" << RESET << "] Salvare date si Inchidere aplicatie\n";
        std::cout << YELLOW << "--------------------------------------------------\n" << RESET;
        std::cout << BOLD << " Selectati o optiune: " << RESET;
        std::cin >> opt;

        // Validare input numeric greșit
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(32767, '\n');
            std::cout << RED << "\n[EROARE] Introducere invalida! Introduceti o cifra.\n" << RESET;
            continue;
        }

        try {
            if (opt == 1) {
                d.raportSubPrag();
            }
            else if (opt == 2) {
                d.recomandari();
            }
            else if (opt == 3) {
                int id, c;
                std::cout << "\n" << BOLD << "--- OPERATIUNE VANZARE ---" << RESET << "\n";
                std::cout << "ID Produs: "; std::cin >> id;
                std::cout << "Cantitate vanduta: "; std::cin >> c;
                d.actualizeaza(id, -c);
                std::cout << GREEN << "\n[OK] Vanzare salvata cu succes!" << RESET << "\n";
            }
            else if (opt == 4) {
                int id, c;
                std::cout << "\n" << BOLD << "--- OPERATIUNE RESTOCK ---" << RESET << "\n";
                std::cout << "ID Produs: "; std::cin >> id;
                std::cout << "Cantitate primita: "; std::cin >> c;
                d.actualizeaza(id, c);
                std::cout << GREEN << "\n[OK] Restock adaugat cu succes!" << RESET << "\n";
            }
            else if (opt == 5) {
                d.afiseazaTranzactii();
            }
            else if (opt != 0) {
                std::cout << RED << "\n[ATENTIE] Opțiune inexistenta in meniu!\n" << RESET;
            }
        }
        catch (const std::exception& e) {
            std::cout << RED << BOLD << "\n[EROARE] " << RESET << RED << e.what() << RESET << "\n";
        }

        std::cout << "\n"; // Linie goala pentru aerisire vizuala

    } while (opt != 0);

    d.salveazaCSV("data/produse.csv");
    std::cout << GREEN << BOLD << "Datele au fost salvate in 'data/produse.csv'. La revedere!" << RESET << "\n";
    return 0;
}