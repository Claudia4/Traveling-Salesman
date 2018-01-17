#include <iostream>
#include <utility>
#include <tuple>
#include <vector>
#include <limits>
#include "input_output.cpp"
#include "queue.cpp"

//Returns the index of a vertex adjacent to v by a required edge, if it exists, and n otherwise
unsigned int is_incident_to_required(QNode & current_node, unsigned int v, unsigned int n)
{
	for (unsigned int i=0; i<current_node.R.size(); i++)
	{
		if (current_node.R.at(i).first == v)
		{
			return current_node.R.at(i).second;
		}
		if( current_node.R.at(i).second == v)
		{
			return current_node.R.at(i).first;
		}
	}
	return n;
}

unsigned int is_in_tree(std::vector<std::pair<unsigned int, unsigned int> > const & tree, unsigned int i, unsigned int j)
{
	for (unsigned int k=0; k<tree.size(); k++)
	{
		if((tree.at(k).first == i && tree.at(k).second == j) || (tree.at(k).first == j && tree.at(k).second == i))
		{
			return 1;
		}
	}
	return 0;
}

unsigned int is_forbidden(QNode const & node, unsigned int i, unsigned int j)
{
	for (unsigned int k=0; k<node.F.size(); k++)
	{
		if((node.F.at(k).first == i && node.F.at(k).second == j) || (node.F.at(k).first == j && node.F.at(k).second == i))
		{
			return 1;
		}
	}
	return 0;
}

