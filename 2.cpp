#include <iostream> 
 
using namespace std; 
 
int main() 
{ 
    cout << "input x: "; 

    double x;
    cin >> x;

    double y;
    if(x>10)
    {
        y=-1;
    }
    else
    {
        y=5;
    }
    cout << "y=" << y;
}