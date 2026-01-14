/* Tomas Carranza Echaniz
*  12/19/2025
*  This program is a student database that uses linked lists. It is for showcasing the functionality of the
*  Node.h and Node.cpp files. You can ADD a new student, which will be added after the current node. You can DELETE
*  the student, which will move you to the next student. You can go to the NEXT student, and PRINT the student's data.
*  You can also PRINT ALL the students at once, or print the AVERAGE of all their GPAs. You can also ask for HELP,
*  which will print all the valid commands, or QUIT the program.
*/

#include <iostream>
#include <limits>
#include <cstring>
#include <iomanip>
#include "Student.h"
#include "Node.h"
using namespace std;

//for ignoring faulty input and extra characters, functionality taken from my previous projects
void CinIgnoreAll(bool force = false) {
    if (!cin || force) { //only go if input is faulty, otherwise forces the user to input an extra enter
        cin.clear(); //clears error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //ignores all characters up until a new line
    }
}

//capitalizes the given charray for easier command interpretation
void AllCaps(char* word) {
    for (int i = 0; i < strlen(word); i++) { //sets all the characters to a capitalized unsigned version of the char (unsigned because some systems sign the chars)
        word[i] = toupper((unsigned char)word[i]);
    }
}

//for getting the id of a student since that happens twice
int getID() {
    int id = 0;
    while (true) {
        cout << "\n> ";
        cin >> id; //gets the id integer
        CinIgnoreAll(true); //removes the newline character or invalid input
        if (cin) { //return the id and end the loop if input was valid
            return id;
        } else { //otherwise give error message and try again
            cout << "\nID must be an integer.";
        }
    }
}

//creates a new student and a new node pointing to it
Node* createStudent() {
    char firstname[255];
    char lastname[255];
    int id;
    float gpa;

    //get the student's first (and middle if applicable) names
    cout << "\nEnter first (and middle) name for new student.";
    bool continuing = true; //continues until valid input is given
    while (continuing) {
        cout << "\n> ";
        cin.getline(firstname, 255);
        if (cin) { //end loop if input was valid
            continuing = false;
        }
        else { //error message
            cout << "\nInput too long.";
        }
        CinIgnoreAll(); //clear any faulty or extra input given
    }

    ///get the student's last name(s)
    cout << "\nEnter last name(s) for new student.";
    continuing = true; //continues until valid input is given
    while (continuing) {
        cout << "\n> ";
        cin.getline(lastname, 255);
        if (cin) { //end loop if input was valid
            continuing = false;
        }
        else { //error message
            cout << "\nInput too long.";
        }
        CinIgnoreAll(); //clear any faulty or extra input given
    }

    //get the student's id
    cout << "\nEnter " << firstname << "'s student ID.";
    id = getID();

    cout << "\nEnter " << firstname << "'s GPA.";
    continuing = true; //continues until valid input is given
    while (continuing) {
        cout << "\n> ";
        cin >> gpa; //gets the gpa float
        if (cin) { //end loop if valid input was given
            continuing = false;
        }
        else { //error message otherwise
            cout << "\nGPA must be a float.";
        }
        CinIgnoreAll(true); //removes the newline character of invalid input
    }

    //creates a new student using the given data
    Student* student = new Student(&firstname[0], &lastname[0], id, gpa);
    //create and return a new node pointing to the new student
    return new Node(student);
}

//creates a new student node and adds it into the linked list according to id order
void addNode(Node*& current, Node* newguy) {
    if (newguy == NULL) {
        newguy = createStudent();
    }
    if (current == NULL) {
        current = newguy;
    } else if (current->getNext()->getStudent()->getID() > newguy->getStudent()->getID()) {
        newguy->setNext(current->getNext());
        current->setNext(newguy);
    }
    
    cout << "\nSuccessfully added " << newguy->getStudent()->getName(0); //success message
    if (current->getNext() != current) { //if it isn't the only node, we print which node we put it after (current)
        cout << " after " << current->getStudent()->getName(0);
    }
    cout << "!"; //exclamation mark!
}

//prints the given student (with option to put the data in a new line)
void printStudent(Student* student, bool newline = true) {
    if (newline) { //prints the new line if we need to
        cout << "\n";
    } //prints all the student's data
    cout << student->getName(0) << " " << student->getName(1) << " (" << student->getID() << ") - GPA of " << student->getGPA();
}

