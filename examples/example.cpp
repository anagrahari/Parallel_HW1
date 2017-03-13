#include <iostream>
#include <cilk/cilk.h>

using namespace std; 

static void hello(){
	for(int i=0;i<10;i++)
		cout << "Hello " << endl;
}

static void world(){
	for(int i=0;i<10;i++)
		cout << "world! " << endl;
}

int main(){
	cilk_spawn hello();
	cilk_spawn world();
	cout << "Done! ";
}
