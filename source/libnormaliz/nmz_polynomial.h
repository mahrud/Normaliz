/*
 * Copyright (C) 2007-2022  W. Bruns, B. Ichim, Ch. Soeger, U. v. d. Ohe
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * As an exception, when this program is distributed through (i) the App Store
 * by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or (iii) Google Play
 * by Google Inc., then that store may impose any digital rights management,
 * device limits and/or redistribution restrictions that are required by its
 * terms of service.
 */

#ifndef LIBNORMALIZ_NMZ_POLYNOMIAL_H
#define LIBNORMALIZ_NMZ_POLYNOMIAL_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <gmpxx.h>

#include "libnormaliz/dynamic_bitset.h"
#include "libnormaliz/general.h"

namespace libnormaliz {

using namespace std;

//-------------------------------------------------------------------
//       OurTerm
//-------------------------------------------------------------------

template <typename Number>
class OurPolynomial;

template<typename Number>
class OurTerm {

    template <typename>
    friend class OurPolynomial;

public:

    Number coeff;
    map<key_t, long> monomial;
    dynamic_bitset support;

    Number evaluate(const vector<Number>& argument) const;
    OurTerm();
    OurTerm(const Number& c, const map<key_t, long>& mon, const dynamic_bitset& supp);
    void shift_coordinates(const int& shift);
    void swap_coordinates(const key_t& first, const key_t& second);
    void cyclic_shift_right(const key_t& col);
};

template<typename Number>
class OurPolynomial : public std::vector<OurTerm<Number> > {

public:

    key_t highest_indet;
    dynamic_bitset support;

    Number evaluate(const vector<Number>& argument) const;
    OurPolynomial();
    OurPolynomial(const string& poly_string, const size_t dim, const bool);
    key_t get_highest_indet() const;
    void shift_coordinates(const int& shift);
    void swap_coordinates(const key_t& first, const key_t& second);
    void cyclic_shift_right(const key_t& col);
};

template<typename Number>
class OurPolynomialSystem : public std::vector<OurPolynomial<Number> > {

public:

    OurPolynomialSystem(const vector<string>& poly_strings, const size_t dim, const bool verb);
    OurPolynomialSystem();
    void shift_coordinates(const int& shift);
    void swap_coordinates(const key_t& first, const key_t& second);
    void cyclic_shift_right(const key_t& col);

    bool check(const vector<Number>& argument, const bool is_quations, const bool exact_length) const;

    bool verbose;

};

template <typename To, typename From>
void convert(OurPolynomial<To>& ret, const OurPolynomial<From>& arg){
    for(auto& T: arg){
        To c = convertTo<To>(T.coeff);
        ret.push_back(OurTerm<To>(c, T.monomial, T.support));
    }
    ret.highest_indet = arg.highest_indet;
    ret.support = arg.support;
}

template <typename To, typename From>
void convert(OurPolynomialSystem<To>& ret, const OurPolynomialSystem<From>& arg){
    for(auto& P: arg){;
        OurPolynomial<To> P_ret;
        convert(P_ret, P);
        ret.push_back(P_ret);
    }
    ret.verbose = arg.verbose;
}

} // name space

#endif
