#include "queue.hpp"

#include <vector>
#include <utility>

QNode::QNode(std::vector<std::pair<unsigned int, unsigned int> > R, std::vector<std::pair<unsigned int, unsigned int> > F, std::vector<double> L, unsigned int size)
	:R ( R ),
	F ( F ),
	lambda  ( L ),
	one_tree(std::vector<std::pair<unsigned int, unsigned int>> (size)),
	HK ( 0 )
	{}
QNode::QNode() {}


//can be deleted - see note in queue.hpp
 
/*
ListElement::ListElement(QNode & content)
	: content (content),
	successor (std::numeric_limits<unsigned int>::max())
	{}
	
	
ListElement::ListElement()
{ successor=std::numeric_limits<unsigned int>::max();
}
	
void ListElement::set_content(QNode & Node) {
	content=Node;
}
void ListElement::insert(std::vector<ListElement> &S, unsigned int position)
{
	if(successor == std::numeric_limits<unsigned int>::max())
	{
		S.at(position).successor=std::numeric_limits<unsigned int>::max();
		successor = position;
	}
	else if(S.at(position).content.HK > S.at(successor).content.HK) {
		S.at(position).successor=std::numeric_limits<unsigned int>::max();
		successor = position;
	}
	else
	{
		S.at(successor).insert(S,position);
	}
}


List::List()
	:head (std::numeric_limits<unsigned int>::max()),
	len (0)
	{}

void List::insert(std::vector<ListElement> &S, unsigned int position)
{
	if (len == 0 )
	{
		S.at(position).successor=std::numeric_limits<unsigned int>::max();
		head = position;
		len = 1;
	}
	else
	{
		if(S.at(head).content.HK < S.at(position).content.HK)
		{
			S.at(position).successor= head;
			head = position;
		}
		else
		{	
			S.at(head).insert(S,position);
		}
		len++;
	}
}


unsigned int List::pop(std::vector<ListElement> &S)
{
	if(len == 0)
	{
		throw std::logic_error("pop failed because list is empty");
	}
	unsigned int r = head;
	head = S.at(head).successor;
	len--;
	return r;
}

unsigned int List::get_len()
{
	return len;
}
*/
