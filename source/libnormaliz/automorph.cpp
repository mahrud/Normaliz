/*
 * Normaliz
 * Copyright (C) 2007-2014  Winfried Bruns, Bogdan Ichim, Christof Soeger
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As an exception, when this program is distributed through (i) the App Store
 * by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or (iii) Google Play
 * by Google Inc., then that store may impose any digital rights management,
 * device limits and/or redistribution restrictions that are required by its
 * terms of service.
 */

//---------------------------------------------------------------------------

#include "libnormaliz/matrix.h"
#include "libnormaliz/nmz_nauty.h"

namespace libnormaliz {
using namespace std;

template<typename Integer>
vector<vector<key_t> > Automorphism_Group<Integer>::getGenPerms() const{
    return GenPerms;
}

template<typename Integer>
mpz_class Automorphism_Group<Integer>::getOrder() const{
    return order;
}

template<typename Integer>
vector<vector<key_t> > Automorphism_Group<Integer>::getLinFormPerms() const{
    return LinFormPerms;
}

template<typename Integer>
vector<vector<key_t> > Automorphism_Group<Integer>::getGenOrbits() const{
    return GenOrbits;
}

template<typename Integer>
vector<vector<key_t> > Automorphism_Group<Integer>::getLinFormOrbits() const{
    return LinFormOrbits;
}

template<typename Integer>
vector<Matrix<Integer> > Automorphism_Group<Integer>::getLinMaps() const{
    return LinMaps;
}

template<typename Integer>
bool Automorphism_Group<Integer>::isFromAmbientSpace() const{
    return from_ambient_space;
}

template<typename Integer>
bool Automorphism_Group<Integer>::isLinMapsComputed() const{
    return LinMaps_computed;
}

template<typename Integer>
bool Automorphism_Group<Integer>::isGraded() const{
    return graded;
}

template<typename Integer>
bool Automorphism_Group<Integer>::isInhomogeneous() const{
    return inhomogeneous;
}

template<typename Integer>
void Automorphism_Group<Integer>::setFromAmbeientSpace(bool on_off){
    from_ambient_space=on_off;
}

template<typename Integer>
void Automorphism_Group<Integer>::setGraded(bool on_off){
    graded=on_off;
}

template<typename Integer>
void Automorphism_Group<Integer>::setInhomogeneous(bool on_off){
    inhomogeneous=on_off;
}

template<typename Integer>
void Automorphism_Group<Integer>::reset(){
    LinMaps_computed=false;
    from_ambient_space=false;
    graded=false;
    inhomogeneous=false;
}

template<typename Integer>
Automorphism_Group<Integer>::Automorphism_Group(){
    reset();
}

template<typename Integer>
bool Automorphism_Group<Integer>::make_linear_maps_primal(){

    LinMaps.clear();
    vector<key_t> PreKey=Gens.max_rank_submatrix_lex();
    vector<key_t> ImKey(PreKey.size());
    for(size_t i=0;i<GenPerms.size();++i){
        for(size_t j=0;j<ImKey.size();++j)
            ImKey[j]=GenPerms[i][PreKey[j]];
        Matrix<Integer> Pre=Gens.submatrix(PreKey);
        Matrix<Integer> Im=Gens.submatrix(ImKey);
        Integer denom,g;
        Matrix<Integer> Map=Pre.solve(Im,denom);
        g=Map.matrix_gcd();
        if(g%denom !=0)
            return false;
        Map.scalar_division(denom);
        if(Map.vol()!=1)
            return false;
        LinMaps.push_back(Map.transpose());
        //Map.pretty_print(cout);
        // cout << "--------------------------------------" << endl;
    }
    LinMaps_computed=true;
    return true;    
}

template<typename Integer>
bool Automorphism_Group<Integer>::compute(const Matrix<Integer>& GivenGens,const Matrix<Integer>& GivenLinForms, 
                                          const size_t nr_special_linforms){
    Gens=GivenGens;
    LinForms=GivenLinForms;
    
    /* for(size_t i=0;i<Gens.nr_of_rows();++i)
        cout << v_scalar_product(Gens[i],LinForms[LinForms.nr_of_rows()-1]) << endl;*/
    vector<vector<long> > result=compute_automs(Gens,LinForms,nr_special_linforms,order);
    size_t nr_automs=(result.size()-2)/2;
    GenPerms.clear();
    LinFormPerms.clear();
    for(size_t i=0;i<nr_automs;++i){
        vector<key_t> dummy(result[0].size());
        for(size_t j=0;j<dummy.size();++j)
            dummy[j]=result[i][j];
        GenPerms.push_back(dummy);
        vector<key_t> dummy_too(result[nr_automs].size());
        for(size_t j=0;j<dummy_too.size();++j)
            dummy_too[j]=result[i+nr_automs][j];
        LinFormPerms.push_back(dummy_too);
    }
    GenOrbits=convert_to_orbits(result[result.size()-2]);
    // cout << GenOrbits;
    LinFormOrbits=convert_to_orbits(result[result.size()-1]);
    // cout << LinFormOrbits;
    return make_linear_maps_primal();
}

template<typename Integer>
bool Automorphism_Group<Integer>::compute(const Matrix<Integer>& ComputeFrom, const Matrix<Integer>& GivenGens,const Matrix<Integer>& GivenLinForms, 
             const size_t nr_special_linforms){
    
    bool success=compute(ComputeFrom,GivenLinForms,nr_special_linforms);
    if(!success)
        return false;
    Gens=GivenGens;
    gen_data_via_lin_maps();
    return true;
}

template<typename Integer>
void Automorphism_Group<Integer>::gen_data_via_lin_maps(){

    GenPerms.clear();
    map<vector<Integer>,key_t> S;
    for(key_t k=0;k<Gens.nr_of_rows();++k)
        S[Gens[k]]=k;
    for(size_t i=0; i<LinMaps.size();++i){
        vector<key_t> Perm(Gens.nr_of_rows());
        for(key_t j=0;j<Perm.size();++j){
            vector<Integer> Im=LinMaps[i].MxV(Gens[j]);
            assert(S.find(Im)!=S.end()); // for safety
            Perm[j]=S[Im];
        }
        GenPerms.push_back(Perm);            
    } 
    GenOrbits=orbits(GenPerms);
}

vector<vector<key_t> > keys(const list<boost::dynamic_bitset<> >& Partition){
    
    vector<vector<key_t> > Keys;
    auto p=Partition.begin();
    for(;p!=Partition.end();++p){
        vector<key_t> key;
        for(size_t j=0;j< p->size();++j)
            if(p->test(j))
                key.push_back(j);
        Keys.push_back(key);
    }
    return Keys;
}

list<boost::dynamic_bitset<> > partition(size_t n, const vector<vector<key_t> >& Orbits){
    
    list<boost::dynamic_bitset<> > Part;
    for(size_t i=0;i<Orbits.size();++i){
        boost::dynamic_bitset<> p(n);
        for(size_t j=0;j<Orbits[i].size();++j)
            p.set(Orbits[i][j],true);
        Part.push_back(p);
    }
    return Part;
}

list<boost::dynamic_bitset<> > join_partitions(const list<boost::dynamic_bitset<> >& P1,
                                               const list<boost::dynamic_bitset<> >& P2){
    list<boost::dynamic_bitset<> > J=P1; // work copy pf P1
    auto p2=P2.begin();
    for(;p2!=P2.end();++p2){
        auto p1=J.begin();
        for(;p1!=J.end();++p1){ // search the first member of J that intersects p1
            if((*p2).intersects(*p1))
                break;
        }
        if((*p2).is_subset_of(*p1)) // is contained in that member, nothing to do
            continue;
        // now we join the members of J that intersect p2
        assert(p1!=J.end()); // to be on the safe side
        auto p3=p1;
        p3++;
        while(p3!=J.end()){
            if((*p2).intersects(*p3)){
                *p1= *p1 | *p3; //the union
                p3=J.erase(p3);
            }else
                p3++;
        }
    }
    return J;
}


vector<vector<key_t> > orbits(const vector<vector<key_t> >& Perms){
    
    vector<vector<key_t> > Orbits;
    if(Perms.size()==0)
        return Orbits;
    Orbits=cycle_decomposition(Perms[0],true); // with fixed points!
    list<boost::dynamic_bitset<> > P1=partition(Perms[0].size(),Orbits);
    for(size_t i=1;i<Perms.size();++i){
        vector<vector<key_t> > Orbits_1=cycle_decomposition(Perms[i]);
        list<boost::dynamic_bitset<> > P2=partition(Perms[0].size(),Orbits_1);
        P1=join_partitions(P1,P2);
    }
    return keys(P1);
}

vector<vector<key_t> > convert_to_orbits(const vector<long>& raw_orbits){
    
    vector<key_t> key(raw_orbits.size());
    vector<vector<key_t> > orbits;
    for(key_t i=0;i<raw_orbits.size();++i){
        if(raw_orbits[i]==(long) i){
            orbits.push_back(vector<key_t>(1,i));
            key[i]=orbits.size()-1;
        }
        else{
            orbits[key[raw_orbits[i]]].push_back(i);
        }
    }
    return orbits;    
}
vector<vector<key_t> > cycle_decomposition(vector<key_t> perm, bool with_fixed_points){

    vector<vector<key_t> > dec;
    vector<bool> in_cycle(perm.size(),false);
    for (size_t i=0;i<perm.size();++i){
        if(in_cycle[i])
            continue;
        if(perm[i]==i){
            if(!with_fixed_points)
                continue;
            vector<key_t> cycle(1,i);
            in_cycle[i]=true;
            dec.push_back(cycle);
            continue;            
        }
        in_cycle[i]=true;
        key_t next=i;
        vector<key_t> cycle(1,i);
        while(true){
            next=perm[next];
        if(next==i)
            break;
        cycle.push_back(next);
        in_cycle[next]=true;
        }
        dec.push_back(cycle);
    }
    return dec;
}

void pretty_print_cycle_dec(const vector<vector<key_t> >& dec, ostream& out){
    
  for(size_t i=0;i<dec.size();++i){
	out << "(";
	for(size_t j=0;j<dec[i].size();++j){
	    out << dec[i][j];
	    if(j!=dec[i].size()-1)
	      out << " ";
	}
	out << ") ";
      
  }
  out << "--" << endl;
}
    
template<typename Integer>
vector<vector<long> > compute_automs(const Matrix<Integer>& Gens, const Matrix<Integer>& LinForms, const size_t nr_special_linforms, mpz_class& group_order){

    vector<vector<long> > Automs=compute_automs_by_nauty(Gens.get_elements(), LinForms.get_elements(), nr_special_linforms, group_order);
    return Automs;
}

} // namespace