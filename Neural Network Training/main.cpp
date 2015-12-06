//
//  main.cpp
//  Neural Network Training
//
//  Created by Johnny Chen on 12/6/15.
//  Copyright (c) 2015 JohnnyChen. All rights reserved.
//

#include <iostream>
#include <fstream> //reading from file

int inputNodes, hiddenNodes, outputNodes;

using namespace std;

int main(int argc, const char * argv[]) {

    string file1, file2, file3;
    cout << "Please enter the file containing the initial neural network\n";
    //read the file
    cin >> file1;
    
    ifstream myFile;
    myFile.open(file1);
    if (myFile.is_open()){
        //Put contents of file into array
        for (int i = 0; i <= 3; i++){
            if (i == 0)
                myFile >> inputNodes;
            if (i == 1)
                myFile >> hiddenNodes;
            if (i == 2){
                myFile >> outputNodes;
            }
        }
        myFile.close();
    }
    else
        cout << "Unable to open file.\n";
    
    cout << "Please enter the file contraining the training set.\n";
    cout << "Where would you like to output the results to?\n";
    return 0;
}

/*
 1st line has 3 integers
 - number of input nodes
 - number of hidden nodes 
 - number of output nodes
 
 Next Nh lines each has Ni + 1 weights entering the 1st hidden node, then 2nd ... until Nh
 There is a +1 because a bias weight which is always the first weight
 Weights are floating point numbers
 
 Next No has Nh + 1 weights
*/
