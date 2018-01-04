
#include <vector>
#include <string>

#include <iostream>

#include "input_output.hpp"

int main(int argc, char* argv[])
{
	if (argc == 3 && argv[1] == "--instance"){
		std::vector<std::vector<double>> graph = read_graph(argv[2]);
		print_matrix(graph);
		return 0;
	}
	//ToDo: add possibility to give also a solution file for output
	else{
		std::cout << "Wrong number of arguments. One argument filename expected.";
		return 1;
	}
}
