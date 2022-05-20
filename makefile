all:test

test:test.o fs.o fs.h
	gcc -o test test.o fs.o

test.o: test.c fs.h
	gcc -c test.c

fs.o: fs.c fs.h
	gcc -c fs.c

clean:
	rm -f *.o test fs

# mkfs:
# 	$(CC) $(flags) -o mkfs mkfs.cpp; ./mkfs

# clean:
# 	rm -f mkfs