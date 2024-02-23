#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;
/*
 * Use Genetic Programming to find the best function that matches Table 10.9 of the Textbook.
 * Your program output should include the final function and the
 * number of generations of chromosome population generated to produce the final function.
 * Please run your program at least 10 times and save the outputs in a file called readme.
 * The readme file should include the problem statement,
 * a description of the chromosome encoding, crossover, mutation mechanisms.
 * Put your program, the readme file,
 * and any additional information in a folder called "yourFirstName  yourLastName Genetic Programming”.
 * Compress the folder into a zip file and upload the zip file to Blackboard.
 * Make sure your program is properly documented.
 */


int random(int n) {
    std::random_device randomizer;
    std::mt19937 generate(randomizer());

    std::uniform_int_distribution<int> r(0, n-1);

    return r(generate);
}
string getValue(){
    string values[] = {"x","-5","-4","-3","-2","-1","0","1","2","3","4","5","+","-","*","/"};
    int size = sizeof(values)/sizeof(values[0]);
    int valueGot = random(size);
    string stringOf = values[valueGot];
    return stringOf;
}
struct tree{
    string value;
    struct tree *left, *right;

    tree(string s) : value(s), left(nullptr), right(nullptr){};
};

tree* newTree() {
    string branch = getValue();
    tree* root = new tree(branch);
    if (branch == "+" || branch == "-" || branch == "*" || branch == "/") {
        root->left = newTree();
        root->right = newTree();
    }
    return root;
}

void printTree(tree* root) {
    if (root == nullptr) {
        return;
    }
    // If root is an operator, print parentheses
    if (root->value == "+" || root->value == "-" || root->value == "*" || root->value == "/") {
        cout << "(";
    }
    printTree(root->left);
    cout << root->value << " ";
    printTree(root->right);
    if (root->value == "+" || root->value == "-" || root->value == "*" || root->value == "/") {
        cout << ")";
    }
}

double calcTree(tree* root, double xVal) {
    if (root == nullptr)
        return 0;

    // If root is an operator, recurse
    if (root->value == "+") {
        return (calcTree(root->left, xVal) + calcTree(root->right, xVal));
    } else if (root->value == "-") {
        return (calcTree(root->left, xVal) - calcTree(root->right, xVal));
    } else if (root->value == "*") {
        return (calcTree(root->left, xVal) * calcTree(root->right, xVal));
    } else if (root->value == "/") {
        double rightValue = calcTree(root->right, xVal);//check for /0
        if (rightValue != 0)
            return (calcTree(root->left, xVal) / rightValue);
        else {
            rightValue = 1; // to avoid division by 0, replace with 1
            return (calcTree(root->left, xVal) / rightValue);
        }
    } else if (root->value == "x") { // if x
        return xVal;
    } else {
        return stod(root->value);
    }
}

tree* cross(tree* t1, tree* t2) {
    if (t1 == nullptr || t2 == nullptr) {
        return nullptr;
    }
    int r = random(2);

    tree* t3 = new tree((r == 0) ? t1->value : t2->value);

    // Recursively crossover subtrees
    if (r == 0) {
        t3->left = cross(t1->left, t2);
        t3->right = cross(t1->right, t2);
    } else {
        t3->left = cross(t1, t2->left);
        t3->right = cross(t1, t2->right);
    }

    return t3;
}


int nodeCount(tree* tree) { // counts nodes of a tree to make sure that i dont get hundreds of trees with hundreds of nodes to cut down on processing time
    if (tree == nullptr) {
        return 0;
    }
    return 1 + nodeCount(tree->left) + nodeCount(tree->right);
}