//deletes the current node, and returns the node after the given one in order to bridge the gap created as a result (we need to pass by reference so we can nullify it from here)
Node* deleteNode(Node*& current, int id = 0, bool first = 0) {
    if (current == NULL) { //return if we ran out of students
        if (first) { //if it's the first one that means there is actually literally nobody to delete
            cout << "\nThere are no students to delete. (type ADD for add)";
        } else { //if it's not the first check, this is after the last node and there were no matching students along the way
            cout << "\nThere are no students with ID " << id << ".";
        }
        return NULL;
    }
    if (first) { //get the id if it's the first student so we know who to delete
        id = getID();
    }
    
    //if the current student node has the needed id, we found something to delete!
    if (current->getStudent()->getID() == id) {
        cout << "\nSuccessfully deleted " << current->getStudent()->getName(0) << "!"; //success message
        delete current; //does the deleting
        if (first) { //nullify current node if it's the first one so int main is updated since it's present there as well
            current = NULL;
            cout << "\nThere are no students left."; //prints status message
        }
        return current->getNext();
    } else {
        Node* next = current->getNext(); //I can't just put this all in one line, I have to define this and then pass it into the next call, because getNext() returns a Node*, but I need to pass a Node*&, but I can't just pass a pointer I got from a function for that. "cannot bind non-const lvalue reference of type ‘Node*&’ to an rvalue of type ‘Node*’"
        current->setNext(deleteNode(next, id)); //sets next to whatever the next recursive call returns, usually the next node again
    }

    return current;
}

//prints the average gpa of all the students
void average(Node* current, float sum = 0, int count = 0) {
    if (current == NULL) { //return if there are no students left
        if (!count) { //print error if the first node is null, meaning there are no students with GPAs to average
            cout << "\nThere are no students with GPAs to average. (type ADD for add)";
        } else { //print the sum over the count (average formula) with up to two decimal points of precision
            cout << "\nAverage GPA: " << fixed << setprecision(2) << sum / count;
        }
        return;
    } //gets the node's student for easier variable getting
    Student* student = current->getStudent();
    average(current->getNext(), sum+student->getGPA(), count+1);
}

//print all the students' data
void printAll(Node* current, bool first = false) {
    if (current == NULL) { //return if we ran out of students to print
        if (!first) { //print error message if there are no students to print
            cout << "\nThere are no students to print. (type ADD for add)";
        }
        return;
    } else if (first) {
        cout << "\nStudents:";
    } //print the current student and continue onto the next student
    printStudent(current->getStudent());
    printAll(current->getNext());
}

//the main player loop
int main() {
    Node* first = NULL; //the start of the linked list
    
    //welcome message with instructions
    cout << "\nWelcome to LINKED LISTS PART 2!\nYou are making a database of students using a linked list.\nType HELP for help.\n\nThere are currently no students. (type ADD for add)";
    
    //continues until continuing is falsified (by typing QUIT)
    bool continuing = true;
    while (continuing) {
        char command[255]; //the command that the user inputs into
        
        cout << "\n> "; //thing for the player to type after
        
        cin.getline(command, 255); //gets the player input, up to 255 characters
        
        AllCaps(&command[0]); //capitalizes the command for easier interpretation
        
        //calls function corresponding to the given command word
        if (!strcmp(command, "ADD")) { //add student
            addNode(first);
        } else if (!strcmp(command, "DELETE")) { //delete current student
            deleteNode(first, 0, true);
        } else if (!strcmp(command, "PRINT")) { //print all students
            printAll(first, true);
        } else if (!strcmp(command, "AVERAGE")) { //print average gpa of all students
            average(first);
        } else if (!strcmp(command, "HELP")) { //print all valid command words
            cout << "\nYour command words are:\nADD\nDELETE\nNEXT\nPRINT\nAVERAGE\nHELP\nQUIT";
        } else if (!strcmp(command, "QUIT")) { //quit the program
            continuing = false; //leave the main player loop
        } else { //give error message if the user typed something unacceptable
            cout << "\nInvalid command \"" << command << "\". (type HELP for help)";
        }
        
        CinIgnoreAll(); //ignore any invalid or extra input that may have been typed this time
    }

    //says bye to the user
    cout <<"\nIn case I don't see ya, good afternoon, good evening, and good night!.\n";

    //delete all the nodes
}