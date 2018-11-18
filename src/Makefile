DEST = ./bin/

CC = gcc
MC = mpicc

LIBS = -fopenmp

SEQ_TEST = seqtest
MPI_TEST = mpitest
OMP_TEST = omptest

OBJS = $(SEQ_TEST) $(MPI_TEST) $(OMP_TEST)

SEQ_MON = seqmon
MPI_MON = mpimon
OMP_MON = ompmon
HUG_INT = hugeint

all: dest $(OBJS)

dest:
	mkdir $(DEST)

$(SEQ_TEST): $(HUG_INT) $(SEQ_MON)
	$(CC) -c $(SEQ_TEST).c
	$(CC) -o $(DEST)$(SEQ_TEST) $(SEQ_TEST).o $(HUG_INT).o $(SEQ_MON).o

$(MPI_TEST): $(HUG_INT) $(MPI_MON)
	$(MC) -c $(MPI_TEST).c
	$(MC) -o $(DEST)$(MPI_TEST) $(MPI_TEST).o $(HUG_INT).o $(MPI_MON).o
	
$(OMP_TEST): $(HUG_INT) $(OMP_MON)
	$(CC) -c $(OMP_TEST).c
	$(CC) -o $(DEST)$(OMP_TEST) $(OMP_TEST).o $(HUG_INT).o $(OMP_MON).o $(LIBS)

$(HUG_INT): $(HUG_INT).c $(HUG_INT).h
	$(CC) -c $(HUG_INT).c

$(SEQ_MON): $(SEQ_MON).c $(SEQ_MON).h
	$(CC) -c $(SEQ_MON).c
	
$(MPI_MON): $(MPI_MON).c $(MPI_MON).h
	$(MC) -c $(MPI_MON).c
	
$(OMP_MON): $(OMP_MON).c $(OMP_MON).h
	$(CC) -c $(OMP_MON).c

clean:
	rm -r $(DEST)
	rm *.o