# Genetic-Programming-Trees-
This program is a demonstration of genetic programming by making generations of tree structures converge to the equation (x^2)/2
A tree is randomly generated using the newTree function, and stored into a population array.
These trees are put into a 1 round tournament against each other to determine which trees are closer to the equation.
This is done by calculating the trees and comparing the sum of errors against differing values of the equation.
The n/2 winning trees are put into a children array, and the winners are then crossed with each other, with 2 parents creating 2 children, making n/2 child trees.
These children are created by randomly selecting values from both parents and building a new tree.
The children array is then copied into the population array.
The error is again calculated to determine the best tree in the generation and best tree overall.
There are several mutation mechanisms to increase program speed and preventing the solution from never being found.
If there are more than 25 characters in a tree, the tree is regenerated, to prevent trees with thousands of characters being formed.
There is a 5% chance per tree to regenerate into a new tree, to prevent the program from endlessly converging into the wrong solution.
If there is a divide by 0, the program treats that as a divide by 1, without altering the tree, to prevent errors.
After all mutation is done, one final error check is run to see if the solution is found. If the solution is found, the program outputs what generation and what solution was found. 
If the solution was not found, the program continues.
