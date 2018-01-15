#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <limits>
#include "input_output.cpp"

bool check_tour(std::vector<std::pair<unsigned int,unsigned int>> & Tree) {
	std::vector<unsigned int> degree(Tree.size(),0);
	for(unsigned int i=0;i<Tree.size();i++) {
		degree.at(Tree.at(i).first)++;
		degree.at(Tree.at(i).second)++;
	}
	for(unsigned int i=0;i<Tree.size();i++) {
		if(degree.at(i)!=2) return 1;
	}
	return 0;
}

bool minimum_spanning_tree(std::vector<std::pair<unsigned int,unsigned int>> & Tree, std::vector <std::vector<int>> const & omitted, 
std::vector <std::vector<double>> const & Weights, unsigned int req) {
	unsigned int size=Weights.size();
	std::vector<bool> visited(size,0);
	
	//min stores for each vertex an edge of minimum weight to the set of visited verticies
	std::vector<std::pair<double,unsigned int>> min(size,std::make_pair(std::numeric_limits<double>::infinity(),0));
	
	unsigned int vertex=1;
	unsigned int new_vertex=1;
	double minimum=std::numeric_limits<double>::infinity();
	
	//number of required edges in Tree
	unsigned int req_num=0;
	
	visited.at(1)=1;
	
	//build MST
	for(unsigned int j=0; j<size-2; j++) {
		//do not consider vertex 0 and 1
		for(unsigned int i=2; i<size;i++) {
			if(i!= vertex && !visited.at(i)) {
				//update the minimum weight of a cennection to visited verticies
				if(omitted.at(i).at(vertex)==1) {
					min.at(i).first=0;
					min.at(i).second=vertex;
				}
				else if(omitted.at(i).at(vertex)==0 && Weights.at(i).at(vertex)<min.at(i).first) {
					min.at(i).first=Weights.at(i).at(vertex);
					min.at(i).second=vertex;
				}
			}
		}
		//find an edge of minimum weight between visited and not visited verticies
		for(unsigned int i=2; i<size;i++) {
			if(!visited.at(i) && min.at(i).first<minimum) {
				minimum=min.at(i).first;
				new_vertex=i;
			}
		}
		//too many required edges
		if(new_vertex==vertex) return 1;
		
		//update number of required edges in Tree
		if(minimum==0) req_num++;
		
		//update vertex
		vertex=new_vertex;
		visited.at(vertex)=1;
		
		//insert corresponding edge in Tree
		Tree.at(j)=std::make_pair(vertex,min.at(vertex).second);
		minimum=std::numeric_limits<double>::infinity();
	}
	//too many required edges
	if(req_num<req) return 1;
	return 0;	
}

