/*
 * Normaliz 2.7
 * Copyright (C) 2007-2011  Winfried Bruns, Bogdan Ichim, Christof Soeger
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
 */

//---------------------------------------------------------------------------

#include <stdlib.h>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

#include "output.h"
#include "libnormaliz/vector_operations.h"

//---------------------------------------------------------------------------

template<typename Integer>
Output<Integer>::Output(){
	type = OT_CONE;
	out=true;
	inv=false;
	ext=false;
	esp=false;
	typ=false;
	egn=false;
	gen=false;
	sup=false;
	tri=false;
	ht1=false;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::read() const{
	cout<<"\nname="<<name<<"\n";
	cout<<"\nout="<<out<<"\n";
	cout<<"\ninv="<<inv<<"\n";
	cout<<"\next="<<ext<<"\n";
	cout<<"\nesp="<<esp<<"\n";
	cout<<"\ntyp="<<typ<<"\n";
	cout<<"\negn="<<egn<<"\n";
	cout<<"\ngen="<<gen<<"\n";
	cout<<"\nsup="<<sup<<"\n";
	cout<<"\ntri="<<tri<<"\n";
	cout<<"\nht1="<<ht1<<"\n";
	cout<<"\nResult is:\n";
	Result->print();
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_name(const string& n){
	name=n;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_type(OutputType t){
	type=t;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::setCone(Cone<Integer> & C) {
	this->Result = &C;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_out(const bool& flag){
	out=flag;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_inv(const bool& flag){
	inv=flag;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_ext(const bool& flag){
	ext=flag;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_esp(const bool& flag){
	esp=flag;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_typ(const bool& flag){
	typ=flag;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_egn(const bool& flag){
	egn=flag;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_gen(const bool& flag){
	gen=flag;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_sup(const bool& flag){
	sup=flag;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_tri(const bool& flag) {
	tri=flag;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_ht1(const bool& flag) {
	ht1=flag;
}


//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_extra_files(){
	out=true;
	inv=true;
	ext=false;
	esp=false;
	typ=true;
	egn=false;
	gen=true;
	sup=true;
	tri=false;
	ht1=false;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::set_write_all_files(){
	out=true;
	inv=true;
	ext=true;
	esp=true;
	typ=true;
	egn=true;
	gen=true;
	sup=true;
	tri=true;
	ht1=true;
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::write_matrix_ext(const Matrix<Integer>& M) const{
	if (ext==true) {
		M.print(name,"ext");
	}
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::write_matrix_esp(const Matrix<Integer>& M) const{
	if (esp==true) {
		M.print(name,"esp");
	}
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::write_matrix_typ(const Matrix<Integer>& M) const{
	if (typ==true) {
		M.print(name,"typ");
	}
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::write_matrix_egn(const Matrix<Integer>& M) const {
	if (egn==true) {
		M.print(name,"egn");
	}
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::write_matrix_gen(const Matrix<Integer>& M) const {
	if (gen==true) {
		M.print(name,"gen");
	}
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::write_matrix_sup(const Matrix<Integer>& M) const{
	if (sup==true) {
		M.print(name,"sup");
	}
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::write_tri() const{
	if (tri==true) {
		string file_name = name+".tri";
		ofstream out(file_name.c_str());

		vector< pair<vector<size_t>,Integer> > Tri = Result->getTriangulation();
		typename vector< pair<vector<size_t>,Integer> >::const_iterator tit = Tri.begin();

		out << Tri.size() << endl;
		out << Result->getBasisChange().get_rank()+1 << endl; //works also for empty list

		for(; tit != Tri.end(); ++tit) {
			for (size_t i=0; i<tit->first.size(); i++) {
				out << tit->first[i] << " ";
			}
			out << tit->second << endl;
		}
		out.close();
	}
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::write_matrix_ht1(const Matrix<Integer>& M) const{
	if (ht1==true) {
		M.print(name,"ht1");
	}
}

//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::write_inv_file() const{
	if (inv==true) {//printing .inv file
		size_t i;
		size_t rank=Result->getBasisChange().get_rank();
		string name_open=name+".inv"; 							 //preparing output files
		const char* file=name_open.c_str();
		ofstream inv(file);

		if (Result->isComputed(ConeProperty::HilbertBasis)) {
			inv<<"integer hilbert_basis_elements = "<<Result->getHilbertBasis().size()<<endl;
		}

		size_t nr_ex_rays = Result->getExtremeRays().size();

		inv<<"integer number_extreme_rays = "<<nr_ex_rays<<endl;
		inv<<"integer rank = "<<rank<<endl;
		inv<<"integer index = "<< Result->getBasisChange().get_index() <<endl;
		inv<<"integer number_support_hyperplanes = "<<Result->getSupportHyperplanes().size()<<endl;

		if (Result->isHt1ExtremeRays()==false) {
			inv<<"boolean homogeneous = "<<"false"<<endl;
		}
		else {
			inv<<"boolean homogeneous = "<<"true"<<endl;
			if (Result->isComputed(ConeProperty::Ht1Elements)) {
				inv<<"integer height_1_elements = "<<Result->getHt1Elements().size()<<endl;
			}
			vector<Integer> Linear_Form = Result->getLinearForm();
			inv<<"vector "<<Linear_Form.size()<<" homogeneous_weights = ";
			for (i = 0; i < Linear_Form.size(); i++) {
				inv<<Linear_Form[i]<<" ";
			}
			inv<<endl;
			if (Result->isComputed(ConeProperty::Multiplicity)){
				inv<<"integer multiplicity = "<<Result->getMultiplicity()<<endl;
			}
			if (Result->isComputed(ConeProperty::HVector)) {
				vector<long64> h_vector=Result->getHVector64();
				inv<<"vector "<<h_vector.size()<<" h-vector = ";
				for (i = 0; i < h_vector.size(); i++) {
					inv<<h_vector[i]<<" ";
				}
				inv<<endl;
			}
			if (Result->isComputed(ConeProperty::HilbertPolynomial)) {
				vector<mpz_class> hilbert_polynomial=Result->getHilbertPolynomial();
				inv<<"vector "<<hilbert_polynomial.size()<<" hilbert_polynomial = ";
				inv<<hilbert_polynomial;
			}
		}
		if (type == OT_REES) {
			if (Result->isReesPrimary()) {
	            inv<<"boolean primary = true"<<endl;
	            inv<<"integer ideal_multiplicity = "<<Result->getReesPrimaryMultiplicity()<<endl;
			} else {
				inv<<"boolean primary = false"<<endl;
			}
		}
		inv.close();
	}
}


//---------------------------------------------------------------------------

template<typename Integer>
void Output<Integer>::write_files() const {
	const Sublattice_Representation<Integer>& BasisChange = Result->getBasisChange();
	size_t dim  = BasisChange.get_dim();
	size_t rank = BasisChange.get_rank();
	size_t i, nr;
	Matrix<Integer> Generators = Result->getGenerators();
	Matrix<Integer> Support_Hyperplanes(Result->getSupportHyperplanes());
	vector<size_t> rees_ideal_key;

	if (esp && Result->isComputed(ConeProperty::SupportHyperplanes)) {
		//write the suport hyperplanes of the full dimensional cone
		Matrix<Integer> Support_Hyperplanes_Full_Cone = BasisChange.to_sublattice_dual(Support_Hyperplanes);
		Support_Hyperplanes_Full_Cone.print(name,"esp");
	}
	if (tri && Result->isComputed(ConeProperty::Triangulation)) {     //write triangulation
		write_tri();
		Generators.print(name,"tgn");
	}

	if (out==true) {  //printing .out file
		string name_open=name+".out"; 							 //preparing output files
		const char* file=name_open.c_str();
		ofstream out(file);

		// write "header" of the .out file
		size_t nr_orig_gens = Result->getGeneratorsOfToricRing().size();
		if (nr_orig_gens > 0) {
			out << nr_orig_gens <<" original generators of the toric ring"<<endl;
		}
		if (Result->isComputed(ConeProperty::HilbertBasis)) {
			out << Result->getHilbertBasis().size() <<" Hilbert basis elements"<<endl;
		}
		if (Result->isComputed(ConeProperty::Ht1Elements)) {
			out << Result->getHt1Elements().size() <<" Hilbert basis elements of height 1"<<endl;
		}
		if (type == OT_REES && Result->isComputed(ConeProperty::HilbertBasis)) {
			Matrix<Integer> Hilbert_Basis = Result->getHilbertBasis();
			nr = Hilbert_Basis.nr_of_rows();
			for (i = 1; i <= nr; i++) {
				if (Hilbert_Basis.read(i,dim)==1) {
					rees_ideal_key.push_back(i);
		         }
			}
			out << rees_ideal_key.size() <<" generators of integral closure of the ideal"<<endl;
		}
		if (Result->isComputed(ConeProperty::ExtremeRays)) {
			size_t nr_ex_rays = Result->getExtremeRays().size();
			out << nr_ex_rays <<" extreme rays"<<endl;
		}
		if (Result->isComputed(ConeProperty::SupportHyperplanes)) {
			out << Result->getSupportHyperplanes().size() <<" support hyperplanes"<<endl;
		}
		out<<endl;
		if (rank == dim){                   //write rank and index
			out<<"rank = "<<rank<<" (maximal)"<<endl;
		}
		else {
			out<<"rank = "<<rank<<endl;
		}
		out<<"index = "<< BasisChange.get_index() <<endl;

		if (type == OT_POLYTOP) {
			out<<"dimension of the polytope = "<<rank-1<<endl;
		}
		
		if (Result->isComputed(ConeProperty::IsIntegrallyClosed)) {
			if (Result->isIntegrallyClosed()) {
				out << "original monoid is integrally closed"<<endl;
			} else {
				out << "original monoid is not integrally closed"<<endl;
			}
		}
		out << endl;
		
		if (Result->isComputed(ConeProperty::IsHt1ExtremeRays)) {
			if ( Result->isHt1ExtremeRays() ) {
				out<<"extreme rays are of height 1";
			} else {
				out<<"extreme rays are not of height 1";
			}
			if ( Result->isComputed(ConeProperty::LinearForm) ) {
				out<<" via the linear form:"<<endl;
				vector<Integer> Linear_Form = Result->getLinearForm();
				for (i = 0; i < Linear_Form.size(); i++) {
					out<<Linear_Form[i]<<" ";
				}
			}
			out<<endl<<endl;
			if ( Result->isComputed(ConeProperty::IsHt1HilbertBasis)
			  && Result->isHt1ExtremeRays() ) {
				if (Result->isHt1HilbertBasis()) {
					out << "Hilbert basis elements are of height 1";
				} else {
					out << "Hilbert basis elements are not of height 1";
				}
				out<<endl<<endl;
			}
			if ( Result->isComputed(ConeProperty::Multiplicity) ) {
				out<<"multiplicity = "<<Result->getMultiplicity()<<endl<<endl;
			}
		}
		if ( Result->isComputed(ConeProperty::HVector) ) {
			vector<long64> h_vector=Result->getHVector64();
			out<<"h-vector:"<<endl;
			for (i = 0; i < h_vector.size(); i++) {
				out<<h_vector[i]<<" ";
			}
			out<<endl<<"denominator:"<<endl<<Result->getHilbertSeries().getDenominator();
			out<<endl<<endl;
		}
		if (Result->isComputed(ConeProperty::HilbertPolynomial)) {
			vector<mpz_class> hilbert_polynomial = Result->getHilbertPolynomial();
			if (hilbert_polynomial.size() > 0) {
				out<<"Hilbert polynomial:"<<endl;
				mpz_class common_denom = permutations<mpz_class>(1,rank);
				mpz_class g;
				for (i = 0; i < hilbert_polynomial.size(); i++) {
					g = gcd<mpz_class>(common_denom,hilbert_polynomial[i]);
					out<<hilbert_polynomial[i]/g<<"/"<<common_denom/g<<" ";
				}
				out << endl<< endl;
			} else {
				vector< vector<mpz_class> > hilbert_quasi_poly = Result->getHilbertQuasiPolynomial();
				long p = hilbert_quasi_poly.size(); // periode
				if (p > 0) { // == 0 means not computed
					mpz_class common_denom = 1;
					for (long i=0; i<(long)rank-1; ++i) common_denom *= p; //p^(dim-1)
					common_denom *= permutations<mpz_class>(1,rank); // * dim!
					long size;
					mpz_class g;
					out<<"Hilbert quasi-polynomial:"<<endl;
					for (long j = 0; j< p; ++j) {
						size =  hilbert_quasi_poly[j].size();
						out << j <<": ";
						for (long i = 0; i < size; ++i) {
							g = gcd<mpz_class>(common_denom,hilbert_quasi_poly[j][i]);
							out<<hilbert_quasi_poly[j][i]/g<<"/"<<common_denom/g<<" ";
						}
						out << endl;
					}
					out << endl<< endl;
				}
			}

		}

		if (type == OT_REES) {
			if (!Result->isComputed(ConeProperty::ReesPrimary)) {
				libnormaliz::errorOutput()<<"error in Output.rees(): primary was NOT computed!"<<endl;
			}
			if (Result->isReesPrimary()) {
				out<<"ideal is primary to the ideal generated by the indeterminates"<<endl;
				if (Result->isComputed(ConeProperty::ReesPrimaryMultiplicity)) {
					out<<"multiplicity of the ideal = "<<Result->getReesPrimaryMultiplicity()<<endl;
				}
			} else {
				out<<"ideal is not primary to the ideal generated by the indeterminates"<<endl;
			}
			out << endl;
		}

		out << "***********************************************************************"
		    << endl << endl;


		if (nr_orig_gens > 0) {
			out << nr_orig_gens <<" original generators:"<<endl;
			Matrix<Integer>(Result->getGeneratorsOfToricRing()).pretty_print(out);
		}
		if (Result->isComputed(ConeProperty::HilbertBasis)) {
			Matrix<Integer> Hilbert_Basis = Result->getHilbertBasis();
			if (egn || typ) {
				Matrix<Integer> Hilbert_Basis_Full_Cone = BasisChange.to_sublattice(Hilbert_Basis);
				write_matrix_egn(Hilbert_Basis_Full_Cone);

				if (typ) {
					Matrix<Integer> V=Hilbert_Basis_Full_Cone.multiplication(BasisChange.to_sublattice_dual(Support_Hyperplanes).transpose());
					write_matrix_typ(V);
				}
			}

			write_matrix_gen(Hilbert_Basis);
			nr=Hilbert_Basis.nr_of_rows();
			out<<nr<<" Hilbert basis elements:"<<endl;
			Hilbert_Basis.pretty_print(out);
			if (type == OT_REES) {
				out << rees_ideal_key.size() <<" generators of integral closure of the ideal:"<<endl;
				Matrix<Integer> Ideal_Gens = Hilbert_Basis.submatrix(rees_ideal_key);
				Ideal_Gens.cut_columns(dim-1);
				Ideal_Gens.pretty_print(out);
			}
		}
		Matrix<Integer> Extreme_Rays;
		if (Result->isComputed(ConeProperty::ExtremeRays)) {
			Extreme_Rays = Result->getExtremeRays();          //write extreme rays
			size_t nr_ex_rays = Extreme_Rays.nr_of_rows();

			write_matrix_ext(Extreme_Rays);
			out<<nr_ex_rays<<" extreme rays:"<<endl;
			Extreme_Rays.pretty_print(out);
		}

		//write constrains (support hyperplanes, congruences, equations)

		out << Support_Hyperplanes.nr_of_rows() <<" support hyperplanes:"<<endl;
		Support_Hyperplanes.pretty_print(out);
		if (Result->isComputed(ConeProperty::ExtremeRays)) {
			//equations
			size_t dim = Extreme_Rays.nr_of_columns();
			size_t nr_of_equ = dim-rank;
			Matrix<Integer> Equations = Result->getEquations();
			if (nr_of_equ > 0) {
				out << nr_of_equ <<" equations:" <<endl;
				Equations.pretty_print(out);
			}

	
			//congruences
			Matrix<Integer> Congruences = Result->getCongruences();
			size_t nr_of_cong = Congruences.nr_of_rows();
			if (nr_of_cong > 0) {
				out << nr_of_cong <<" congruences:" <<endl;
				Congruences.pretty_print(out);
			}

			if(sup) {
				string cst_string = name+".cst";
				const char* cst_file = cst_string.c_str();
				ofstream cst_out(cst_file);
	
				Support_Hyperplanes.print(cst_out);
				cst_out<<"hyperplanes"<<endl;
				Equations.print(cst_out);
				cst_out<<"equations"<<endl;
				Congruences.print(cst_out);
				cst_out<<"congruences"<<endl;
	
				cst_out.close();
			}	
		}
		
		if (Result->isHt1ExtremeRays()) {
			if ( Result->isComputed(ConeProperty::Ht1Elements) ) {
				Matrix<Integer> Hom = Result->getHt1Elements();
				write_matrix_ht1(Hom);
				nr=Hom.nr_of_rows();
				out<<nr<<" Hilbert basis elements of height 1:"<<endl;
				Hom.pretty_print(out);
			}
		}
		out.close();
	}

	write_inv_file();
}

