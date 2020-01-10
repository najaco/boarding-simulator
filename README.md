# boarding-problem
Multi-threaded program that simulates the onboarding experience of airplanes.

## Build
To build the project run:
```console
$ cmake .
$ make all
```

## Run
If given an input file (`input.txt`) of numbers separated line by line, you can run the program by with either of the following:
```console
$ ./boarding-problem -f input.txt
```
```console
$ cat input.txt | ./boarding-problem
```

## Run Script
The script `run.sh` is provided, which generates up to a specified number in a specified order.
### Usage
```console
$ ./run.sh [-h --help] [-irs] <number_of_passengers>

```

#### Examples
To run with *1000* passengers **inorder**:
```console
$ ./run.sh -i 1000
```
To run with *1000* passengers in **reverse** order:
```console
$ ./run.sh -r 1000
```
To run with *1000* passengers in **random** order:
```console
$ ./run.sh -s 1000
```
