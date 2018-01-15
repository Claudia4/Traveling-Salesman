#include "queue.hpp"

#include <vector>
#include <utility>

QNode::QNode(std::vector<std::pair<unsigned int, unsigned int> > R, std::vector<std::pair<unsigned int, unsigned int> > F)
	:R ( R ),
	F ( F ),
	lambda  ( std::vector<double> () ),
	HK ( 0 )
	{}


ListElement::ListElement(QNode content, ListElement* successor)
	: content (content),
	successor (successor)
	{}

void ListElement::insert(QNode node)
{
	if(successor == NULL || node.HK < successor -> content.HK)
	{
		ListElement new_elem = ListElement(node, successor);
		successor = &new_elem;
	}
	else
	{
		successor -> insert(node);
	}
}


List::List()
	:head (NULL),
	len (0)
	{}

void List::insert(QNode node)
{
	if (len == 0 )
	{
		ListElement new_elem = ListElement(node, NULL);
		head = & new_elem;
		len = 1;
	}
	else
	{
		if(head -> content.HK > node.HK)
		{
			ListElement new_elem = ListElement(node, head);
			head = & new_elem;
		}
		else
		{
			head -> insert(node);
		}
		len++;
	}
}


QNode List::pop()
{
	if(len == 0)
	{
		throw std::logic_error("pop failed because list is empty");
	}
	QNode r = head -> content;
	head = head -> successor;
	len--;
	return r;
}

unsigned int List::get_len()
{
	return len;
}
