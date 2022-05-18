#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
HANDLE semaphore_r;
HANDLE semaphore_m;
HANDLE readtry;
DWORD Thread1ID, Thread2ID, Thread3ID;
clock_t execution_time;

int r=0;
int shared_variable=0;


DWORD WINAPI LookUp1(LPVOID param)
{
	int lookUpItem1;


	while (1)
	{
		WaitForSingleObject(readtry,INFINITE); //writer precedence semaphore
    
    	WaitForSingleObject(semaphore_r,INFINITE); //binary semaphore for the variable r
    
    	++r;
    	if(r==1) //if r==1 then this is the first process so it needs to prevent the writer process from working now
    	{
    		 WaitForSingleObject(semaphore_m,INFINITE);
    		 printf("writer process blocked\n");

    	}
    ReleaseSemaphore(semaphore_r,1,NULL); //done with modifying r
   ReleaseSemaphore(readtry,1,NULL);


    	lookUpItem1=shared_variable; //read shared item
    	execution_time = clock();
    	printf("look up process 1 id=%d and execution time is %d\n ", Thread2ID, execution_time);
    
    	printf("looked up item in process 1= %d\n", lookUpItem1);

    	//modifying r again


    	WaitForSingleObject(semaphore_r,INFINITE); //binary semaphore for the variable r
    	--r;
    	if(r==0) //if r==1 then all processes have finished reading and now the writer can modify it again
    	{
    		ReleaseSemaphore(semaphore_m,1,NULL);
    		 printf("writer unblocked\n");

    	}
    ReleaseSemaphore(semaphore_r,1,NULL); //done with modifying r
	}

}

DWORD WINAPI LookUp2(LPVOID param)
{
	int lookUpItem2;
	printf ("look up process 2  \n");

	while (1)
	{
    	WaitForSingleObject(readtry,INFINITE); //writer precedence semaphore
    	WaitForSingleObject(semaphore_r,INFINITE); //binary semaphore for the variable r
    	++r;
    	if(r==1) //if r==1 then this is the first process so it needs to prevent the writer process from working now
    	{
    		 WaitForSingleObject(semaphore_m,INFINITE);
    		 printf("writer process blocked\n");

    	}
    ReleaseSemaphore(semaphore_r,1,NULL); //done with modifying r
     ReleaseSemaphore(readtry,1,NULL);


    	lookUpItem2=shared_variable; //read shared item
    	printf("look up process 2 id=%d and execution time is %d\n", Thread3ID, execution_time);
    	printf("looked up item in process 2= %d\n", lookUpItem2);

    	//modifying r again


    	WaitForSingleObject(semaphore_r,INFINITE); //binary semaphore for the variable r
    	--r;
    	if(r==0) //if r==1 then all processes have finished reading and now the writer can modify it again
    	{
    		ReleaseSemaphore(semaphore_m,1,NULL);
    		 printf("writer unblocked\n");

    	}
    ReleaseSemaphore(semaphore_r,1,NULL); //done with modifying r
	}

}


//first we will create two threads
DWORD WINAPI Writer(LPVOID param)
{
		while (1)
    	{
			printf ("writer process\n");
			WaitForSingleObject(readtry,INFINITE); //writer precedence semaphore

    	 WaitForSingleObject(semaphore_m,INFINITE); //writer process is working so no other processes can work
    	 
    	 printf("other processes blocked\n");
    	 shared_variable++;
		printf("writer process id=%d and execution time is %d\n", Thread1ID, execution_time);
    	 printf("shared variable is modified= %d\n", shared_variable);
    	 
		ReleaseSemaphore(semaphore_m,1,NULL); 
		ReleaseSemaphore(readtry,1,NULL);
			
    	  
		  	printf("writer process done\n");
		  	
		  
    }
    	 
    	

}

int main()
{
         
         HANDLE Thread1, Thread2, Thread3;
         char sem_r, sem_m, read_try;

         //creating the threads

        Thread1= CreateThread(NULL,0,Writer, NULL, CREATE_SUSPENDED, &Thread1ID);

      	Thread2= CreateThread(NULL,0,LookUp1,NULL, CREATE_SUSPENDED, &Thread2ID);

         Thread3= CreateThread(NULL,0,LookUp2, NULL, CREATE_SUSPENDED, &Thread3ID);
         
         

        
         //creating semaphores
         semaphore_r = CreateSemaphore(NULL,1,1, &sem_r);
         semaphore_m = CreateSemaphore(NULL,1,1,&sem_m);
         readtry = CreateSemaphore(NULL,1,1,&read_try);

        
           while(getche() != '1');
            
           
           
            ResumeThread(Thread2);
          
         
        
            
       while(getche() != '2');
       ResumeThread(Thread1);
      
		ResumeThread(Thread3);
       
          
      
        
         	
         	while(getche()!= 'e');
		 	while(getche()!= 'e');
        
    
         
}


