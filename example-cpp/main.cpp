#include <iostream>

#include "device.hpp"

int main(){
    using namespace std;
    cout << "Hello World!" << endl;

    Device d("", true);

    //d.create_pipeline("");

    int wait;
    cin >> wait;

    return 0;
}