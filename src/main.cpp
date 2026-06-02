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
        // Versiune curată care nu dă erori de encodare în CMD
        std::cout << "\n\033[30m\033[46m  == SISTEM GESTIUNE DEPOZIT v1.2 ==  \033[0m";
        std::cout << "  \033[90m[ Sesiune Activa ]\033[0m\n";

        std::cout << "\033[36m============================================================\033[0m\n";
        std::cout << "  \033[1mOPTIUNI DISPONIBILE:\033[0m\n";
        std::cout << "  \033[36m|\033[0m  [\033[92m1\033[0m] Raport produse sub prag\n";
        std::cout << "  \033[36m|\033[0m  [\033[92m2\033[0m] Recomandari reaprovizionare\n";
        std::cout << "  \033[36m|\033[0m  [\033[92m3\033[0m] Inregistreaza Vanzare (Iesire stoc)\n";
        std::cout << "  \033[36m|\033[0m  [\033[92m4\033[0m] Inregistreaza Restock (Intrare stoc)\n";
        std::cout << "  \033[36m|\033[0m  [\033[92m5\033[0m] Vizualizare istoric tranzactii\n";
        std::cout << "  \033[36m|\033[0m  [\033[92m6\033[0m] Cautare produs dupa nume (partial)\n";
        std::cout << "  \033[36m|\033[0m  [\033[95m7\033[0m] Vizualizare statistici financiare globale\n"; // Opțiune nouă 1
        std::cout << "  \033[36m|\033[0m  [\033[95m8\033[0m] Export raport managerial in format HTML\n"; // Opțiune nouă 2
        std::cout << "  \033[36m|----------------------------------------------------------|\033[0m\n";
        std::cout << "  \033[36m|\033[0m  [\033[91m0\033[0m] Salvare date si Iesire securizata\n";
        std::cout << "\033[36m============================================================\033[0m\n";
        std::cout << " Cautati o optiune din lista de mai sus.\n";
        std::cout << " \033[1mComanda dumneavoastra >>\033[0m ";
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
                d.aplicaLoyaltyCard(id, c);
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
            else if (opt == 6) {
                std::string text;
                std::cout << "Introduceti textul pentru cautare: ";
                std::cin.ignore(); // Curăță buffer-ul de la cin-ul anterior
                std::getline(std::cin, text); // Folosim getline pentru a citi și spații (ex: "Mouse Wireless")
                d.cautaDupaNume(text);
            }
            else if (opt == 7) {
                d.afiseazaStatistici();
            }
            else if (opt == 8) {
                std::string fisierRaport = "data/raport_stocuri.html";
                d.exportaRaportHTML(fisierRaport);
                std::cout << "\033[32m\033[1m[SUCCES] Raportul HTML a fost generat in: " << fisierRaport << "\033[0m\n";
                std::cout << "[INFO] Puteti deschide fisierul direct in Google Chrome / Microsoft Edge pentru a-l vizualiza.\n";
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