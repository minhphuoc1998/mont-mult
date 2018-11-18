GEN_DEST = "./sample_data/"
GEN_FILE = "gentest.py"
GEN_NAME = "input"
MPIRUN = "mpirun"
SEQ = "seq"
MPI = "mpi"
OMP = "omp"
TYPES = [SEQ, MPI, OMP]

RES_DEST = "./result/"
SEQ_RES_NAME = "seq_result"
MPI_RES_NAME = "mpi_result"
OMP_RES_NAME = "omp_result"
RES = {SEQ:SEQ_RES_NAME, MPI:MPI_RES_NAME, OMP:OMP_RES_NAME}

BIN_DEST = "./bin/"
SEQ_TEST = "seqtest"
MPI_TEST = "mpitest"
OMP_TEST = "omptest"
TEST = {SEQ:SEQ_TEST, MPI:MPI_TEST, OMP:OMP_TEST}

import os
import sys

def empty(_type):
	f = open(RES_DEST + RES[_type], 'w')
	f.close()

def gen(_num, _length):
	print " ".join(["GENERATING", str(_num), "TESTCASES WITH", str(_length << 7), "BITS LENGTH"])
	comp = ["python", GEN_DEST + GEN_FILE, str(_num), str(_length), GEN_DEST + GEN_NAME]
	os.system(" ".join(comp))

def command(_type, _length, _num, _num_threads = 0):
	return ' '.join([BIN_DEST + TEST[_type], str(_length), str(_num), GEN_DEST + GEN_NAME]), RES_DEST + RES[_type]

def seq_test(_length, _num):
	print "***SEQ TESTING***"
	empty(SEQ)
	for length in range(4, _length + 1, 4):
		gen(_num, length)
		comp = [BIN_DEST + TEST[SEQ], str(length), str(_num), GEN_DEST + GEN_NAME]
		print " ".join(["TESTING", str(_num), "TESTCASES WITH", str(length << 7), "BITS LENGTH"])
		os.system(" ".join(comp) + " >> " + RES_DEST + RES[SEQ])
	print "********************************************"
	print "CHECK " + RES_DEST + RES[SEQ] + " FOR RESULT"
	print "***SEQ TESTING: DONE***"

def mpi_test(_length, _num, _num_threads):
	print "***MPI TESTING WITH " + str(_num_threads) + " THREADS***"
	empty(MPI)
	for length in range(4, _length + 1, 4):
		gen(_num, _length)
		comp = [MPIRUN, "-n " + str(_num_threads), BIN_DEST + TEST[MPI], str(length), str(_num), GEN_DEST + GEN_NAME, str(_num_threads)]
		print " ".join(["TESTING", str(_num), "TESTCASES WITH", str(length << 7), "BITS LENGTH"])
		os.system(" ".join(comp) + " >> " + RES_DEST + RES[MPI])
	print "********************************************"
	print "CHECK " + RES_DEST + RES[MPI] + " FOR RESULT"
	print "***MPI TESTING: DONE***"

def omp_test(_length, _num, _num_threads):
	print "***OMP TESTING WITH " + str(_num_threads) + " THREADS***"
	empty(OMP)
	for length in range(4, _length + 1, 4):
		gen(_num, length)
		comp = [BIN_DEST + TEST[OMP], str(length), str(_num), GEN_DEST + GEN_NAME, str(_num_threads)]
		print " ".join(["TESTING", str(_num), "TESTCASES WITH", str(length << 7), "BITS LENGTH"])
		os.system(" ".join(comp) + " >> " + RES_DEST + RES[OMP])
	print "********************************************"
	print "CHECK " + RES_DEST + RES[OMP] + " FOR RESULT"
	print "***OMP TESTING: DONE***"

def all_test():
	seq_test()
	mpi_test()
	omp_test()

def main():
	if len(sys.argv) < 4:
		print "Usage: python test.py <type_test> <length_of_number> <number_of_test> <num_threads>"
	else:
		_type = sys.argv[1]
		_length = int(sys.argv[2])
		_num = int(sys.argv[3])
		
		if _type == SEQ:
			seq_test(_length, _num)
		elif _type == OMP:
			_num_threads = int(sys.argv[4])
			omp_test(_length, _num, _num_threads)
		elif _type == MPI:
			_num_threads = int(sys.argv[4])
			mpi_test(_length, _num, _num_threads)
		elif _type == "all":
			_num_threads = int(sys.argv[4])
			seq_test(_length, _num)
			omp_test(_length, _num, _num_threads)
			mpi_test(_length, _num, _num_threads)
		
			
if __name__ == "__main__":
	main()