std::vector<QNode> branch(std::vector<std::pair<unsigned int, unsigned int> > const & tree, std::vector<int> const & degrees, QNode & current_node, unsigned int n)
{
	std::vector<QNode> result;
	//assuming that we already checked wether tree is a tour
	unsigned int min_degree_req = n; //minimal degree greater than two of a vertex incident to a required edge
	unsigned int min_degree = n; //minimal degree greater than two of a vertex not incident to a required edge
	unsigned int p_req; //vertex where min_degree_req was attained
	unsigned int p; //vertex where min_degree was attained
	unsigned int req_neighbor;
	
	for (unsigned int i = 0; i<n; i++)
	{
		if (degrees.at(i) > 2)
		{
			if (is_incident_to_required(current_node, i, n) == n)
			{
				if ((unsigned int)degrees.at(i) < min_degree)
				{
					min_degree = degrees.at(i);
					p = i;
				}
			}
			else
			{
				if ((unsigned int)degrees.at(i) < min_degree_req)
				{
					min_degree_req = degrees.at(i);
					p_req = i;
					req_neighbor = is_incident_to_required(current_node, i, n);
				}
			}
		}
	}
	if (min_degree_req < n) //branching with two new vertices
	{
		//choose e_1 that is not in the tree, add S_2 and S_3 (with notation as in the paper)
		unsigned int i;
		for (i=0; i<n; i++)
		{
			if(i!=p_req && i!=req_neighbor)
			{
				if(! is_in_tree(tree, i, p_req) && ! is_forbidden(current_node, i, p_req))
				{
					//we may take e_1 = {i, p}
					break;
				}
			}
		}
		if (i==n) //all non-tree edges incident to p are forbidden, choose a tree edge as e_1
		{	
			for (i=0; i<n; i++)
			{
				if(i!=p_req && i!=req_neighbor)
				{
					if(! is_forbidden(current_node, i, p_req))
					{
						//we may take e_1 = {i, p}
						break;
					}
				}
			}
		}
		std::vector<std::pair<unsigned int, unsigned int> > R = current_node.R;
		R.push_back(std::pair<unsigned int, unsigned int> (i,p_req));
		std::vector<std::pair<unsigned int, unsigned int> > F = current_node.F;
		//forbid all edges incident to p except the two required ones
		for (unsigned int k=0; k<n; k++)
		{
			if(k!=i && k!=p_req && k!=req_neighbor && !is_forbidden(current_node, p_req, k))
			{
				F.push_back(std::pair<unsigned int, unsigned int> (p_req, k));
			}
		}
		QNode S2 = QNode(R, F, current_node.lambda, n);
		result.push_back(S2);

		R = current_node.R;
		F = current_node.F;
		F.push_back(std::pair<unsigned int, unsigned int> (i, p_req));
		
		//check if all but two are forbidden, if so, require the two other ones
		std::vector<unsigned int> forbidden (n, 0);
		forbidden[p_req] = 1;
		unsigned int num_forbidden = 0;
		for(unsigned int k = 0; k<F.size(); k++)
		{
			if(F.at(k).first == p_req)
			{
				forbidden[F.at(k).second] = 1;
				num_forbidden++;
			}
			if(F.at(k).second == p_req)
			{
				forbidden[F.at(k).first] = 1;
				num_forbidden++;
			}
		}
		if(num_forbidden == n-3)
		{
			for (unsigned int k=0; k<n; k++)
			{
				if(! forbidden[k])
				{
					if (k!=req_neighbor)
					{
						R.push_back(std::pair<unsigned int, unsigned int> (p_req, k));
					}
				}
			}
		}
		QNode S3 = QNode(R, F, current_node.lambda, n);
		result.push_back(S3);
	}
	else //branching with three new vertices
	{
		//choose e_1 that is not in the tree and some e_2, add S_1, S_2 and S_3
		unsigned int i, j;
		for (i=0; i<n; i++)
		{
			if(i!=p && ! is_in_tree(tree, i, p) && ! is_forbidden(current_node, i, p))
			{
				//we may take e_1 = {i, p}
				break;
			}
		}
		if (i==n) //all non-tree edges incident to p are forbidden, choose a tree edge as e_1
		{	
			for (i=0; i<n; i++)
			{
				if(i!= p && ! is_forbidden(current_node, i, p))
				{
					//we may take e_1 = {i, p}
					break;
				}
			}
		}
		for (j=0; j<n; j++)
		{
			if(j!=p && j!=i)
			{
				if(! is_forbidden(current_node, j, p))
				{
					//we may take e_2 = {j, p}
					break;
				}
			}
		}
		std::vector<std::pair<unsigned int, unsigned int> > R = current_node.R;
		R.push_back(std::pair<unsigned int, unsigned int> (i,p));
		R.push_back(std::pair<unsigned int, unsigned int> (j,p));
		std::vector<std::pair<unsigned int, unsigned int> > F = current_node.F;
		//forbid all edges incident to p except the two required ones
		for (unsigned int k=0; k<n; k++)
		{
			if(k!=i && k!=p && k!=j && !is_forbidden(current_node, p, k))
			{
				F.push_back(std::pair<unsigned int, unsigned int> (p, k));
			}
		}
		QNode S1 = QNode(R, F, current_node.lambda, n);
		result.push_back(S1);

		R = current_node.R;
		F = current_node.F;
		R.push_back(std::pair<unsigned int, unsigned int> (i, p));
		F.push_back(std::pair<unsigned int, unsigned int> (j,p));
		//check if all but two are forbidden, if so, require the two other ones
		std::vector<unsigned int> forbidden (n, 0);
		forbidden[p] = 1;
		unsigned int num_forbidden = 0;

		for(unsigned int k = 0; k<F.size(); k++)
		{
			if(F.at(k).first == p)
			{
				forbidden[F.at(k).second] = 1;
				num_forbidden++;
			}
			if(F.at(k).second == p)
			{
				forbidden[F.at(k).first] = 1;
				num_forbidden++;
			}
		}
		if(num_forbidden == n-3)
		{
			for (unsigned int k=0; k<n; k++)
			{
				if(! forbidden[k])
				{
					if (k!=i)
					{
						R.push_back(std::pair<unsigned int, unsigned int> (p, k));
					}
				}
			}
		}
		QNode S2 = QNode(R, F, current_node.lambda, n);
		result.push_back(S2);

		R = current_node.R;
		F = current_node.F;
		F.push_back(std::pair<unsigned int, unsigned int> (i,p));
		if(num_forbidden == n-3)
		{
			//we can reuse forbidden, but we have to be careful because it should be a bit different now
			for (unsigned int k=0; k<n; k++)
			{
				if(! forbidden[k])
				{
					if (k!=i)
					{
						R.push_back(std::pair<unsigned int, unsigned int> (p, k));
					}
				}
			}
			R.push_back(std::pair<unsigned int, unsigned int> (p, j));
		}
		QNode S3 = QNode(R, F, current_node.lambda, n);
		result.push_back(S3);
	}
	return result;
}

