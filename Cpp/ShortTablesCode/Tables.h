//
// Created by EliaHarel on 20/10/2020.
//

#ifndef TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_TABLES_H
#define TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_TABLES_H

#define ROUNDS_OF_SHORTEN_TABLE 4

#include <vector>
namespace types {
    const int matrix_size = 256;
    typedef std::vector<double> vd;
    typedef std::vector<vd> vvd;
    typedef std::vector<vvd> vvvd;
    typedef std::vector<vvvd> vvvvd;

    typedef std::vector<int> vi;
    typedef std::vector<vi> vvi;
    typedef std::vector<vvi> vvvi;
}
using namespace types;

void Calc16Tables(int rounds, vvvvd& pre_calculated_mat);

#endif //TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_TABLES_H
