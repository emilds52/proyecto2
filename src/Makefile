default:
	make clean; make callgrind;
	callgrind_annotate --auto=yes callgrind.out.* mm.c

mm: mm.c
	gcc mm.c -o mm # Lo hicimos sin flag de optimización. Nos dimos cuenta al final que deberíamos haber puesto -O2 o -O6

callgrind: mm 
	valgrind --tool=callgrind --simulate-cache=yes --I1=1024,2,64 --D1=2048,4,64 --LL=131072,1,32 ./mm 1024 64
	
clean:
	rm -rf callgrind.out.*

cacti:
	cd ../../valgrind/cacti; ./cacti -infile cache.cfg
