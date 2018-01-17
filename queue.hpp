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
	//required edges
	std::vector<std::pair<unsigned int, unsigned int> > R;
	//forbidden edges
	std::vector<std::pair<unsigned int, unsigned int> > F;
	//initial lambda vector
	std::vector<double> lambda;
	//one_tree
	std::vector<std::pair<unsigned int,unsigned int>> one_tree;
	//Held-Karp-Bound
	double HK;
	QNode(std::vector<std::pair<unsigned int, unsigned int>> R, std::vector<std::pair<unsigned int, unsigned int>> F, std::vector<double> L, unsigned int size);
	QNode();
};
//can be deleted
/*
List element for the class list (singly linked list)
to store a priority queue of QNodes

class ListElement
{
public:
	QNode content;
	unsigned int successor;
	
	ListElement(QNode & content);
	ListElement();
	//Recursive insertion of list elements sorted by content.HK
	void insert(std::vector<ListElement> &S, unsigned int position);
	void set_content(QNode & Node);
};


List class for singly linked list using ListElements
to store a priority queue of QNodes (sorted by content.HK)

class List
{
public:
	List();

	//Add a new ListElement with content node, insertion sorted by content.HK
	//this only makes sense after node.content.HK has been set correctly
	void insert(std::vector<ListElement> &S, unsigned int position);
	//Get and delete the first element from the list
	unsigned int pop(std::vector<ListElement> &S);
	//Get length of the list
	unsigned int get_len();
private:
	unsigned int head;
	unsigned int len;
};
*/
#endif