//ToDo store Tree with maximum HK bound
double Held_Karp_bound(std::vector <std::vector<double>> const & W, std::vector <double> & lambda,  double t,  unsigned int const  steps, std::vector <std::pair<unsigned int,unsigned int>> const & req, std::vector <std::pair<unsigned int,unsigned int>> const & forb ) {
	
	unsigned int size=W.size();
	
	std::vector <std::vector<double>> Weights(size, std::vector <double>(size));
	std::vector <int> degree(size,0);
	std::vector <std::pair<unsigned int,unsigned int>> Tree(size);
	std::vector <std::vector<int>> omitted(size, std::vector <int>(size,0));
	std::vector <std::pair<unsigned int,unsigned int>> OptTree(size);
	std::vector <double> OptLambda(size);
	
	double HK=0;
	double Treeweight=0;
	double delta=3*t/(2*steps);
	double ddelta=t/(steps*steps-steps);
	double firstmin= std::numeric_limits<double>::infinity();
	double secondmin= std::numeric_limits<double>::infinity();
	unsigned int first,second;
	
	
	//mark all required and forbidden edges
	for(unsigned int i=0; i<req.size();i++) {
		omitted.at(req.at(i).first).at(req.at(i).second)=1;
		omitted.at(req.at(i).second).at(req.at(i).first)=1;
	}
	for(unsigned int i=0; i<forb.size();i++) {
		omitted.at(forb.at(i).first).at(forb.at(i).second)=2;
		omitted.at(forb.at(i).second).at(forb.at(i).first)=2;
	}
	
	//new weights
	for(unsigned int i=0;i<size; i++) {
		for(unsigned int j=0;j<size;j++) {
			
			Weights.at(i).at(j)=W.at(i).at(j)+lambda.at(i)+lambda.at(j);
		}
	}
	
	for(unsigned int k=0;k<steps;k++) {
		
		//MST
		if(minimum_spanning_tree(Tree,omitted, Weights,req.size())) return 0;
	
		//1-tree
		for(unsigned int i=1; i<size;i++) {
			if(Weights.at(0).at(i)<firstmin) {
				firstmin=Weights.at(0).at(i);
				first=i;
			}
			else if(Weights.at(0).at(i)<secondmin) {
				secondmin=Weights.at(0).at(i);
				second=i;
			}
		}
	
		Tree.at(Tree.size()-2)=std::make_pair(0,first);
		Tree.at(Tree.size()-1)=std::make_pair(0,second);
	
		//compute degrees
		for(unsigned int i=0;i<Tree.size();i++) {
			degree.at(Tree.at(i).first)++;
			degree.at(Tree.at(i).second)++;
		}
		/*
		for(unsigned int i=0;i<Tree.size();i++) {
		std::cout<<Tree.at(i).first<<" "<<Tree.at(i).second<<"\n";
		}*/
		//update HK
		for(unsigned int i=0;i<Tree.size();i++) {
			Treeweight+=Weights.at(Tree.at(i).first).at(Tree.at(i).second);
			Treeweight-=2*lambda.at(i);
		}
		std::cout<< HK<<"\n";
		//store Tree and lambda
		if(HK<Treeweight) {
			HK=Treeweight;
			for(unsigned int i=0;i<OptTree.size();i++) {
				OptTree.at(i)=Tree.at(i);
				OptLambda.at(i)=lambda.at(i);
			}
		}
		
		//update weights
		for(unsigned int i=0;i<size; i++) {
			for(unsigned int j=0;j<size;j++) {
				Weights.at(i).at(j)+=(degree.at(i)-2)*t+(degree.at(j)-2)*t;;
			}
		}
		
		//update lambda
		for(unsigned int i=0;i<size;i++) {
			lambda.at(i)=lambda.at(i)+(degree.at(i)-2)*t;
			//std::cout<<lambda.at(i)<<"\n";
			degree.at(i)=0;
		}
		 
		//update t, delta
		t-=delta;
		delta-=ddelta;
		Treeweight=0;
		firstmin= std::numeric_limits<double>::infinity();
		secondmin= std::numeric_limits<double>::infinity();
	}
	return HK;
}
double scalar(std::vector <std::vector<double>> const & W) {
	std::vector<std::pair<unsigned int,unsigned int>> Tree(W.size());
	std::vector <std::vector<int>> const omitted(W.size(),std::vector<int>(W.size(),0));
	double t=0;
	double firstmin= std::numeric_limits<double>::infinity();
	double secondmin= std::numeric_limits<double>::infinity();
	unsigned int first,second;
	
	minimum_spanning_tree(Tree,omitted,W,0);
	
	for(unsigned int i=1; i<Tree.size();i++) {
		if(W.at(0).at(i)<firstmin) {
			firstmin=W.at(0).at(i);
			first=i;
		}
		else if(W.at(0).at(i)<secondmin) {
			secondmin=W.at(0).at(i);
			second=i;
		}	
	}
	Tree.at(Tree.size()-2)=std::make_pair(0,first);
	Tree.at(Tree.size()-1)=std::make_pair(0,second);
	
	for(unsigned int i=0;i<Tree.size();i++) {
		t+=W.at(Tree.at(i).first).at(Tree.at(i).second);
	} 
	return t;
}

void Branch_and_Bound(std::vector <std::vector<double>> const & W) {
		//ToDo Queue anlegen
		//compute upper bound
		double U=W.at(W.size()-1).at(0);
		double t;
		unsigned int N;
		unsigned int size=W.size();
		
		for(unsigned int i=0; i<W.size()-1;i++) {
			U+=W.at(i).at(i+1);
		}
		//initialization of N and t
		//for root:
		t=scalar(W);
		N=floor(size*size/50)+1+size+15;
		
		return;
}




int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Wrong number of arguments. One argument filename expected.";
		return 1;
	}
 	std::string arg (argv[1]);
	if(arg.compare("--instance") == 0)
	{
		std::vector<std::vector<double>> graph = read_graph(argv[2]);
		std::vector <double> lambda(graph.size(),0);
		std::vector <std::pair<unsigned int,unsigned int>> req(0);
		std::cout<<Held_Karp_bound(graph,lambda,3,500,req,req);
		
		//print_matrix(graph);
		return 0;
	}
	if(argc == 5)
	{
		//print output file
		return 0;
	}
}
