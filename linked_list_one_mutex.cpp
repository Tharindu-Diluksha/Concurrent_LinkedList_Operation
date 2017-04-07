#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <cmath>
#include <algorithm>
#include <array>
#include <random>
#include <chrono>

using namespace std;

//----------------------------------------------class Timer----------------------------
class Timer {
private:
    clock_t begin;
    clock_t end;
public:
    Timer();
    ~Timer();
    void start();
    void stop();
    void print_time();
    float get_time();
};
Timer::Timer(){};
Timer::~Timer(){};
void Timer::start()
{
    begin = clock();
}
void Timer::stop()
{
    end = clock();
}
void Timer::print_time()
{
    end = clock();
    cout<< double(end - begin) / CLOCKS_PER_SEC<<"\n";
}
float Timer::get_time(){
	return clock();
}


//----------------------------------------------class Node----------------------------
struct Node {
	int data;
	Node* next;  
};


//-------------------------------------------class LinkedList------------

class LinkedList {
private:  
	Node* head; 
	int length;
    pthread_mutex_t mutex_ll;
public:
	LinkedList();
    ~LinkedList();
	Node* getHead();
    void setHead(Node* node);
    int* generate_unique_ran_list(int m,int range);
    void print();

	//Thread tested functions
    int Member(int value);
	void Delete(int value);
	void Insert(int value);
};
	
LinkedList::LinkedList()
{
    head= NULL;
    mutex_ll=PTHREAD_MUTEX_INITIALIZER;
}

LinkedList::~LinkedList()
{
    Node * CN=head;
    if(CN!=NULL)
    {
        while (CN->next != NULL) {
            Node *tmp = CN;
            CN = CN->next;
            delete tmp;
        }
    }
    delete CN;
}

Node* LinkedList::getHead()
{
    return head;
}

void LinkedList::setHead(Node* node)
{
    head=node;
}

//uses the mutex
void LinkedList::Insert(int value)
{
    pthread_mutex_lock(&mutex_ll);
    Node* newNode = new Node();
    newNode->data=value;
    newNode->next=NULL;
    Node* CN =getHead();
    newNode->next=getHead();
    setHead(newNode);
    pthread_mutex_unlock(&mutex_ll);
}


//uses the mutex
int LinkedList::Member(int value)
{
    pthread_mutex_lock(&mutex_ll);
    Node* CN=getHead();
    while(CN != NULL)
    {
        if(CN->data==value){
            int val=CN->data;
            pthread_mutex_unlock(&mutex_ll);
            return val;

        }
        else{
            CN= CN->next;
        }
    }
    pthread_mutex_unlock(&mutex_ll);
    return -1;
}

