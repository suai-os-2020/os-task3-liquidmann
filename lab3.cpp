#include "lab3.h"
#include <windows.h>
#include <stdio.h>
#include <map>
#include <string>

#define MAX_SEM_COUNT 10
#define THREADCOUNT 13

//
// lab3 code should be located here!
//


HANDLE stdout_mutex;
HANDLE app_mutex;

HANDLE semA, semB, semC, semG, semK, semI, semF, semM, semP;
HANDLE semD1, semD2, semD3, semH1, semH2, semN1, semE1, semE2;

HANDLE semSync1, semSync2, semSync3;

std::vector<HANDLE> threads;

unsigned int lab3_thread_graph_id()
{
	return 15;
}

const char* lab3_unsynchronized_threads()
{
	return "bcdgh";
}

const char* lab3_sequential_threads()
{
	return "emn";
}

//Вариант 15


DWORD WINAPI thread_a(LPVOID);
DWORD WINAPI thread_b(LPVOID);
DWORD WINAPI thread_c(LPVOID);
DWORD WINAPI thread_d(LPVOID);
DWORD WINAPI thread_e(LPVOID);
DWORD WINAPI thread_f(LPVOID);
DWORD WINAPI thread_g(LPVOID);
DWORD WINAPI thread_k(LPVOID);
DWORD WINAPI thread_i(LPVOID);
DWORD WINAPI thread_h(LPVOID);
DWORD WINAPI thread_m(LPVOID);
DWORD WINAPI thread_n(LPVOID);
DWORD WINAPI thread_p(LPVOID);

HANDLE sem_init(int n)
{
	HANDLE new_sem = CreateSemaphore(nullptr, n, MAX_SEM_COUNT, nullptr);
	if (new_sem)
		return new_sem;
	std::cerr << "Cannot create semaphore, code:" << GetLastError() << std::endl;
	exit(-1);
}

HANDLE create_thread(DWORD(WINAPI* temp)(LPVOID))
{
	HANDLE new_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)temp, NULL, 0, NULL);
	if (new_thread)
		return new_thread;
	std::cerr << "Cannot create thread, error code: " << GetLastError() << std::endl;
	exit(-1);
}

void wait_threads()
{
	for (int i = 0; i < THREADCOUNT; i++) 
	{
		//Wait for execute
		while (WaitForSingleObject(threads[i], 0) == WAIT_TIMEOUT) 
		{
			Sleep(0);
		}
	}
}

void print_char(char c)
{
	while (WaitForSingleObject(stdout_mutex, 0) == WAIT_TIMEOUT)
		Sleep(0);
	std::cout << c << std::flush;
	ReleaseMutex(stdout_mutex);
}


void sem_wait(HANDLE s)
{
	auto status = WaitForSingleObject(s, 0);
	while (status == WAIT_TIMEOUT)
	{
		status = WaitForSingleObject(s, 0);
	}
}

void sem_post(HANDLE s)
{
	ReleaseSemaphore(s, 1, NULL);
}


int lab3_init()
{
	stdout_mutex = CreateMutex(NULL, false, NULL);
	app_mutex = CreateMutex(NULL, false, NULL);


	semA = sem_init(0);
	semB = sem_init(0);
	semC = sem_init(0);
	semG = sem_init(0);
	semK = sem_init(0);
	semI = sem_init(0);
	semF = sem_init(0);
	semM = sem_init(0);
	
	semE1 = sem_init(0);
	semE2 = sem_init(0);
	semD1 = sem_init(0);
	semD2 = sem_init(0);
	semD3 = sem_init(0);
	semH1 = sem_init(0);
	semH2 = sem_init(0);
	semN1 = sem_init(0);
	
	semSync1 = sem_init(1);
	semSync2 = sem_init(0);
	semSync3 = sem_init(0);

	int count = 0;


	threads.resize(13);
	threads[count] = create_thread(thread_a);
	count++;
	threads[count] = create_thread(thread_d);
	count++;
	
	threads[count] = create_thread(thread_b);
	count++;
	threads[count] = create_thread(thread_c);
	count++;
	threads[count] = create_thread(thread_g);
	count++;
	threads[count] = create_thread(thread_h);
	count++;
	
	threads[count] = create_thread(thread_f);
	count++;
	threads[count] = create_thread(thread_e);
	count++;
	threads[count] = create_thread(thread_k);
	count++;
	threads[count] = create_thread(thread_i);
	count++;
	
	threads[count] = create_thread(thread_m);
	count++;
	
	threads[count] = create_thread(thread_n);
	count++;
	threads[count] = create_thread(thread_p);
	count++;


	wait_threads();

	//clear memory
	CloseHandle(stdout_mutex);
	CloseHandle(app_mutex);
	
	CloseHandle(semA);
	CloseHandle(semB);
	CloseHandle(semC);

	return 0;
}

