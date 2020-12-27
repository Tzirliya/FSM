# Finite State Machine
This is my Computer Systems semester project.
It's a finite state machine coded in C.
These are the [project requirements](https://docs.google.com/document/d/1tHuGRHS5HiEXZUwigwdqy_AOisYOH_ZUptY2BbIfMng/edit?pli=1#) for the semester project.

## Getting Started
These instructions will allow you to copy the project and run it on your local machine.

### Prerequisites
I use CLion to develop and run the project, and I recommend using it for this project.

### Installation
Create an empty git repository

```git init```

Clone this repository

```git clone https://github.com/Tzirliya/FSM```

Open Clion

```clion```

Add the relative path of the definition file and the input file (separated by a space) to the program arguments

```Run >> Edit Configurations >> Program Arguments```

Select the relevant option in the run/debug configuration dropdown

```SemesterProject | Debug```

The program is ready to be run!

## Testing

### End-to-End Tests
There are three tests that run the program from start to finish.
The first test runs the program using the test files that are provided in the [project requirments doc](https://docs.google.com/document/d/1tHuGRHS5HiEXZUwigwdqy_AOisYOH_ZUptY2BbIfMng/edit?pli=1#).
The files are named test1.fsm and test1.inputs.
Tests two and three use test files with invalid inputs, so they're both supposed to fail.
These tests are not very comprehensive, as I focused my time on unit tests instead.

In order to run these tests, select the ```All Tests``` option in the run/debug configuration dropdown.

### Unit Tests
```cmake-build-debug/Testing/Tests/tests.c``` had all of the unit tests I created.
These unit tests test every function that's necessary for the project to run.
The unit tests redirect ```stdout``` to a file and only prints it when the test fails.
Additionally, ```stderr``` is redirected to ```/dev/null``` because many tests are supposed to raise an error.

To run these tests, select the ```Test``` option in the run/debug configuration dropdown.

## Future Features and Improvements Considered
The following are features and improvements that I would like to have implemented given more time.

### Support for String Inputs
Currently, the program only allows for alphabet character inputs as transitions between states.
Originally when I began this project, I assumed that all string inputs are allowed, so I began the project with that in mind.
However, when I found out that only support for alphabet characters is necessary, I changed the design of the program to be simpler and more efficient.
In order to support string inputs, I created a struct that holds the string input and the corresponding next state.
Each state holds a list of these structs.
With this implementation, searching for the next state takes linear time.
However, limiting the input to a character set of size 52, I was able to just create an array with 52 locations, and index to get the next state.
This implementation is constant time and much simpler to implement, so this is what the program currently does.

### Standardize Errors
Currently, error messages are print statements to ```stderr```.
Ideally, I'd like to codify the message (perhaps using an enum) so that they can be used while testing to ensure that the correct error occurs.
