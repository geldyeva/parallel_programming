#include <process.h>
#include <Windows.h>
#include <iostream>

using namespace std;

const int COUNT = 100;
const int NTHREAD = 4;
int arr[COUNT];

typedef struct { int *a; int left; int right; int max; } INFORM;
void init_array(int arr[])
{
	for (int i = 0; i < COUNT; i++)
		arr[i] = rand() % 100;

}


unsigned __stdcall maximum(void* arg)
{
	INFORM *inform = (INFORM*)arg;
	inform->max = inform->a[inform->left];
	for (int i = inform->left+1; i < inform->right; i++)
		if (inform->max < inform->a[i]) inform->max = inform->a[i];

	_endthreadex(0);
	return 0;
}

int max_parallel(int arr[])
{
	HANDLE t[NTHREAD];
	unsigned ThreadID[NTHREAD];
	INFORM informs[NTHREAD];
	int n = COUNT / NTHREAD;
	for (int i = 0; i < NTHREAD; i++)
	{
		informs[i].a = arr;
		informs[i].left = n * i;
		informs[i].max = informs[i].a[informs[i].left];
		if (i == NTHREAD - 1) informs[i].right = COUNT;
		else informs[i].right = n * (i + 1);

		t[i] = (HANDLE)_beginthreadex(NULL, 0, &maximum, &informs[i], 0, &ThreadID[i]);
	}

	WaitForMultipleObjects(NTHREAD, t, true, INFINITE);

	
	int tmp = informs[0].max;
	for (int i = 1; i < NTHREAD; i++)
		if (informs[i].max > tmp) tmp = informs[i].max;


	for (int i = 0; i < NTHREAD; i++)
		CloseHandle(t[i]);

	return tmp;
}

int main()
{
	int a[COUNT];
	init_array(a);
	//for (int i = 0; i < COUNT; i++)
		//cout << a[i] << ' ';

	//nonparallel
	int nonp = a[0];
	for (int i = 1; i < COUNT; i++)
		if (a[i] > nonp) nonp = a[i];

	cout << "maximum nonparallel: " << nonp << endl;
	cout << "maximum parallel: " << max_parallel(a) << endl;
	cin.ignore();
	return 0;
}















/*int main()
{
	cout << thread::hardware_concurrency() << endl;

	system("pause");
	return 0;
}*/