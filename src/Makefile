default:
	make clean; make callgrind

mm: mm.c
	gcc mm.c -o mm

callgrind: mm 
	valgrind --tool=callgrind --simulate-cache=yes --I1=1024,2,64 --D1=2048,4,64 --LL=131072,1,32 ./mm 128 128
	
clean:
	rm -rf callgrind.out.*
