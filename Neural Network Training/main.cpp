//
//  main.cpp
//  Neural Network Training
//
//  Created by Johnny Chen on 12/6/15.
//  Copyright (c) 2015 JohnnyChen. All rights reserved.
//

#include <iostream>
#include <fstream> //reading from file
#include <vector>

using namespace std;

int inputNodes, hiddenNodes, outputNodes;
vector<vector<float>> weightsToHidden;
vector<vector<float>> weightsToOutput;
string file1, file2, file3;

int numTrainingExamples, inputs, outputs;
int epoch, learningRate;

void readFromFile1(string name);
void readFromFile2(string name);

int main(int argc, const char * argv[]) {

    cout << "Please enter the file containing the initial neural network\n";
    //read the file
    cin >> file1;
    readFromFile1(file1);
    
    cout << "Please enter the file contraining the training set.\n";
    cin >> file2;
    readFromFile2(file2);
    
    cout << "Where would you like to output the results to?\n";
    
    cout << "Choose epoch.";
    cin >> epoch;
    
    cout << "Choose learning rate.";
    cin >> learningRate;
    
    return 0;
}

void readFromFile1(string name){
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
        weightsToHidden.resize(hiddenNodes);
        for (int j = 0; j < hiddenNodes; j++){
            weightsToHidden[j].resize(inputNodes+1);
            for (int m = 0; m <= inputNodes; m++){
                cout << "j = " << j << "m = " << m << endl;
                myFile >> weightsToHidden[j][m];
            }
        }
        weightsToOutput.resize(outputNodes);
        for (int k = 0; k < outputNodes; k++){
            weightsToOutput[k].resize(hiddenNodes+1);
            for (int n = 0; n <= hiddenNodes; n++){
                myFile >> weightsToOutput[k][n];
            }
        }
        myFile.close();
    }
    else
        cout << "Unable to open file.\n";
}

//training examples to train
void readFromFile2(string name){
    ifstream myFile;
    myFile.open(file1);
    if (myFile.is_open()){
        //Put contents of file into array
        for (int i = 0; i <= 3; i++){
            if (i == 0)
                myFile >> numTrainingExamples;
            if (i == 1)
                myFile >> inputs;
            if (i == 2){
                myFile >> outputs;
            }
        }
        if (inputs != inputNodes && outputs != outputNodes){
            cout << "The input and output values do not match the initial neural network.";
        }
        else{
            //go through each line one at a time, put the stuff in a vector, and do the backprop
            //look at epoch
            myFile.close();
        }
    }
    else
        cout << "Unable to open file.\n";

    /*
     1st line has 3 integers
     - number of training examples
     - number of input nodes(will match file1)
     - number of output nodes(will match file1)
     
     every other line is an example 
     - Ni floating point inputs
     - No Boolean output(0 or 1)
     */
}

//given return a neural network
vector<vector<int>> backPropLearning(vector<int> examples, vector<vector<int>> network){
    
    //what does it mean indexed by network node
    vector<int> errors;
    
    int loop = 0;
    //
    while (loop < epoch){
        //go through each example in examples
        loop++;
    }
    
    return network;
}