int main() {
    int size = 600;
    double xVal;
    tree* population[size];
    //winners and children will populate into this tree, and at the end of every generation, population will be replaced with this
    int generations = 2500;
    int generationBestFound = -1;//there is a check that runs every generation to see if x^2/2 was found, and if so, it is printed at the end which generation


    double xVals[] = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9}; // x^2/2
    double yVals[] = {0,0.005,0.020,0.045, 0.080, 0.125, 0.180, 0.245, 0.32, 0.405}; // x^2/2


    for (int i = 0; i < size; i++) {//set initial population
        tree* root = newTree();
        population[i] = root;
    }
    tree* overallBest = population[0];//keep track of the overallBest tree across every generation

    for (int i = 0; i < generations; ++i) {//iterate through every generation
        tree* children[size];
        //tournament to decide winners
        int index = 0;
        for (int j = 0; j < size; j+=2) {
            double errorSum1 = 0.0;
            double errorSum2 = 0.0;
            //evaluate fitness between pop[j] and pop[j+1] for values of x to determine winner
            for (int k = 0; k < 10; k++) {
                xVal = xVals[k];

                double t1 = calcTree(population[j], xVal);
                double t2 = calcTree(population[j+1], xVal); //calculate tree values

                errorSum1 += abs(t1-yVals[k]);
                errorSum2 += abs(t2-yVals[k]);
            }//end errorSum
            //winners populate new tree with children
            if(errorSum1 < errorSum2){//populate children tree with winners
                children[index++] = population[j];
            }
            else{
                children[index++] = population[j+1];
            }

        }//end tournament

        //winners reproduce by creating 2 children per 2 parents
        //crossover
        for (int j = 0; j < size/2; j+= 2) {
            tree* newChild1 = cross(children[j], children[j+1]);
            tree* newChild2 = cross(children[j+1], children[j]);
            //latter half of children get replaced with the crossover
            children[index++] = newChild1;
            children[index++] = newChild2;
        }


        //replace population with children values

        for (int j = 0; j < size; ++j) {
            population[j] = children[j];
        }

        //shuffle
        shuffle(population, population + size, std::mt19937(std::random_device()()));

        //set children values to null to avoid issues with data carryover
        for (int j = 0; j < size; ++j) {
            children[j] = nullptr;
        }

        //output the best tree per generation and check if we have a new best overall tree
        tree* best = population[0];
        double error = 0.0;
        for (int k = 0; k < 10; k++) {
            xVal = xVals[k];

            double t1 = calcTree(best, xVal);
            error += abs(t1-yVals[k]);
        }

        for (int j = 1; j < size; j++) {
            double errorSum = 0.0;

            //evaluate fitness between pop[j] and pop[j+1] for values of x to determine winner
            for (int k = 0; k < 10; k++) {
                xVal = xVals[k];

                double t1 = calcTree(population[j], xVal);
                errorSum += abs(t1-yVals[k]);
            }//end errorSum

            if(errorSum < error){
                best = population[j];
            }

        }

        for (int j = 0; j < 15; ++j) {
            int mutate = random(300)+300;
            delete population[mutate]; //kill the tree, i dont want to logic replacing a value
            population[mutate] = newTree();
        }


        //check if a tree is too long, and makes new tree
        for (int j = 0; j < size; ++j) {
            if(nodeCount(population[j]) > 25){
                delete population[j];
                population[j] = newTree();
            }
        }

        double errorBest = 0.0;
        for (int k = 0; k < 10; k++) {
            xVal = xVals[k];

            double t1 = calcTree(overallBest, xVal);
            errorBest += abs(t1-yVals[k]);
        }


        double errorSum = 0.0;
        for (int k = 0; k < 10; k++) {
            xVal = xVals[k];
            double t1 = calcTree(best, xVal);
            errorSum += abs(t1-yVals[k]);
        }//end errorSum

        if(errorSum < errorBest){
            overallBest = best;
        }
        if(errorSum <= 1.01481e-16){//if the error between the values is astronomically small, because the way my calculations work, it doesnt go to 0
            generationBestFound = i+1;
        }


        cout << "Generation " << i+1 << ":\n";
        cout<< "Best of generation: ";
        printTree(best);
        cout<<"\n";
        cout<< "Best Overall: ";
        printTree(overallBest);
        cout<<"\n";
        if(generationBestFound != -1){
            cout<<"Value found in Generation: " << generationBestFound;
            break;
        }
        cout<<"\n\n";





    }//close generations

    return 0;
}