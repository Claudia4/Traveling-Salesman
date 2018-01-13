
#include <vector>
#include <string>

#include <iostream>

#include "input_output.cpp"

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
		print_matrix(graph);
		return 0;
	}
	if(argc == 5)
	{
		//print output file
		return 0;
	}
}
