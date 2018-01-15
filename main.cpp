
#include <vector>
#include <string>
#include <utility>

#include <iostream>

#include "input_output.cpp"
#include "queue.cpp"
//#include "HKBound.cpp"

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

std::vector<QNode> branch(std::vector<std::pair<unsigned int, unsigned int> > const & tree, std::vector<unsigned int> const & degrees, QNode & current_node, unsigned int n)
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
				if (degrees.at(i) < min_degree)
				{
					min_degree = degrees.at(i);
					p = i;
				}
			}
			else
			{
				if (degrees.at(i) < min_degree_req)
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
			if(i!=p && i!=req_neighbor)
			{
				if(! is_in_tree(tree, i, p) && ! is_forbidden(current_node, i, p))
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
				if(i!=p && i!=req_neighbor)
				{
					if(! is_forbidden(current_node, i, p))
					{
						//we may take e_1 = {i, p}
						break;
					}
				}
			}
		}
		std::vector<std::pair<unsigned int, unsigned int> > R = current_node.R;
		R.push_back(std::pair<unsigned int, unsigned int> (i,p));
		std::vector<std::pair<unsigned int, unsigned int> > F = current_node.F;
		//forbid all edges incident to p except the two required ones
		for (unsigned int k=0; k<n; k++)
		{
			if(k!=i && k!=p && k!=req_neighbor && !is_forbidden(current_node, p, k))
			{
				F.push_back(std::pair<unsigned int, unsigned int> (p, k));
			}
		}
		QNode S2 = QNode(R, F, current_node.lambda, n);
		result.push_back(S2);

		R = current_node.R;
		F = current_node.F;
		F.push_back(std::pair<unsigned int, unsigned int> (i, p));
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
					if (k!=req_neighbor)
					{
						R.push_back(std::pair<unsigned int, unsigned int> (p, k));
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
				F.pushback(std::pair<unsigned int, unsigned int> (p, k));
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
		unsigned int num_forbidden = 0;<unsigned int, unsigned int>
		for(unsigned int k = 0; k<F.size(); k++)
		{
			if(F.at(k).first == p)
			{
				forbidden[F.at(k).second] = 1;
				num_forbidden++;
			}
			if(F.at(k).second == p)
			{
				forbidden[f.at(k).first] = 1;
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

void branch_and_bound(std::vector<std::vector<double> > graph)
{
	List Q = List();
	QNode first = QNode(std::vector<std::pair<unsigned int, unsigned int> > (), std::vector<std::pair<unsigned int, unsigned int> > ());
	//first iteration of HKBound for first with bigger number of steps
	Q.insert(first);
	
	
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Required first argument '--instance <filename>' missing.";
		return 1;
	}
 	std::string arg (argv[1]);
	if(arg.compare("--instance") != 0)
	{
		std::cout << "Required first argument '--instance <filename>' missing or misspelled.";
		return 1;
	}
	std::vector<std::vector<double>> graph = read_graph(argv[2]);
	print_matrix(graph);
	//do other stuff here
	std::vector<std::pair<unsigned int, unsigned int> > tree;

	//std::cout << cost of tour
	if(argc == 5)
	{
		std::string arg (argv[3]);
		if(arg.compare("--solution") == 0)
		{
			std::string filename = argv[4];
			write_output(filename, tree, graph.size());
		}
		else
		{
			std::cout << "Optional second argument '--solution <filename>' misspelled.";
			return 1;
		}
	}
	return 0;
}
