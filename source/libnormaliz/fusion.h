/*
 * Normaliz
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

#ifndef LIBNORMALIZ_FUSION_H_
#define LIBNORMALIZ_FUSION_H_

#include <vector>
#include <list>
#include <map>
#include <set>
#include <fstream>

#include "libnormaliz/general.h"
#include "libnormaliz/matrix.h"

namespace libnormaliz {
using std::vector;
using std::map;
using std::list;
using std::set;
using std::ifstream;

template <typename Integer>
class FusionData    {
    template <typename, typename>
    friend class ProjectAndLift;

public:

    bool activated;
    bool type_and_duality_set;

    bool verbose;

    bool commutative;

    bool check_simplicity;
    bool select_simple;
    bool candidate_given;

    bool use_automorphisms;
    bool select_iso_classes;

    size_t nr_coordinates;

    vector<key_t> duality;
    size_t fusion_rank;
    vector<key_t> fusion_type;

    vector<key_t> subring_base_key;
    vector<vector<key_t> > all_base_keys;
    vector<vector<vector<key_t> > > all_critical_coords_keys;
    vector<vector<key_t> > coords_to_check_key;
    vector<dynamic_bitset> coords_to_check_ind;
    vector<vector<key_t> > all_ind_tuples;
    vector<vector<key_t> > selected_ind_tuples; // the lex smallest in each FrobRec set
    map<set<vector<key_t> >, key_t> CoordMap;

    vector<vector<key_t> > Automorphisms;
    vector<dynamic_bitset> Orbits;

    FusionData();
    void set_options(const ConeProperties& ToCompute, const bool verb);
    void read_data(const bool a_priori);
    void read_data_from_file();
    void data_from_roject();

    // coordinates
    void make_CoordMap();
    key_t dual(const key_t i) const;
    set<vector<key_t> > FrobRec(const vector<key_t>& ind_tuple);
    set<vector<key_t> > FrobRec_6(const vector<key_t>& ind_tuple);
    set<vector<key_t> > FrobRec_12(const vector<key_t>& ind_tuple);
    key_t coord(set<vector<key_t> >& FR);
    key_t coord(vector<key_t>& ind_tuple);
    void make_all_ind_tuples();

    // for simplicity check
    dynamic_bitset critical_coords(const vector<key_t>& base_key);
    void make_all_base_keys();
    void prepare_simplicity_check();
    Matrix<Integer> do_select_simple_inner(const Matrix<Integer>& LattPoints);
    Matrix<Integer> do_iso_classes_inner(const Matrix<Integer>& LattPoints);
    Matrix<Integer> do_select_simple(const Matrix<Integer>& LattPoints) const;
    Matrix<Integer> do_iso_classes(const Matrix<Integer>& LattPoints) const;
    bool simplicity_check(const vector<key_t>& subring, const vector<Integer>& sol);
    bool simplicity_check(const vector<vector<key_t> >& subrings, const vector<Integer>& sol);
    bool automs_compatible(const vector<key_t>& cand) const;

    // for automosphisms
    void make_automorphisms();
    vector<Integer> norrmal_form(const vector<Integer> lattice_point);

};

// helpers
Matrix<long long> extract_latt_points_from_out(ifstream& in_out);
template <typename Integer>
Matrix<Integer> select_simple(const Matrix<Integer>& LattPoints, const ConeProperties& ToCompute, const bool verb);
template <typename Integer>
Matrix<Integer> fusion_iso_classes(const Matrix<Integer>& LattPoints, const ConeProperties& ToCompute, const bool verb);
//void select_simple_fusion_rings();
template <typename Integer>
void split_into_simple_and_nonsimple(Matrix<Integer>& SimpleFusionRings, Matrix<Integer>& NonsimpleFusionRings, const Matrix<Integer>& FusionRings, bool verb);

vector<dynamic_bitset> make_all_subsets(const size_t card);
vector<vector<key_t> > make_all_permutations(size_t n);
vector<vector<key_t> > collect_coincidence_subset_keys(const vector<key_t>& type);
vector<vector<key_t> > make_all_permutations(const vector<key_t>& v);
vector<vector<key_t> > make_all_permutations(const vector<key_t>& type, const vector<key_t>& duality);

void post_process_fusion(const vector<string>& command_line_items);

}  // end namespace libnormaliz

#endif /* FUSION_H_ */