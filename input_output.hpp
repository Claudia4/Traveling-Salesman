#ifndef INPUT_OUTPUT_HPP
#define INPUT_OUTPUT_HPP

#include <string>
#include <vector>

#include <iostream>

#include <cmath>

/*********************************
INPUT SECTION
*/
/*
Coordinate pair of a point in \R^2
*/
struct coordinate_pair
{
	double x;
	double y;
};

/*
Perform the whole reading-in process of the graph
using read_coordinates to actually read the file
and calculate_initial_cost_matrix to calculate the cost matrix from the coordinates read
*/
std::vector<std::vector<double>> read_graph(std::string filename);

/*
Reads a given file <filename> that is expected to be in TSPLIB format
extracts the coordinates of the vertices of the graph and writes them to the array nodes.
The vector nodes is expected to be empty when this method is called on it.
*/
void read_coordinates(std::string filename, std::vector<coordinate_pair> & nodes);

/*
Calculates the initial cost matrix from the vector of coordinates of points given
*/
std::vector<std::vector<double>> calculate_initial_cost_matrix(std::vector<coordinate_pair> & nodes);



/*************************************
OUTPUT SECTION
to be filled
*/


void print_matrix(std::vector<std::vector<double>> const & matrix)
{
	for (unsigned int i=0; i<matrix.size(); i++)
	{
		for (unsigned int j=0; j<matrix[i].size(); j++)
		{
			std::cout << std::to_string(matrix[i][j]);
		}
	}
}

/***********************************
OTHER
*/
/*
Returns the rounded euclidean distance between the points p1 and p2
*/
int distance(coordinate_pair p1, coordinate_pair p2)
{
	return std::lround(std::sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)));
}

#endif
