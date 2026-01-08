/*
12.18.2025
*/
#include "Node.h"
#include "Student.h"
#include <iostream>
Node::Node(Student* s)
{
	stud = s;
	next = NULL;
}
Node::~Node()
{
	//std::cout<< "a";
	delete stud;
	//std::cout << "b";
}
void Node::setNext(Node* in_n)
{
	next = in_n;
}
Node* Node::getNext()
{
	return next;
}
Student* Node::getStudent()
{
	return stud;
}