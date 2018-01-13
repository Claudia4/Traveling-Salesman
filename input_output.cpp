#include "input_output.hpp"

//Streams for input/output
#include <fstream>
#include <sstream>
#include <iostream>

//Datatypes
#include <string>
#include <vector>


/***************************************************
INPUT SECTION
*/


/*
Reads a given file <filename> that is expected to be in TSPLIB format
extracts the coordinates of the vertices of the graph and writes them to the array nodes.
The vector nodes is expected to be empty when this method is called on it.
*/
std::vector<coordinate_pair> read_coordinates(std::string filename)
{
	//Open the file filename
	std::ifstream infile(filename);
	std::string line;
	
	if (!infile){
		std::cerr << "Could not read file "+filename;
		return std::vector<coordinate_pair> ();
	}
	
	std::istringstream iss;
	std::string s;
	unsigned int n = 0;

	//find the problem dimension and proceed to the 'node coord section', ignoring all other lines
	while(std::getline(infile, line)){
		iss.str (line);
		iss.clear();
		iss >> s;
		if (s == "DIMENSION:"){
			iss >> n;
		}
		if (s == "NODE_COORD_SECTION"){
			break;
		}
	}
	
	if (n == 0){
		std::cerr << "Input file doesn't meet the specification or specifies an emtpy graph.";
		infile.close();
		return std::vector<coordinate_pair> ();
	}
	
	//read the 'node coord section' and fill the vector nodes with coordinate_pairs
	std::vector<coordinate_pair> nodes;
	nodes.reserve(n);
	unsigned int i;
	double x, y;
	while(std::getline(infile, line)){
		if (line == "EOF"){
			break;
		}
		iss.str (line);
		iss.clear();
		iss >> i >> x >> y;
		if (nodes.size() == i-1){
			nodes.push_back(coordinate_pair {x, y});
		}
		else
		{} //ToDo: enter the node in the right position. For most files, this case will not occur, but we probably have to allow it.
	}

	if (n != nodes.size())
	{
		std::cerr << "The number of vertices doesn't match the specification.";
	}
	infile.close();
	return nodes;
}


/*
Calculates the initial cost matrix from the vector of coordinates of points given
*/
std::vector<std::vector<double>> calculate_initial_cost_matrix(std::vector<coordinate_pair>& nodes)
{
	unsigned int n = nodes.size();
	std::vector<std::vector<double>> result (n, std::vector<double> (n, 0.0));
	for (unsigned int i = 0; i<n; i++)
	{
		for (unsigned int j = 0; j<i; j++)
		{
			double dist = distance(nodes[i], nodes[j]);
			result[i][j] = dist;
			result[j][i] = dist;
		}
	}
	return result;
}

/*
Perform the whole reading-in process of the graph
using read_coordinates to actually read the file
and calculate_initial_cost_matrix to calculate the cost matrix from the coordinates read
*/
std::vector<std::vector<double>> read_graph(std::string filename)
{
	std::vector<coordinate_pair> nodes = read_coordinates(filename);
	std::vector<std::vector<double>> matrix = calculate_initial_cost_matrix(nodes);
	return matrix;
}


/***************************************************
OUTPUT SECTION
*/

void print_matrix(std::vector<std::vector<double>> const & matrix)
{
	for (unsigned int i=0; i<matrix.size(); i++)
	{
		for (unsigned int j=0; j<matrix[i].size(); j++)
		{
			std::cout << std::to_string(matrix[i][j]) << " ";
		}
		std::cout << "\n";
	}
}

/***********************************
OTHER
*/
int distance(coordinate_pair p1, coordinate_pair p2)
{
	return std::lround(std::sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)));
}
//Here will be something like 
//void write_tour(std::string filename, some_tour)
