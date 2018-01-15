#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <vector>
#include <utility>

/*
Vertex of the Branch&Bound tree
to save required (R) and forbidden (F) edges 
and the Held-Karp bound (HK) and the lambda vector for which it was attained
*/
class QNode
{
public:
	std::vector<std::pair<unsigned int, unsigned int> > R;
	std::vector<std::pair<unsigned int, unsigned int> > F;
	std::vector<double> lambda;
	std::vector<std::pair<unsigned int,unsigned int>> one_tree;
	double HK;
	QNode(std::vector<std::pair<unsigned int, unsigned int>> R, std::vector<std::pair<unsigned int, unsigned int>> F);
};

/*
List element for the class list (singly linked list)
to store a priority queue of QNodes
*/
class ListElement
{
public:
	QNode content;
	ListElement* successor;
	
	ListElement(QNode content, ListElement* successor);
	//Recursive insertion of list elements sorted by content.HK
	void insert(QNode node);
};

/*
List class for singly linked list using ListElements
to store a priority queue of QNodes (sorted by content.HK)
*/
class List
{
public:
	List();

	//Add a new ListElement with content node, insertion sorted by content.HK
	//this only makes sense after node.content.HK has been set correctly
	void insert(QNode node);
	//Get and delete the first element from the list
	QNode pop();
	//Get length of the list
	unsigned int get_len();
private:
	ListElement* head;
	unsigned int len;
};

#endif
