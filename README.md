# Montgomery Multiplication
This project contains the implementation of some Sequential Montgomery Multiplication[1] (SOS, CIOS, FIOS) and Parallel Montgomery Multiplication using OpenMP and MPI.

The implementation depends on the paper:

"THE PERFORMANCE OF CRYPTOGRAPHIC ALGORITHMS IN THE AGE OF PARALLEL COMPUTING"[2]

## Installation
Downloading the source.
```
git clone https://github.com/minhphuoc1998/parallelcomputing.git
```

Building the source.
```
cd parallelcomputing/src
make
```

The compiled files are in ```bin``` directory.

## Usage
Genereting tescases
```
python ./sample_data/gentest.py <length_of_number> <number_of_test> <output_dest>
```

Sequential algorithm testing (include SOS, CIOS, FIOS and FIOS modified)
```
./bin/seqtest <length_of_test> <number_of_test> <data_file>
```

MPI parallel algorithm testing
```
mpirun -n <number_of_threads> ./bin/seqtest <length_of_test> <number_of_test> <data_file> <number_of_threads>
```

OpenMP parallel algorithm testing
```
./bin/omptest <length_of_test> <number_of_test> <data_file> <number_of_threads>
```

Various length testing
```
python test.py <type_test> <length_of_number> <number_of_test> <num_threads>
```
In which ```<type_test>``` is one of ```seq``` (sequential), ```omp``` (openmp), ```mpi``` (mpi), ```all``` (all types).

### Example
Sequential testing against 512 to 4096 bits length
```
python test.py seq 32 10000
```

OpenMP testing against 512 to 8096 bits length with 2 threads
```
python test.py seq 64 10000 2
```

## Result
Check ```src/result``` for the result of testing

Sequential algorithm
```
Number of Test: 1000
Length: 512 bits
Time for SOS: 0.000460
Time for CIOS: 0.000235
Time for FIOS: 0.000195
Time for FIOS (Modified): 0.000159

Number of Test: 1000
Length: 1024 bits
Time for SOS: 0.000970
Time for CIOS: 0.000591
Time for FIOS: 0.000632
Time for FIOS (Modified): 0.000520
```

MPI parallel algorithm
```
Number of Test: 1000
Length: 512 bits
Number of threads: 2
Time for PSHS from 1: 0.003165
Time for PSHS from 0: 0.003024
Number of Test: 1000
Length: 1024 bits
Number of threads: 2
Time for PSHS from 0: 0.005000
Time for PSHS from 1: 0.004987
```

OpemMP parallel algorithm
```
Number of Test: 1000
Length: 512 bits
Number of threads: 2
Time for Naive pFIOS: 0.000404
Time for New pFIOS: 0.001536

Number of Test: 1000
Length: 1024 bits
Number of threads: 2
Time for Naive pFIOS: 0.000754
Time for New pFIOS: 0.001082
```

# References
[1] https://en.wikipedia.org/wiki/Montgomery_modular_multiplication

[2] http://www.macs.hw.ac.uk/cs/msc_dissertations/2012/9.pdf