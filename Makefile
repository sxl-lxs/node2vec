lib=/HME-Quartz-broadwell/src/lib
so=/HME-Quartz-broadwell/build/src/lib
obj=main.o node.o edge.o graph.o

main:$(obj)
	g++ -o main $(obj) -I $(lib) -L $(so) -lnvmemul
edge.o:edge.cpp
	gcc -c edge.cpp -I $(lib) -L $(so) -lnvmemul
node.o:node.cpp
	gcc -c node.cpp -I $(lib) -L $(so) -lnvmemul
graph.o:graph.cpp
	gcc -c graph.cpp
.PHONY:clean
clean:
	rm $(obj) main
