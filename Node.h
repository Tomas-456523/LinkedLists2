/*
1.6.2026
node class header file
*/
#ifndef NODE
#define NODE
#include "Student.h"
#include <iostream>
class Node
{
private:
	Student* stud;//the student pointer --> set to Student*
	Node* next;//next node's pointer --> set to NULL
public:
	Node(Student*);//constructor --> input Student* when calling this 
	~Node();//destructor --> deletes stud (stud can only be associated with one node)
	void setNext(Node* in_n);//set next node
	Node* getNext();//return next node pointer
	Student* getStudent();//return student pointer
};

#endif //NODE