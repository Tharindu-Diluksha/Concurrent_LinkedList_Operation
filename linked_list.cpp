#include <iostream>
#include <stdlib.h>

using namespace std;

//----------------------------------------------class Node----------------------------
struct Node {
	int data;
	Node* next;  
};


//-------------------------------------------class LinkedList------------//sorted property preserved

class LinkedList {
private:  
	Node* head; 
	int length;  

public:
	LinkedList();
	Node* getHead();
    void setHead(Node* node);
    bool is_list_empty();
    int* generate_unique_ran_list(int m,int range);
    int list_length();
    void print();

	//Thread tested functions
    int Member(int value);
	void Delete(int value);
	bool Insert(int value,int i);



};
	
LinkedList::LinkedList()
{
    head= NULL;
}

Node* LinkedList::getHead()
{
    return head;
}

void LinkedList::setHead(Node* node)
{
    head=node;
}

bool LinkedList::Insert(int value, int i)
{
    Node* newNode = new Node();
    newNode->data=value;
    newNode->next=NULL;
    Node* CN =getHead();
    int position;
    position=0;
    if (CN==NULL){
        if(i==1){
            newNode->next=getHead();
            setHead(newNode);
            return true;
        }
        else{
            return false;
        }
    }
    else if (i==1){
        newNode->next=getHead();
        setHead(newNode);
        return true;
    }
    else{
        while(CN != NULL){
            if(position==i-2){
                newNode->next=CN->next;
                CN->next=newNode;
                return true;
            }
            else{
                CN=CN->next;
                position=position+1;
            }
        }
        return false;
    }
}


int LinkedList::Member(int value)
{
    Node* CN=getHead();
    while(CN != NULL){
        if(CN->data==value){
            return CN->data;
        }
        else{
            CN= CN->next;
        }
    }
    return -1;
}




void LinkedList::Delete(int value)
{
    Node* CN=getHead();
    if(CN==NULL){
        return;
    }
    else{
        if(CN->data==value){
            Node* B= getHead();
            Node* A=CN->next;
            setHead(A);
            delete B;
            return;
        }
        else{
            while(CN->next != NULL){
                if((CN->next)->data==value)
                {
                    Node* A=CN->next;
                    CN->next=(CN->next)->next;
                    delete A;
                    return ;
                }
                else{
                    CN= CN->next;
                }
            }
        }
    }
    return;
}

int* generate_unique_ran_list(int m,int range)
{
    int* result=new int[m];
    int i=0;
    while(i<m)
    {
        int r=rand()%(range);
        for(int j=0;j<i;j++)
        {
            if(result[j]==r)
            {
                continue;
            }
        }
        result[i] = r;
        i++;
    }
    return result;
}

bool LinkedList::is_list_empty()
{
    return head==NULL;
}

int LinkedList::list_length()
{
    int count;
    count=0;
    Node* CN=getHead();
    if(CN==NULL){
        count=0;
    }
    else{
        while(CN != NULL){
            count+=1;
            CN=CN->next;
        }
    }
    return count;
}


void LinkedList::print()
{
    Node* CN=getHead();
    if(CN==NULL){
        cout<<"Empty"<<"\n";
    }
    else{
        while(CN != NULL)
        {
            cout<<CN->data<<"  ";
            CN=CN->next;
        }
        cout<<"\n";
    }
}


//---------------------------------------------------main-------------------------------------

int main()
{
	
	LinkedList LL;

    LL.Insert(5,1);
	LL.Insert(10,1);
	LL.Insert(15,1);
	LL.Insert(87,1);
	LL.print();
	LL.Insert(7,3);
	LL.print();
	LL.Delete(2);
	LL.print();
	LL.Insert(13,2);
	LL.print();
	LL.Insert(11,1);
	LL.print();
	LL.Delete(13);
	LL.print();
	LL.Delete(11);
	LL.print();
	cout<<LL.list_length()<<"\n";
	LL.print();
	cout<<LL.Member(87)<<"\n";
	LL.Delete(10);
	LL.print();
	cout<<LL.Member(50)<<"\n";
	cout<<LL.Member(15)<<"\n";
	LL.Delete(15);
	cout<<LL.Member(15)<<"\n";

	return 0;
}
