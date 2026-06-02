#include "Furnizor.h"

Furnizor::Furnizor() : nume("Necunoscut"), email("-") {}
Furnizor::Furnizor(std::string n, std::string e) : nume(n), email(e) {}

std::string Furnizor::getNume() const { return nume; }
std::string Furnizor::getEmail() const { return email; }