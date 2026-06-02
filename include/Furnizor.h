#pragma once
#ifndef FURNIZOR_H
#define FURNIZOR_H

#include <string>

class Furnizor {
    std::string nume;
    std::string email;
public:
    Furnizor();
    Furnizor(std::string n, std::string e);
    std::string getNume() const;
    std::string getEmail() const;
};

#endif