bool check_tour(std::vector<std::pair<unsigned int,unsigned int>> const & Tree) {
	std::vector<unsigned int> degree(Tree.size(),0);
	for(unsigned int i=0;i<Tree.size();i++) {
		degree.at(Tree.at(i).first)++;
		degree.at(Tree.at(i).second)++;
	}
	//check whether the degree of each vertex is two
	for(unsigned int i=0;i<Tree.size();i++) {
		if(degree.at(i)!=2) return 1;
	}
	return 0;
}

bool minimum_spanning_tree(std::vector<std::pair<unsigned int,unsigned int>> & Tree, std::vector <std::vector<int>> const & omitted, 
std::vector <std::vector<double>> const & Weights, unsigned int const req) {
	unsigned int size=Weights.size();
	std::vector<bool> visited(size,0);
	
	//min stores for each vertex an edge of minimum weight incident to the set of visited verticies
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
		//too many forbidden edges
		if(new_vertex==vertex) return 1;
		
		//update number of required edges in Tree
		if(omitted.at(new_vertex).at(min.at(new_vertex).second)) req_num++;
		
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


//Node contains required edges, forbidden edges, empty tree and initial lambda value from the parent branching node
bool Held_Karp_bound(std::vector <std::vector<double>> const & W, QNode & Node, std::vector <int> & degree, double t,  unsigned int const  steps) {
	
	unsigned int size=W.size();
	
	std::vector <std::vector<double>> Weights(size, std::vector <double>(size));
	std::vector <std::pair<unsigned int,unsigned int>> Tree(size);
	std::vector <std::vector<int>> omitted(size, std::vector <int>(size,0));
	std::vector <double> OptLambda(size);
	
	unsigned int req=Node.R.size();
	double Treeweight=0;
	double delta=3*t/(2*steps);
	double ddelta=t/(steps*steps-steps);
	double firstmin= std::numeric_limits<double>::infinity();
	double secondmin= std::numeric_limits<double>::infinity();
	unsigned int first,second;
	unsigned int req1=0;
	unsigned int req2=0;
	std::vector<bool> forbidden(size,0);
	
	for(unsigned int i=0;i<size;i++) {
			degree.at(i)=0;
	}
	
	//mark all required and forbidden edges
	//if edges incident to 0 are required we store them
	for(unsigned int i=0; i<(Node.R).size();i++) {
		omitted.at((Node.R).at(i).first).at((Node.R).at(i).second)=1;
		omitted.at((Node.R).at(i).second).at((Node.R).at(i).first)=1;
		if((Node.R).at(i).first==0) {
			req--;
			if(req<Node.R.size()-1){
				req2=(Node.R).at(i).second;
			}
			else req1=(Node.R).at(i).second;
			
		}
		if((Node.R).at(i).second==0) {
			req--;
			if(req<Node.R.size()-1){
				req2=(Node.R).at(i).first;
			}
			else req1=(Node.R).at(i).first;
			
		}
	}
	//if edges incident to 0 are forbidden we mark them
	for(unsigned int i=0; i<(Node.F).size();i++) {
		omitted.at((Node.F).at(i).first).at((Node.F).at(i).second)=2;
		omitted.at((Node.F).at(i).second).at((Node.F).at(i).first)=2;
		if((Node.F).at(i).first==0) forbidden.at((Node.F).at(i).second)=1;
		if((Node.F).at(i).second==0) forbidden.at((Node.F).at(i).first)=1;
	}
	
	//compute new weights
	for(unsigned int i=0;i<size; i++) {
		for(unsigned int j=0;j<size;j++) {
			Weights.at(i).at(j)=W.at(i).at(j)+(Node.lambda).at(i)+(Node.lambda).at(j);
		}
	}
	
	for(unsigned int k=0;k<steps;k++) {
		
		//compute MST, if MST does not exist return 1
		if(minimum_spanning_tree(Tree,omitted, Weights,req)) return 1;
	
		//1-tree
		for(unsigned int i=1; i<size;i++) {
			if(Weights.at(0).at(i)<firstmin && !forbidden.at(i)) {
				firstmin=Weights.at(0).at(i);
				first=i;
			}
			else if(Weights.at(0).at(i)<secondmin && !forbidden.at(i)) {
				secondmin=Weights.at(0).at(i);
				second=i;
			}
		}
		if(req1==0) {
			Tree.at(Tree.size()-2)=std::make_pair(0,first);
			Tree.at(Tree.size()-1)=std::make_pair(0,second);
		}
		else if(req2==0) {
			Tree.at(Tree.size()-2)=std::make_pair(0,first);
			Tree.at(Tree.size()-1)=std::make_pair(0,req1);
		}
		else {
			Tree.at(Tree.size()-2)=std::make_pair(0,req1);
			Tree.at(Tree.size()-1)=std::make_pair(0,req2);
		}
	
		//compute degrees
		for(unsigned int i=0;i<Tree.size();i++) {
			degree.at(Tree.at(i).first)++;
			degree.at(Tree.at(i).second)++;
		}

		//update HK
		for(unsigned int i=0;i<Tree.size();i++) {
			Treeweight+=Weights.at(Tree.at(i).first).at(Tree.at(i).second);
			Treeweight-=2*(Node.lambda).at(i);
		}
		//store Tree and lambda
		if(Node.HK<Treeweight) {
			Node.HK=Treeweight;
			for(unsigned int i=0;i<(Node.one_tree).size();i++) {
				(Node.one_tree).at(i)=Tree.at(i);
				OptLambda.at(i)=(Node.lambda).at(i);
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
			(Node.lambda).at(i)+=(degree.at(i)-2)*t;
			degree.at(i)=0;
		}
		 
		//update t, delta
		t-=delta;
		delta-=ddelta;
		Treeweight=0;
		firstmin= std::numeric_limits<double>::infinity();
		secondmin= std::numeric_limits<double>::infinity();
	}
	
	for(unsigned int i=0; i< size;i++) {
		(Node.lambda).at(i)=OptLambda.at(i);
		degree.at(Node.one_tree.at(i).first)++;
		degree.at(Node.one_tree.at(i).second)++;
	}
	return 0;
}

double initial_value(std::vector <std::vector<double>> const & W) {
	std::vector<std::pair<unsigned int,unsigned int>> Tree(W.size());
	std::vector <std::vector<int>> omitted(W.size(),std::vector<int>(W.size(),0));
	double t=0;
	double firstmin= std::numeric_limits<double>::infinity();
	double secondmin= std::numeric_limits<double>::infinity();
	unsigned int first,second;
	
	//compute MST
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
	
	//one_tree
	Tree.at(Tree.size()-2)=std::make_pair(0,first);
	Tree.at(Tree.size()-1)=std::make_pair(0,second);
	
	//compute 
	for(unsigned int i=0;i<Tree.size();i++) {
		t+=W.at(Tree.at(i).first).at(Tree.at(i).second);
	} 
	return t/(2*W.size());
}

//insert a branching node new_elem in the queue L wrt the Held-Karp-Bound of new_elem 
void insert(std::vector<QNode> & L, QNode const & new_elem) {
	L.push_back(new_elem);
	QNode tmp;
	unsigned int position=L.size()-1;
	while(position>0 && L.at(position-1).HK<L.at(position).HK) {
		tmp=L.at(position);
		L.at(position)=L.at(position-1);
		L.at(position-1)=tmp;
		position--;
	}
}

std::pair<std::vector<std::pair<unsigned int, unsigned int>>, double> Branch_and_Bound(std::vector <std::vector<double>> const & W) {
		unsigned int size=W.size();
		std::vector <int> degree(size,0);
		double U=W.at(size-1).at(0);
		double t;
		unsigned int N;
		
		std::vector<std::pair<unsigned int, unsigned int>> Opt(size);
		
		Opt.at(0)=std::make_pair(size-1,0);
		//compute upper bound
		for(unsigned int i=0; i<W.size()-1;i++) {
			U+=W.at(i).at(i+1);
			Opt.at(i+1)=std::make_pair(i,i+1);
		}
		//initialization of N and t
		//for root:
		t=initial_value(W);
		N=(unsigned int)ceil((double)size*(double)size/50.)+size+15;
		
		//computing lower bound
		QNode root(std::vector<std::pair<unsigned int, unsigned int>>(), std::vector<std::pair<unsigned int, unsigned int>>(), std::vector<double>(size), size);
		Held_Karp_bound(W, root, degree, t, N);
		
		//if a tour is already found, it is optimum
		if(!check_tour(root.one_tree)) {
			for(unsigned int i=0;i<size;i++) {
				Opt.at(i)=root.one_tree.at(i);
			}
			return std::make_pair(Opt, root.HK);
		}
		
		//initialization of N and t for other verticies
		N=(unsigned int)ceil((double)size/4.)+5;
		t=0;
		for(unsigned int i=0; i< size ; i++) {
			t+=std::abs((root.lambda).at(i));
		}
		t=t/(2.*size);
		
		//begin of branching
		std::vector<QNode> B = branch(root.one_tree, degree, root, size);
		//queue
		std::vector<QNode> S;
		for(unsigned int i=0; i< B.size(); i++) {
			if(!Held_Karp_bound(W, B.at(i), degree, t, N)){
				if(B.at(i).HK<U) {
					insert(S,B.at(i));
				}
			}
		}
		
		std::vector <int> degree1(size,0);
		int current=S.size()-1;
		
		//Branch and Bound using best bound
		while(current>=0){
			QNode node=S.at(current);
			S.pop_back();
			
			//since we use best bound, node.HK is a lower bound for the optimum
			if(ceil(node.HK)>=U) return std::make_pair(Opt, U);
			
			//consider node only if its HK bound is smaller than U
			if(node.HK<U) {
				//std::cout<<node.HK<<"\n";
				for(unsigned int i=0; i< size ; i++) {
					degree1.at(node.one_tree.at(i).first)++;
					degree1.at(node.one_tree.at(i).second)++;
				}
				//check whether the current one_tree is a tour 
				if(!check_tour(node.one_tree)) { 
					//update U
					U=node.HK;
					//update Opt
					for(unsigned int i=0;i<size;i++) {
						Opt.at(i)=node.one_tree.at(i);
					}
				}
				else {
					//branch with node
					std::vector<QNode> B= branch(node.one_tree,degree1,node,size);
					for(unsigned int i=0; i< B.size(); i++) {
						if(!Held_Karp_bound(W, B.at(i), degree1, t, N)) {
							//consider B.at(i) only if its HK bound is smaller than U
							if(B.at(i).HK<U){
								//check whether we found a tour
								if(!check_tour(B.at(i).one_tree)) { 
									U=B.at(i).HK;
									for(unsigned int k=0;k<size;k++) {
										Opt.at(k)=B.at(i).one_tree.at(k);
									}
								}
								//insert B.at(i) wrt Held-Karp-Bound
								insert(S,B.at(i));
								current++;
							}
						}
					}
				}
			}
			for(unsigned int k=0; k<size;k++) degree1.at(k)=0;
			current--;
		}
	return std::make_pair(Opt, U);
}




int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Required first argument '--instance <filename>' missing.";
		return 1;
	}
	if(argc!=3 && argc!=5) {
		std::cout<<"Wrong number of arguments.";
		return 1;
	}
 	std::string arg (argv[1]);
	std::vector<std::vector<double>> graph;
	
	if(arg.compare("--instance") != 0)
	{
		std::cout << "Required first argument '--instance <filename>' missing or misspelled.";
		return 1;
	}
	try {
		graph = read_graph(argv[2]);
	}
	catch (std::logic_error e) 
	{
		std::cout<<e.what();
		return 1;
	}
	std::vector<std::pair<unsigned int, unsigned int>>Tree;
	double OPT=0;
	std::tie( Tree, OPT)=Branch_and_Bound(graph);
	std::cout<<OPT;
	//print_matrix(graph);

	if(argc == 5)
	{	
		std::string arg (argv[3]);
		if(arg.compare("--solution") == 0)
		{
			std::string filename = argv[4];
			write_output(filename, Tree, graph.size());
		}
		else
		{
			std::cout << "Optional second argument '--solution <filename>' misspelled.";
			return 1;
		}
	}
	return 0;
}
