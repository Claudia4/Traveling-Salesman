#ifndef INPUT_OUTPUT_HPP
#define INPUT_OUTPUT_HPP

#include <string>
#include <vector>
#include <utility>

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
std::vector<coordinate_pair> read_coordinates(std::string filename);

/*
Calculates the initial cost matrix from the vector of coordinates of points given
*/
std::vector<std::vector<double>> calculate_initial_cost_matrix(std::vector<coordinate_pair> * nodes);



/*************************************
OUTPUT SECTION
to be filled
*/
void write_output(std::string filename, std::vector<std::pair<unsigned int, unsigned int> > & tree, unsigned int n);

void print_matrix(std::vector<std::vector<double>> const & matrix);

/***********************************
OTHER
*/
/*
Returns the rounded euclidean distance between the points p1 and p2
*/
int distance(coordinate_pair p1, coordinate_pair p2);

#endif
