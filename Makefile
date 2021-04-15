lib=/HME-Quartz-broadwell/src/lib
so=/HME-Quartz-broadwell/build/src/lib
obj=main.o node.o edge.o graph.o

main:$(obj)
	g++ -o main $(obj) -I $(lib) -L $(so) -lnvmemul
edge.o:edge.cpp
	gcc -c edge.cpp -I $(lib) -L $(so) -lnvmemul
%.o:%.cpp
	gcc -c $<
.PHONY:clean
clean:
	rm $(obj) main

