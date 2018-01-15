
#include <vector>
#include <string>
#include <utility>

#include <iostream>

#include "input_output.cpp"
#include "queue.cpp"
#include "HKBound.cpp"

//Returns the index of a vertex adjacent to v by a required edge, if it exists, and n otherwise
unsigned int is_incident_to_required(QNode & current_node, unsigned int v, unsigned int n)
{
	for (unsigned int i=0; i<current_node.content.R.size(); i++)
	{
		if (current_node.content.R.at(i).first == v)
		{
			return current_node.content.R.at(i).second;
		}
		if( current_node.content.R.at(i).second == v)
		{
			return current_node.content.R.at(i).first;
		}
	}
	return n;
}

void branch(List & Q, std::vector<std::pair<unsigned int, unsigned int> > const & tree, std::vector<unsigned int> const & degrees, QNode & current_node, unsigned int n)
{
	//assuming that we already checked wether tree is a tour
	unsigned int min_degree_req = n; //minimal degree greater than two of a vertex incident to a required edge
	unsigned int min_degree = n; //minimal degree greater than two of a vertex not incident to a required edge
	unsigned int p_req; //vertex where min_degree_req was attained
	unsigned int p; //vertex where min_degree was attained
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
				}
			}
		}
	}
	if (min_degree_req < n) //branching with two new vertices
	{
		
	}
	else //branching with three new vertices
	{
	}
	
}

void branch_and_bound(std::vector<std::vecor<double> > graph)
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
