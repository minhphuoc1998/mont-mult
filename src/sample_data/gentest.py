from random import randint
import sys

HEX = "0x"

def rands(l):
	return map(lambda x: HEX + hex(x)[2:].zfill(8), [randint(0, 1 << 32) for i in range(l)])

def gentest(length, ntest, dest):
	
	f = open(dest, 'w')

	for i in range(3 * ntest):
		f.write(' '.join(rands(length)) + "\n")

	f.close()

def main():
	if len(sys.argv) < 4:
		print "Usage: python gentest.py <length_of_number> <number_of_test> <output_dest>"
	else:
		gentest(int(sys.argv[1]), int(sys.argv[2]), str(sys.argv[3]))

if __name__ == "__main__":
	main()