void post_sems(int n, HANDLE sem)
{
	for (int i = 0; i < n; i++) { sem_post(sem); }
}

void stage1_wait()
{
	sem_wait(semA);
	sem_wait(semD1);
}

void stage2_wait()
{
	sem_wait(semH1);
	sem_wait(semD2);
	
	sem_wait(semG);
	sem_wait(semB);
	sem_wait(semC);
}

void stage3_wait()
{
	sem_wait(semK);
	sem_wait(semI);
	sem_wait(semF);
	
	sem_wait(semE1);
	sem_wait(semH2);
}

void stage4_wait()
{
	sem_wait(semN1);
	sem_wait(semE2);
	
	sem_wait(semM);
}


DWORD WINAPI thread_a(LPVOID)
{
	for (int i = 0; i < 3; i++) { print_char('a'); computation(); }

	//post 1 stage
	post_sems(5, semA);
	return 0;
}
DWORD WINAPI thread_d(LPVOID)
{
	for (int i = 0; i < 3; i++) { print_char('d'); computation(); }

	post_sems(5, semD1);
	
	stage1_wait();
	for (int i = 0; i < 3; i++) { print_char('d'); computation(); }
	
	post_sems(6, semD2);
	
	stage2_wait();
	for (int i = 0; i < 3; i++) { print_char('d'); computation(); }
	
	post_sems(2, semD3);
	
	return 0;
}

/////////////////2

DWORD WINAPI thread_b(LPVOID)
{
	stage1_wait();
	for (int i = 0; i < 3; i++) { print_char('b'); computation(); }
	post_sems(6, semB);
	return 0;
}
DWORD WINAPI thread_c(LPVOID)
{
	stage1_wait();
	for (int i = 0; i < 3; i++) { print_char('c'); computation(); }

	post_sems(6, semC);

	return 0;
}
DWORD WINAPI thread_g(LPVOID)
{
	stage1_wait();
	for (int i = 0; i < 3; i++) { print_char('g'); computation(); }

	post_sems(6, semG);
	return 0;
}
DWORD WINAPI thread_h(LPVOID)
{
	stage1_wait();
	for (int i = 0; i < 3; i++) { print_char('h'); computation(); }
	post_sems(6, semH1);
	
	stage2_wait();
	for (int i = 0; i < 3; i++) { print_char('h'); computation(); }
	post_sems(6, semH2);
	
	return 0;
}

/////////////////3

DWORD WINAPI thread_e(LPVOID)
{
	stage2_wait();
	//sequential
	for (int i = 0; i < 3; i++) { print_char('e'); computation(); }
	post_sems(3, semE1);
	
	stage3_wait();
	for (int i = 0; i < 3; i++) 
	{
		sem_wait(semSync1);
		print_char('e'); 
		computation();
		sem_post(semSync2);
	}
	
	post_sems(2, semE2);
	
	return 0;
}
DWORD WINAPI thread_f(LPVOID)
{
	stage2_wait();

	for (int i = 0; i < 3; i++) { print_char('f'); computation(); }
	post_sems(3, semF);

	return 0;
}
DWORD WINAPI thread_k(LPVOID)
{
	stage2_wait();
	for (int i = 0; i < 3; i++) { print_char('k'); computation(); }
	post_sems(3, semK);
	return 0;
}
DWORD WINAPI thread_i(LPVOID)
{
	stage2_wait();
	for (int i = 0; i < 3; i++) { print_char('i'); computation(); }
	post_sems(3, semI);
	return 0;
}


/////////////////4

DWORD WINAPI thread_m(LPVOID)
{
	stage3_wait();
	for (int i = 0; i < 3; i++) 
	{
		sem_wait(semSync2);
		print_char('m');
		computation();
		sem_post(semSync3);
	}
	post_sems(2, semM);
	return 0;
}
DWORD WINAPI thread_n(LPVOID)
{
	stage3_wait();
	for (int i = 0; i < 3; i++)
	{
		sem_wait(semSync3);
		print_char('n');
		computation();
		sem_post(semSync1);
	}
	
	post_sems(2, semN1);
	
	stage4_wait();
	for (int i = 0; i < 3; i++) { print_char('n'); computation(); }
	
	return 0;
}

////////////////5

DWORD WINAPI thread_p(LPVOID)
{
	stage4_wait();
	for (int i = 0; i < 3; i++) { print_char('p'); computation(); }


	return 0;
}
