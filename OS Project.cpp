#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<windows.h>
#include<unistd.h>

using namespace std;

struct DataProcessing
{
	int num,pid,arrivalTime;
	int burstTime,py, finishTime, remainingTime;
	int waitingTime, startTime, responseTime;
	int turnaroundTime;

}cur;

typedef struct DataProcessing p ;

struct comparingOperator // structure has been made to sort on the basis of py
{
	bool operator()(const p& a ,const p& b)
	{
		if( a.py < b.py )
			return false;
		
		else if(a.py > b.py )
			return true;
			
		if( a.pid > b.pid ) // if py is same, then comparing on the basis of pid
			return true;

		return false;
	}
};

bool processIdSort(const p& a , const p& b)
{
	return a.pid < b.pid;
}

// method to sort the arrival time, if the arrival time will be same, then sorting will be done on the basis of pid

bool arrivalTimeSort( const p& a ,const p& b)
{
	if(a.arrivalTime > b.arrivalTime)
		return false;
	
	else if(a.arrivalTime < b.arrivalTime)
		return true;
	
	if(a.py > b.py)
		return false;

	else if(a.py < b.py)
		return true;

	if(a.pid < b.pid)
		return true;

	return false;
}

bool SortingNum( const p& a ,const p& b)
{
	return a.num < b.num;
}

void checker(vector<p> cc)
{
	for(int i= 0; i < cc.size() ;i++)
	{
		cout<<" DataProcessing Id :"<<cc[i].pid<<" Arrival Time : "<<cc[i].arrivalTime<<" Burst Time : "<<cc[i].burstTime<<" Priority : "<<cc[i].py<<"\n";
	}
}

int main()
{
	p t;
	int process1 = 0, process2 = 0; 
	int arrivalTime, burstTime, pid;
	int py,n,clk;
	int tet = 0;
	
	cout<<"\n\n\t\tScheduling Algorithm to Implement Queue with Two Levels\n\n";
	cout<<"\t\tLevel 1 : Fixed priority preemptive Scheduling\n\n\t\tLevel 2 : Round Robin Scheduling\n\n";
	
	cout<<"\t\tLOADING ";
	for(int i=0;i<5;i++)
	{
		cout<<".";
		sleep(1.5);
	}
	
	system("cls");
	
	cout<<"Enter the no of processes : ";
	cin>>n;
	
	vector<p> process_1_vector;
	vector<p> process_2_vector;
	
	for(int i= 0; i< n; i++ )
	{
		cout<<"\n\nEnter the pid : ";
		cin>>pid;
		
		cout<<"\nEnter the arrival time of pid "<<pid<<" : ";
		cin>>arrivalTime;
		
		cout<<"\nEnter the burst time of pid "<<pid<<" : ";
		cin>>burstTime;
		
		cout<<"\nEnter the priority of pid "<<pid<<" : ";
		cin>>py;
		
		t.num = i+1;
		t.arrivalTime = arrivalTime;
		t.burstTime = burstTime;
		t.remainingTime = burstTime;
		t.pid = pid;
		t.py = py;
		process_1_vector.push_back(t);
		
		cout<<"\n\t----------------------------------------------";
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

	priority_queue <p ,vector<DataProcessing> ,comparingOperator> prioqueue; // queue has been made for the py premption

	queue<p> round_robin_queue; // for level 2 queue of round robin
	
	int cpuState = 0; // initially CPU is idle, so value is 0
	int time_Quantum = 2 ; //Time time_Quantum should be multiple of 2
	cur.pid = -2;
	cur.py = 100;

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
				time_Quantum = 2; 
			}
			else if(!round_robin_queue.empty())
			{
				cur = round_robin_queue.front();
				cpuState = 1;
				process2 = 1;
				round_robin_queue.pop();
				time_Quantum = 2;
			}
		}
		else if(cpuState == 1)
		{
			if(process1 == 1 && (!prioqueue.empty()))
			{
				if(prioqueue.top().py < cur.py ) 
				{
					round_robin_queue.push(cur); 
					cur = prioqueue.top();
					prioqueue.pop();
					time_Quantum = 2; 
				}
			}
			else if(process2 == 1 && (!prioqueue.empty()))
			{
				round_robin_queue.push(cur);
				cur = prioqueue.top();
				prioqueue.pop();
				process2 = 0;
				process1 = 1;
				time_Quantum = 2 ;
			}
		}

		if(cur.pid != -2) 
		{
			cur.remainingTime--;
			time_Quantum--;
			
			ghantChart[clk] = cur.pid;
			
			if(cur.remainingTime == 0) 
			{
				cpuState = 0 ;
				time_Quantum = 2 ;
				cur.pid = -2;
				cur.py = 999999;
				process2 = 0;
				process1 = 0;
			}
			else if(time_Quantum == 0 ) 
			{
				round_robin_queue.push(cur);
				cur.pid = -2;
				cur.py = 999999;
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
	
	sort( process_1_vector.begin(), process_1_vector.end(), SortingNum );

	for(int i=0;i<n;i++)
	{
		process_1_vector[i].responseTime=process_1_vector[i].startTime - process_1_vector[i].arrivalTime;
		process_1_vector[i].waitingTime=(process_1_vector[i].finishTime-process_1_vector[i].arrivalTime)-process_1_vector[i].burstTime;
		process_1_vector[i].turnaroundTime = process_1_vector[i].finishTime - process_1_vector[i].arrivalTime;
	}
	
	system("cls");
	
	cout<<"Processing Your data ";
	
	for(int i=0;i<5;i++)
	{
	    cout<<".";
	    sleep(2);
	}
	
	cout<<"\n\nPid |\tResponse Time |\tFinish Time |\t Turnaround Time | Waiting Time\n";
	
	int ans1 = 0;
	int ans2 = 0;
	
	for(int i=0;i<n;i++)
	{
		cout<<" "<<process_1_vector[i].pid<<"\t\t"<<process_1_vector[i].responseTime<<"\t\t"<<process_1_vector[i].finishTime
		cout<<"\t\t"<<process_1_vector[i].turnaroundTime<<"\t\t"<<process_1_vector[i].waitingTime<<endl;
		
		ans1 = ans1 + process_1_vector[i].waitingTime;
		ans2 = ans2 + process_1_vector[i].turnaroundTime;
	}
	
	cout<<"\n\n System doing calculations, please wait ";
	for(int i=0;i<5;i++)
	{
		cout<<".";
		sleep(1);
	}
	
	cout<<"\n\nAverage Turnaround Time : "<<(float)ans2/n<<"s";
	cout<<"\nAverage Waiting Time : "<<(float)ans1/n<<"s";
		
	return 0;
}
