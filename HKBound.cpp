#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include<cmath>
#include <limits>
#include <algorithm>

#include "union_find.cpp"


int minimum_spanning_tree(std::vector<std::pair<unsigned int,unsigned int>> const & Edges, std::vector<std::pair<unsigned int,unsigned int>> & Tree, unsigned int size, unsigned int req) 
{	UF::UnionFind Components(size);
	Tree.clear();
	for(unsigned int i=0;i<size;i++){
		if(!Components.equals(Edges.at(i).first,Edges.at(i).second)) {
			Tree.push_back(Edges.at(i));
			Components.unite(Edges.at(i).first,Edges.at(i).second);
		}
		//too many required edges 
		else if(i<req) return 0;
	}
	//too many forbidden edges
	if(Tree.size()<size-2) return 0;
	
	return 1;
}

/*void merge(std::vector <std::pair<int,int>> const & considered, unsigned int bleft, unsigned int eleft,unsigned int bright,unsigned int eright, 
std::vector <std::vector<double>> const & Weights, std::vector <std::pair<int,int>> & sorted) {
	unsigned int ileft=bleft;
	int rsize=eright-bright+1;
	int lsize=eleft-bleft+1;
	for(int i=bleft; i++; i<=eright) {
		if(ileft>eleft) sorted.at(i)=considered.at(i);
		else if(ileft< i-rsize) {
			sorted.at(i)=considered.at(ileft);
			ileft++;
		}
		else if (Weights.at(considered.at(ileft).first).at(considered.at(ileft).second) < Weights.at(considered.at(i).first).at(considered.at(i).second)){
			sorted.at(i)=considered.at(ileft);
			ileft++;
		}
		else sorted.at(i)=considered.at(i-ileft+lsize);
	}
}

void Mergesort(std::vector <std::pair<int,int>> const & considered, std::vector <std::pair<int,int>> & sorted, unsigned int begin, unsigned int end,
std::vector <std::vector<double>> const & Weights) {
	unsigned int bright,bleft,eright,eleft;
	if(begin==end-1 || begin==end) return;
	
	bleft=begin;
	eleft=floor((end-begin+1)/2);
	bright=eleft+1;
	eright=end;
	Mergesort(considered, sorted, bleft, eleft, Weights);
	Mergesort(considered, sorted, bright, eright, Weights);
	
	merge(considered, bleft, eleft, bright, eright, Weights, sorted);
}*/

bool comparefunction(std::pair<std::pair<unsigned int,unsigned int>,double> i,std::pair<std::pair<unsigned int,unsigned int>,double> j) {return i.second<j.second;}

void sortEdges(std::vector <std::pair<unsigned int,unsigned int>> & Edges, std::vector <std::pair<unsigned int,unsigned int>> const & req, std::vector <std::pair<unsigned int,unsigned int>> const & forb, 
std::vector <std::vector<double>> const & Weights, unsigned int size) {
	Edges.clear();
	std::vector <std::pair<std::pair<unsigned int,unsigned int>,double>> considered;
	//std::vector <std::pair<int,int>> sorted(size-req.size()-forb.size());
	std::vector <std::vector<bool>> omitted(size, std::vector <bool>(size,0));
	for(unsigned int i=0; i<req.size();i++) {
		omitted.at(req.at(i).first).at(req.at(i).second)=1;
		omitted.at(req.at(i).second).at(req.at(i).first)=1;
		Edges.push_back(std::make_pair(req.at(i).first, req.at(i).second));
	}
	for(unsigned int i=0; i<forb.size();i++) {
		omitted.at(forb.at(i).first).at(forb.at(i).second)=1;
		omitted.at(forb.at(i).second).at(forb.at(i).first)=1;
	}
	//do not consider edges incident to vertex 0
	for(unsigned int i=1; i<size ; i++) {
		for(unsigned int j=1;j<i; j++) {
			if(!omitted.at(j).at(i)) considered.push_back(std::make_pair(std::make_pair(i,j), Weights.at(i).at(j)));
		}
	}
	std::sort(considered.begin(),considered.end(),comparefunction);
	//Mergesort(considered,sorted,0,considered.size(), Weights);
	for(unsigned int i=0;i<considered.size();i++) {
		Edges.push_back(considered.at(i).first);
	}
	
}


//ToDo store Tree with maximum HK bound
double Held_Karp_bound(std::vector <std::vector<double>> const & W, std::vector <double> & lambda, unsigned int size, double t,  unsigned int steps, std::vector <std::pair<unsigned int,unsigned int>> const & req,
std::vector <std::pair<unsigned int,unsigned int>> const & forb) {
	
	std::vector <std::vector<double>> Weights(size, std::vector <double>(size));
	std::vector <std::pair<unsigned int,unsigned int>> Edges;
	std::vector <unsigned int> degree(size,0);
	std::vector <std::pair<unsigned int,unsigned int>> Tree;
	
	double HK=0;
	double Treeweight=0;
	double delta=3*t/(2*steps);
	double ddelta=t/(steps*steps-steps);
	double firstmin= std::numeric_limits<double>::infinity();
	double secondmin= std::numeric_limits<double>::infinity();
	unsigned int first,second;
	
	//new weights
	for(unsigned int i=0;i<size; i++) {
		for(unsigned int j=0;j<size;j++) {
			Weights.at(i).at(j)=W.at(i).at(j)+lambda.at(i)+lambda.at(j);
		}
	}
	
	for(unsigned int k=0;k<steps;k++) {
		
		//MST
		sortEdges(Edges,req,forb,Weights,size);
		if(!minimum_spanning_tree(Edges,Tree,size,req.size())) return 0;
	
		//1-tree
		for(unsigned int i=0; i<size;i++) {
			if(Weights.at(0).at(i)<firstmin) {
				firstmin=Weights.at(0).at(i);
				first=i;
			}
			else if(Weights.at(0).at(i)<secondmin) {
				secondmin=Weights.at(0).at(i);
				second=i;
			}
		}
	
		Tree.push_back(std::make_pair(0,first));
		Tree.push_back(std::make_pair(0,second));
	
		//compute degrees
		for(unsigned int i=0;i<Tree.size();i++) {
			degree.at(Tree.at(i).first)++;
			degree.at(Tree.at(i).second)++;
		}
	
		//update HK
		for(unsigned int i=0;i<Tree.size();i++) {
			Treeweight+=Weights.at(Tree.at(i).first).at(Tree.at(i).second);
		}
		for(unsigned int i=0;i<size;i++) {
			Treeweight-=2*lambda.at(i);
		}
		
		//ToDo: store the Tree and lambda
		if(HK<Treeweight) HK=Treeweight;
		
		//update lambda
		for(unsigned int i=0;i<size;i++) {
			lambda.at(i)=lambda.at(i)+(degree.at(i)-2)*t;
			degree.at(i)=0;
		}
		//update weights 
		for(unsigned int i=0;i<size; i++) {
			for(unsigned int j=0;j<size;j++) {
				Weights.at(i).at(j)+=(degree.at(i)-2)*t+(degree.at(j)-2)*t;;
			}
		}
		
		//update t, delta
		t-=delta;
		delta-=ddelta;
		firstmin= std::numeric_limits<double>::infinity();
		secondmin= std::numeric_limits<double>::infinity();
	}
	return HK;
}

int main() {return 0;}