//uses the mutex
void LinkedList::Delete(int value)
{
    pthread_mutex_lock(&mutex_ll);
    Node* CN=getHead();
    if(CN==NULL)
    {
        pthread_mutex_unlock(&mutex_ll);
        return;
    }
    else{
        if(CN->data==value){
            Node* B= getHead();
            Node* A=CN->next;
            setHead(A);
            delete B;
            pthread_mutex_unlock(&mutex_ll);
            return;
        }
        else{
            while(CN->next != NULL){
                if((CN->next)->data==value)
                {
                    Node* A=CN->next;
                    CN->next=(CN->next)->next;
                    delete A;
                    pthread_mutex_unlock(&mutex_ll);
                    return ;
                }
                else{
                    CN= CN->next;
                }
            }
        }
    }
    pthread_mutex_unlock(&mutex_ll);
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


void LinkedList::print()
{
    pthread_mutex_lock(&mutex_ll);
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
    pthread_mutex_unlock(&mutex_ll);

}


//----------------------------------------------class Tester---------------------------

class Test
{
private:
    float mean;
    float s;
    int* initial_list;
    void read_init_file(int case_no,int m);
    void read_case_file(int case_no, int m);
    void create_initial_linklist(LinkedList* linked_list, int n);
    int** op_list;
	LinkedList* linked_list;
	long no_of_threads;
	int operations_per_thread;
    
public:
    Test();
    float run_single_test(int case_no,int tread_count,int m);
    void aggregate_results(int count,int case_no,int thread_count,int m);
    int* generate_unique_ran_list(int n,int range);
    void generate_and_populate_op_list(int case_no,int n,int m,float member_p,float insert_p,float delete_p);
    static void* do_operation(void* params);
    ~Test();
};

Test::Test() {};

Test::~Test() {};

struct Arg_struct {
	long id;
	LinkedList* linked_list;
	int** op_list;
	long nu_of_operations;	
};

void* Test::do_operation(void* params)
{	
	Arg_struct* args = (Arg_struct*)params;
	long my_id = args->id;
	int op_start_position = my_id*args->nu_of_operations;
	int op_end_position = (my_id+1)*args->nu_of_operations;
	
	for(int i=op_start_position;i<op_end_position;i++){
		if(args->op_list[i][1]==0){
			//memeber
			args->linked_list->Member(args->op_list[i][0]);
		}
		else if(args->op_list[i][1]==1){
			//insert
			args->linked_list->Insert(args->op_list[i][0]);
		}
		else{
			//delete
			args->linked_list->Delete(args->op_list[i][0]);
		}
	}
}

float Test::run_single_test(int case_no,int thread_count,int m)
{	 
	linked_list =  new LinkedList();
    read_init_file(case_no,m);
    read_case_file(case_no,m);
    create_initial_linklist(linked_list, 1000);
    
    operations_per_thread = m/thread_count;
    pthread_t* thread_handles;
    no_of_threads = (long)thread_count;
	thread_handles = static_cast<pthread_t*>(malloc (no_of_threads*sizeof(pthread_t*)));
	Timer* timer = new Timer();
	long thread;
	
	float start_time = timer->get_time();
	for(thread = 0; thread<no_of_threads; thread++){
		Arg_struct * params = new Arg_struct();
		params->id = thread;
		params->linked_list = linked_list;
		params->op_list = op_list; 
		params->nu_of_operations = (long)operations_per_thread;
		pthread_create(&thread_handles[thread],NULL,&Test::do_operation,(void*)params);
	}
	
	for(thread = 0; thread<no_of_threads; thread++){
		pthread_join(thread_handles[thread],NULL);
	}
	float end_time = timer->get_time(); 
	free(thread_handles);
	delete(linked_list);
	delete(timer);
	return end_time-start_time;
}


void Test::create_initial_linklist(LinkedList* linked_list, int n){
	for(int i=0;i<n;i++){
		linked_list->Insert(initial_list[i]);
	} 
}

void Test::read_init_file(int case_no,int m)
{
	initial_list = new int[m];
	std::fstream myfile("mutex_init_"+to_string(case_no), std::ios_base::in);
	if (myfile.is_open()){
		int line_no = 0;
		int number;
		while ( myfile >> number)
		{
		  initial_list[line_no]=number;
		  line_no+=1;
		  
		}
		myfile.close();
	}

	else cout << "Unable to open file" << '\n'; 
}

void Test::read_case_file(int case_no, int m)
{
	op_list = new int* [m];
	
	ifstream myfile ("mutex_case_"+to_string(case_no));
	if (myfile.is_open())
	{
		//int line_no=0;
		
		for (int line_no=0;line_no<m;line_no++)
		{
			op_list[line_no]= new int[2];
			myfile >> op_list[line_no][0] >> op_list[line_no][1];
		}
		myfile.close();
	}

	else cout << "Unable to open file" << '\n'; 
}

void Test::aggregate_results(int count,int thread_count,int case_no,int m)
{
    float* test_results=new float[count];
    mean=0;
    for (int i=0;i<count;i++)
    {
        float value=run_single_test(case_no,thread_count,m);
        mean+=value;
        test_results[i]=value;
    }
    mean=mean/count;
    s=0;
    for (int i=0;i<count;i++)
    {
        s+=pow(mean-test_results[i],2);
    }
    s=s/count;
    delete test_results;
    cout<<"case no: "<<case_no<<", thread count :"<<thread_count<<"\n";
    cout<<"mean :"<<mean<<"\n";
    cout << "s :"<<s<<"\n";
    cout << "thread_count :"<<thread_count<<"\n\n";
}

int* Test::generate_unique_ran_list(int n,int range)
{
    int* result=new int[n];
    int i=0;
    while(i<n)
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

void Test::generate_and_populate_op_list(int case_no,int n,int m,float member_p,float insert_p,float delete_p)
{

    ofstream case_commands;
    ofstream init_population;
    case_commands.open("mutex_case_"+to_string(case_no));
    init_population.open("mutex_init_"+to_string(case_no));
    int member_c= m*member_p;
    int insert_c= m*insert_p;
    int delete_c= m*delete_p;
    int * ran_list = generate_unique_ran_list(n+m,pow(2,16));

    //from 0 to n-1 : initiate the list
    for(int j=m;j<m+n;j++)
    {
        init_population<<ran_list[j]<<" ";
    }
    init_population.close();

    int command_map[m][2];
    /*
     *
     * 0=member ,1=insert,2=delete
     */

    //generate command array
    for(int i=0;i<m;i++)
    {
        if(i<member_c)
        {
            command_map[i][0]=ran_list[i];
            command_map[i][1]=0;
        }
        else if(member_c<=i && i<member_c+insert_c)
        {
            command_map[i][0]=ran_list[i];
            command_map[i][1]=1;
        }
        else
        {
            command_map[i][0]=ran_list[i-(member_c+insert_c)];
            command_map[i][1]=2;
        }

    }

    //shuffle command array
    random_shuffle(&command_map[0], &command_map[m]);

    //write commands to file
    for(int i=0;i<m;i++)
    {
        case_commands<<command_map[i][0]<<" "<<command_map[i][1]<<"\n";
    }
    case_commands.close();

}

//---------------------------------------------------main-------------------------------------
int sample_count;
int case_no;  
int main(int argc, char* argv[])
{	
	Test* test=new Test();
	if(argc !=3){
		cout << " Error in Input Arguments "<<'\n' <<"Incorrect number of arguments. Should be like ./mutex sample_count case_no" << '\n';
	}
	else {
		/* Implement terminal argivemnts to set caseNo and count(no of samples)*/
		sample_count=atoi(argv[1]);
		case_no=atoi(argv[2]);
		
		if(case_no==1)
		{
			//case 1
			test->generate_and_populate_op_list(1,1000, 10000, 0.99, 0.005, 0.005);
			for(int i=0;i<4;i++)
			{
				test->aggregate_results(sample_count,pow(2, i),case_no,10000);
			}
		}
		else if(case_no==2)
		{
			//case 2
			test->generate_and_populate_op_list(2,1000, 10000, 0.9, 0.05, 0.05);
			for(int i=0;i<4;i++)
			{
				test->aggregate_results(sample_count,pow(2, i),case_no,10000);
			}
		}
		else if(case_no==3)
		{
			//case 3
			test->generate_and_populate_op_list(3,1000, 10000, 0.5, 0.25, 0.25);
			for(int i=0;i<4;i++)
			{
				test->aggregate_results(sample_count,pow(2, i),case_no,10000);
			}
		}
		else{
			cout << "  Error in Input Arguments. Error in Third Argument. "<<'\n'<< "Incorrect Case Number. It should be either 1 or 2 or 3." << '\n';
		}	
	}
	delete test;
    return 0;
}
