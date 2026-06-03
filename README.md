# Sistem Integrat de Gestiune ERP și POS — Nexus IT Hub

Aplicație C++17 orientată pe obiecte destinată managementului complet al stocurilor, fluxurilor logistice și vânzărilor dintr-un showroom modern de componente hardware și echipamente IT. Proiectul include o interfață interactivă în consolă securizată prin coduri ANSI (paletă cromatică albastru-alb) și suport pentru generarea rapoartelor de audit.

**Autor:** Cirdeiu Loredana, grupa 3122B

---

## Funcționalități principale

### Domeniu & model
* **Entități core:** `Produs` (Componente hardware), `Tranzactie<T>` (Șablon pentru intrări/ieșiri), `Depozit` (Manager orchestrator)
* **Design patterns & Concepte POO:** Încapsulare strictă (atribute private, acces controlat prin Getters/Setters), Programare Generică / Șabloane (`Tranzactie<T>` bazată pe structuri de diferențiere logică `Intrare` și `Iesire`)
* **Eficiență containere (STL):** Utilizarea `std::unordered_map` pentru maparea produselor după codul de bare (ID), asigurând o viteză optimă de căutare în timp constant $O(1)$. Istoricul este salvat în containere de tip `std::vector`.
* **Gestiune erori:** Tratare robustă a fluxurilor invalide prin blocuri `try-catch` (ex: stoc insuficient la vânzare, ID inexistent).

### Panou Operator / Manager Showroom (Meniu 1-8)

#### 1. Verificare stoc critic la raft
Identifică și afișează în timp real produsele hardware ale căror unități fizice au scăzut sub pragul minim de siguranță setat, prevenind golirea rafturilor din showroom.

#### 2. Recomandări aprovizionare furnizor
Algoritm automat care analizează deficitele din baza de date și generează o listă centralizată de comenzi către depozitul central sau furnizori pentru refacerea stocurilor.

#### 3. Înregistrare vânzare POS & Nexus Loyalty Card
Permite vânzarea rapidă prin introducerea codului de bare (ID) și a cantității dorite.
* **Sistem de loialitate integrat:** Modulul simulează scanarea unui *Nexus Card* sau aplicarea unui cod promoțional (ex: `TECH10`, `STUDENT20`, `NEXUS30`), scade stocul automat în background și generează instant un **Bon Fiscal complet** cu preț standard, discount aplicat, economie realizată și total de plată.

#### 4. Recepție loturi noi (Restock)
Înregistrarea intrărilor de marfă (camioane furnizori) în baza de date, crescând stocurile pentru reperele hardware specificate.

#### 5. Jurnal audit și istoric
Afișează toate operațiunile financiare și logistice efectuate în cadrul sesiunii curente, stocate cronologic (vânzări și aprovizionări).

#### 6. Căutare rapidă Info-Point
Permite clienților sau operatorilor căutarea unui produs după denumire. Căutarea este **case-insensitive** (folosește `std::transform`), permițând găsirea componentelor chiar dacă utilizatorul introduce doar un text parțial (ex: „tast” pentru „Tastatură Mecanică”).

#### 7. Indicatori financiari (Patrimoniu)
Calculează automat activele totale ale magazinului (valoarea cumulată a stocurilor în RON), numărul de articole active și reperele statistice cheie.

#### 8. Export Registru Stocuri (HTML)
Generează automat un raport managerial complet stilizat în format web (`raport_inventar_NexusIT.html`). Raportul conține tabele moderne și insigne colorate (badges) pentru o vizualizare facilă direct în browser.

---

## Tehnologii

| Componentă | Tehnologie | Descriere |
| :--- | :--- | :--- |
| **Limbaj** | C++17 | Standard modern de programare structurală |
| **Build System** | Make / CMake | Automatizarea procesului de compilare |
| **Compilator** | g++ (GCC) | Compilator nativ folosit pentru medii Linux/Ubuntu |
| **Interfață** | ANSI CLI | Consolă interactivă colorată în paleta albastru-alb |
| **Persistență** | Fișiere CSV / TXT | Salvare incrementală și securizată la închiderea sesiunii |

---

## Structura proiectului

```text
├── include/                 # Headere (interfețele claselor)
│   ├── Produs.h             # Definirea entității hardware
│   ├── Tranzactie.h         # Clasă șablon pentru jurnalizare fluxuri
│   ├── Depozit.h            # Centralizator / Manager al magazinului
│   └── Exceptii.h           # Tratarea excepțiilor custom
├── src/                     # Implementări sursă (.cpp)
│   ├── main.cpp             # Interfața cu meniul POS și opțiunile utilizatorului
│   ├── Produs.cpp
│   └── Depozit.cpp          # Logica de business (salvare, calcul discount, HTML)
├── data/                    # Baze de date (date persistate)
│   ├── produse.csv          # Catalogul de produse hardware (stoc, preț, prag)
│   ├── istoric_tranzactii.txt # Istoricul text al operațiunilor de audit
│   └── raport_inventar_NexusIT.html # Raportul managerial generat automat
├── Makefile                 # Fișierul de configurare pentru compilare rapidă în Linux
└── README.md