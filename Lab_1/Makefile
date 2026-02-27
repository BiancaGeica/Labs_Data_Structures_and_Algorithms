build: charTest longTest
	
charTest: OrderedSet.h charTest.c
	gcc -std=c9x charTest.c -o charTest

longTest: OrderedSet.h longTest.c
	gcc -std=c9x longTest.c -o longTest
clean:
	rm -f charTest longTest