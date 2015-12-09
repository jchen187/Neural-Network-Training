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
#include <math.h> //to use the activation function because we need e
#include <iomanip> //for 3 decimal places

using namespace std;

string file1, file2, file3;

//from the first file
int inputNodes, hiddenNodes, outputNodes;
vector<vector<float>> weightsToHidden;
vector<vector<float>> weightsToOutput;
vector<vector<float>> network;

//from the 2nd file
int numTrainingExamples, inputs, outputs;
vector<vector<float>> exampleInputs;
vector<vector<float>> exampleOutputs; //0 or 1
vector<vector<float>> examples;

//user enters this
int epoch, learningRate;

void readFromFile1(string name);
void readFromFile2(string name);
void writeNetworkToFile(string name, vector<vector<float>> network);

vector<vector<float>> backPropLearning(vector<vector<float>> examples, vector<vector<float>> network);

int main(int argc, const char * argv[]) {

    cout << "Please enter the file containing the initial neural network\n";
    //read the file
//    cin >> file1;
    file1 = "initialNN.txt";
    readFromFile1(file1);
    
    network.reserve( weightsToHidden.size() + weightsToOutput.size() ); // preallocate memory
    network.insert( network.end(), weightsToHidden.begin(), weightsToHidden.end() );
    network.insert( network.end(), weightsToOutput.begin(), weightsToOutput.end() );
    
    cout << "Please enter the file containing the training set.\n";
//    cin >> file2;
//    readFromFile2(file2);
    file2 = "train.txt";
    readFromFile2(file2);
    
    examples.reserve( exampleInputs.size() + exampleOutputs.size() ); // preallocate memory
    examples.insert( examples.end(), exampleInputs.begin(), exampleInputs.end() );
    examples.insert( examples.end(), exampleOutputs.begin(), exampleOutputs.end() );
    
    cout << "Where would you like to output the results to?\n";
    file3 = "results.txt";
    
    cout << "Choose epoch.\n";
//    cin >> epoch;
    epoch = 100;
    
    cout << "Choose learning rate.\n";
//    cin >> learningRate;
    learningRate = 0.1;
    
    vector<vector<float>> newNetwork = backPropLearning(examples, network);
    writeNetworkToFile(file3, newNetwork);
//    writeNetworkToFile("t.txt", network);

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
//                cout << "j = " << j << "m = " << m << endl;
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
    /*
     1st line has 3 integers
     - number of training examples
     - number of input nodes(will match file1)
     - number of output nodes(will match file1)
     
     every other line is an example
     - Ni floating point inputs
     - No Boolean output(0 or 1)
     */
    
    ifstream myFile;
    myFile.open(file2);
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
        if (inputs != inputNodes || outputs != outputNodes){
            cout << "The input and output values do not match the initial neural network.";
        }
        else{
            int sum = inputs + outputs;
            
            exampleInputs.resize(numTrainingExamples);
            exampleOutputs.resize(numTrainingExamples);
            //go through each line one at a time, put the stuff in a vector, and do the backprop
            for (int i = 0; i < numTrainingExamples; i++){
                exampleInputs[i].resize(inputs);
                exampleOutputs[i].resize(outputs);
                for (int j = 0; j < sum; j++){
                    if (j < inputs){
                        myFile >> exampleInputs[i][j];
                    }
                    else {
                        myFile >> exampleOutputs[i][j-inputs];
                    }
                }
            }
            myFile.close();
        }
    }
    else
        cout << "Unable to open file.\n";
}

float applyActivFunct(float x){
    float result = 1/(1+exp(-1*x));
    return result;
}

float applyDerivActivFunct(float x){
    float result = applyActivFunct(x) * (1 - applyActivFunct(x));;
    return result;
}

