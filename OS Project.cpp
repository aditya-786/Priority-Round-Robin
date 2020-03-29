#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<windows.h>
#include<unistd.h>

using namespace std;

struct Process
{
	int num,pid,arrivalTime;
	int burstTime,priority, finishTime, remainingTime;
	int waitingTime, startTime, responseTime;

}cur;

struct comparingOperator // structure has been made to sort on the basis of priority
{
	bool operator()(const p& a ,const p& b)
	{
		if(a.priority > b.priority )
			return true;
		else if( a.priority < b.priority )
			return false;
		if( a.pid > b.pid ) // if priority is same, then comparing on the basis of pid
			return true;

		return false;
	}
};

typedef struct Process p ;

bool processIdSort(const p& a , const p& b)
{
	return a.pid < b.pid;
}

/** Sorting on the base of arrival time if that match then on Priority of Priority also  match than on the base of Process Id**/

bool arrivalTimeSort( const p& a ,const p& b)
{
	if(a.arrivalTime < b.arrivalTime)
		return true;
	else if(a.arrivalTime > b.arrivalTime)
		return false;
	if(a.priority < b.priority)
		return true;
	else if(a.priority > b.priority)
		return false;
	if(a.pid < b.pid)
		return true;

	return false;
}

bool Numsort( const p& a ,const p& b)
{
	return a.num < b.num;
}


/**To check the Input **/
void my_check(vector<p> mv)
{
	for(unsigned int i= 0; i < mv.size() ;i++)
	{
		cout<<" Process Id :"<<mv[i].pid<<" Arrival Time : "<<mv[i].arrivalTime<<" Burst Time : "<<mv[i].burstTime<<" Priority : "<<mv[i].priority<<endl;
	}
}

int main()
{
	p t;
	int process1 = 0, process2 = 0; 
	int arrivalTime, burstTime, pid;
	int priority,n,clk;
	int tet = 0;
	
	cout<<"Enter the no of processes : ";
	cin>>n;
	
	vector<p> process_1_vector;
	vector<p> process_2_vector;
	
	for(int i= 0; i< n; i++ )
	{
		cout<<"Enter the pid : ";
		cin>>pid;
		
		cout<<"Enter the arrival time of pid "<<pid<<" : ";
		cin>>arrivalTime;
		
		cout<<"Enter the burst time of pid "<<pid<<" : ";
		cin>>burstTime;
		
		cout<<"Enter the priority of pid "<<pid<<" : ";
		cin>>priority;
		
		t.num = i+1;
		t.arrivalTime = arrivalTime;
		t.burstTime = burstTime;
		t.remainingTime = burstTime;
		t.pid = pid;
		t.priority = priority;
		process_1_vector.push_back(t);
	}
	
	process_2_vector = process_1_vector;
	
	sort( process_1_vector.begin(), process_1_vector.end(), arrivalTimeSort );
    
    tet = tet + process_1_vector[0].arrivalTime;
    
	for(int i= 0 ;i< n; i++ )
    {
    	if(tet >= process_1_vector[i].arrivalTime )
    	{
    		tet = tet + process_1_vector[i].burstTime;
    	}
    	else
    	{
    		int ans = (process_1_vector[i].arrivalTime - tet);
    		tet = tet + ans + burstTime;
    	}
    }

	int ghantChart[tet]={0}; // ghant chart for process queue
	
	for(int i= 0; i<tet;i++)
	{
		ghantChart[i]=-1;
	}

	priority_queue <p ,vector<Process> ,comparingOperator> prioqueue; // queue has been made for the priority premption

	queue<p> round_robin_queue; // for level 2 queue of round robin
	
	int cpuState = 0; // initially CPU is idle, so value is 0
	int quantum = 2 ; //Time quantum should be multiple of 2
	cur.pid = -2;
	cur.priority = 999999;

	for ( clk = 0; clk<tet; clk++ )
	{
		for( int j = 0; j< n ; j++ )
		{
			if(clk == process_1_vector[j].arrivalTime)
			{
				prioqueue.push(process_1_vector[j]);
			}
		}
		

		if(cpuState == 0)
		{
			if(!prioqueue.empty())
			{
				cur = prioqueue.top();
				cpuState = 1;
				process1 = 1;
				prioqueue.pop();
				quantum = 2; 
			}
			else if(!round_robin_queue.empty())
			{
				cur = round_robin_queue.front();
				cpuState = 1;
				process2 = 1;
				round_robin_queue.pop();
				quantum = 2;
			}
		}
		else if(cpuState == 1)
		{
			if(process1 == 1 && (!prioqueue.empty()))
			{
				if(prioqueue.top().priority < cur.priority ) 
				{
					round_robin_queue.push(cur); 
					cur = prioqueue.top();
					prioqueue.pop();
					quantum = 2; 
				}
			}
			else if(process2 == 1 && (!prioqueue.empty()))
			{
				round_robin_queue.push(cur);
				cur = prioqueue.top();
				prioqueue.pop();
				process2 = 0;
				process1 = 1;
				quantum = 2 ;
			}
		}

		if(cur.pid != -2) 
		{
			cur.remainingTime--;
			quantum--;
			
			ghantChart[clk] = cur.pid;
			
			if(cur.remainingTime == 0) 
			{
				cpuState = 0 ;
				quantum = 2 ;
				cur.pid = -2;
				cur.priority = 999999;
				process2 = 0;
				process1 = 0;
			}
			else if(quantum == 0 ) 
			{
				round_robin_queue.push(cur);
				cur.pid = -2;
				cur.priority = 999999;
				process2 = 0;
				process1 = 0;
				cpuState = 0;
			}
		}
	}

	sort( process_1_vector.begin(), process_1_vector.end(), processIdSort );
	
	for(int i=0;i<n;i++)
	{
		for(int k=tet;k>=0;k--)
		{
			if(ghantChart[k]==i+1)
			{
				process_1_vector[i].finishTime=k+1;
				break;
			}
		}
	}
	
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<tet;k++)
		{

			if(ghantChart[k]==i+1)
			{
				process_1_vector[i].startTime = k;
				break;
			}
		}
	}
	
	sort( process_1_vector.begin(), process_1_vector.end(), Numsort );

	for(int i=0;i<n;i++)
	{
		process_1_vector[i].responseTime=process_1_vector[i].startTime - process_1_vector[i].arrivalTime;
		process_1_vector[i].waitingTime=(process_1_vector[i].finishTime-process_1_vector[i].arrivalTime)-process_1_vector[i].burstTime;
	}
	
	system("cls");
	
	cout<<"Processing Your data ";
	
	for(int i=0;i<5;i++)
	{
	    cout<<".";
	    sleep(2);
	}
	
	cout<<"\n\nPid |\tResponse Time |\tFinish Time |\tWaiting Time\n";
	
	for(int i=0;i<n;i++)
	{
		cout<<" "<<process_1_vector[i].pid<<"\t\t"<<process_1_vector[i].responseTime<<"\t\t"<<process_1_vector[i].finishTime<<"\t\t"<<process_1_vector[i].waitingTime<<endl;
	}	
	return 0;
}
