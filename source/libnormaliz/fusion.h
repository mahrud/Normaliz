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
#include "libnormaliz/input.h"

namespace libnormaliz {
using std::vector;
using std::map;
using std::list;
using std::set;
using std::ifstream;

template <typename Integer>
class FusionComp    {
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
    bool write_mult_tables;

    size_t nr_coordinates;

    size_t fusion_rank;
    vector<key_t> fusion_type; // only coincidence pattern
    string fusion_type_string;
    vector<key_t> duality;

    void initialize();
    void import_global_data();
    vector<vector<vector<key_t> > > all_critical_coords_keys;
    vector<vector<key_t> > coords_to_check_key;
    vector<dynamic_bitset> coords_to_check_ind;
    vector<vector<key_t> > all_ind_tuples;
    vector<vector<key_t> > selected_ind_tuples; // the lex smallest in each FrobRec set
    map<set<vector<key_t> >, key_t> CoordMap;

    vector<vector<key_t> > Automorphisms;
    vector<dynamic_bitset> Orbits;

    vector<vector<Matrix<Integer> > > AllTables;

    FusionComp();
    void set_options(const ConeProperties& ToCompute, const bool verb);
    pair<bool, bool> read_data(const bool a_priori, const bool only_test = false);
    //void read_data_from_file();
    pair<bool, bool>  data_from_string(const string& our_fusion, const bool onyl_test);

    // coordinates
    void make_CoordMap();
    set<vector<key_t> > FrobRec(const vector<key_t>& ind_tuple);
    set<vector<key_t> > FrobRec_6(const vector<key_t>& ind_tuple);
    set<vector<key_t> > FrobRec_12(const vector<key_t>& ind_tuple);
    key_t coord(set<vector<key_t> >& FR);
    key_t coord(vector<key_t>& ind_tuple);
    key_t coord_cone(vector<key_t>& ind_tuple);
    void make_all_ind_tuples();
    Integer value(const vector<Integer>& ring, vector<key_t>& ind_tuple);

    // for simplicity check
    vector<key_t> subring_base_key;
    dynamic_bitset critical_coords(const vector<key_t>& base_key);
    vector<vector<key_t> > all_base_keys;
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

    Matrix<Integer> make_linear_constraints(const vector<Integer>& d);
    Matrix<Integer> make_linear_constraints_partition(const vector<Integer>& d,
                                            const vector<long>& card);
    pair<Integer, vector<key_t> >  term(const key_t& i, const key_t& j, const key_t& k);
    set<map<vector<key_t>, Integer> > make_associativity_constraints();
    void make_input_from_fusion_data(InputMap<mpq_class>&  input, const bool write_input_file);
    void make_partition_input_from_fusion_data(InputMap<mpq_class>&  input, const bool write_input_file);
    void do_werite_input_file(InputMap<mpq_class>&  input);
    // void set_global_fusion_data();

    void write_all_data_tables(const Matrix<Integer>& rings, ostream& table_out);
    void tables_for_all_rings(const Matrix<Integer>& rings);
    vector<Matrix<Integer> > make_all_data_tables(const vector<Integer>& ring);
    Matrix<Integer> data_table(const vector<Integer>& ring, const size_t i);
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

void remove_global_fusion_data();

void post_process_fusion(const vector<string>& command_line_items);

template <typename Integer>
void make_full_input(InputMap<Integer>& input_data, set<map<vector<key_t>, Integer> >& Polys);

template <typename Integer>
void make_full_input_partition(InputMap<Integer>& input_data);

template <typename Integer>
vector<key_t> fusion_coincidence_pattern(const vector<Integer>& v);

}  // end namespace libnormaliz

#endif /* FUSION_H_ */