//given return a neural network and the examples, return a neural network
//examples include both input and output examples. same with network. contains first weight from input to hidden node, then hidden node to output
vector<vector<float>> backPropLearning(vector<vector<float>> examples, vector<vector<float>> network){
    
    //error for both outlayer and hiddenlayer
    vector<vector<float>> errors;
    errors.resize(2);
    errors[0].resize(outputNodes);
    errors[1].resize(hiddenNodes);
    
    int loop = 0;
    
    while (loop < epoch){
        //go through each example in examples
        //you could do numTrainingExamples or you can do the exampleInputs.size()
        for (int i = 0; i < numTrainingExamples; i++){
    
            //base will contain all the example inputs
            vector<float> base = examples[i];
            
            //propagate the inputs forward to compute the outputs
            vector<float> middle;
            middle.resize(hiddenNodes);
            
            //loop through each hidden node
            for (int j = 0; j < hiddenNodes; j++){
                float result;
                //get contribution from each node from previous layer
                for (int k = 0; k < inputNodes+1; k++){
                    //for the first weight, multiply by the fixed input -1
                    if (k == 0){
                        result += -1 * network[j][0];
                    }
                    else {
                        //network has the bias weight at the beginningn and is ahead of the example/base
                        result += network[j][k] * base[k-1];
                    }
                }
                middle[j] = applyActivFunct(result);
            }
            
            //output
            vector<float> top;
            top.resize(outputNodes);
            for (int j = 0; j < outputNodes; j++){
                float result;
                for (int k = 0; k < hiddenNodes+1; k++){
                    if (k == 0){
                        result += -1 * network[hiddenNodes+j][0];
                    }
                    else {
                        result += network[hiddenNodes+j][k] * middle[k-1];
                    }
                }
                top[j] = applyActivFunct(result);
            }
            
            //vector<float> base = examples[i];
            
            //propagate delta backwards from output layer to input
            //go through each node in the output later
            //errors[0] contains the error of the output
            for (int j = 0; j < outputNodes; j++){
                errors[0][j] = applyDerivActivFunct(top[j]) * (examples[numTrainingExamples+i][j] - top[j]);
            }
            
            for (int j = 0; j < hiddenNodes; j++){
                //loop through the output layer
                for (int k = 0; k < outputNodes; k++){
                    //ARE WE IGNORING THE BIAS WEIGHT? if we are we will use j+1
                    errors[1][j] += network[hiddenNodes+k-1][j+1] * errors[0][k];
                }
                errors[1][j] *= applyDerivActivFunct(middle[j]);
            }
            
            //update every weight in network - breaking this up into two separate parts
            //
            for (int j = 0; j < hiddenNodes; j++){
                //wij = wij + learningRate * activationI *errorsJ
                
                //input to hidden layer
                for (int k = 0; k < inputNodes; k++){
                    network[j][k] = network[j][k] + learningRate * base[k] * errors[1][j];
                }
            }
            
            for (int j = 0; j < outputNodes; j++){
                //wij = wij + learningRate * activationI *errorsJ
                
                //hidden layer to output
                for (int k = 0; k < hiddenNodes; k++){
                    network[j][k] = network[j][k] + learningRate * middle[k] * errors[0][j];
                }
            }
        }
        loop++;
    }
    
    return network;
}

void writeNetworkToFile(string name, vector<vector<float>> network){
    ofstream myfile;
    myfile.open (name);
    if (myfile.is_open())
    {
        myfile << inputNodes << " " << hiddenNodes << " " << outputNodes << "\n";
        
        //size is hidden + output
        for (int i = 0; i < hiddenNodes; i++){
//            myfile << "This is another line.\n";
            for (int j = 0; j < inputNodes + 1; j++){
                myfile << fixed << setprecision(3) << network[i][j] << " ";
//                myfile << network[i][j] << " ";
            }
            myfile << "\n";
        }
        for (int i = 0; i < outputNodes; i++){
            
            for (int j = 0; j < hiddenNodes + 1; j++){
                myfile << fixed << setprecision(3) << network[hiddenNodes+i][j] << " ";
            }
            myfile << "\n";
        }
        
        myfile.close();
    }
    else cout << "Unable to open file";
}


