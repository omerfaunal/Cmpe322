all: oneThread fiveThreads tenThreads

oneThread: oneThread.cpp
	g++ -std=c++11 -o oneThread oneThread.cpp
fiveThreads: fiveThreads.cpp
	g++ -std=c++11 -o fiveThreads fiveThreads.cpp
tenThreads: tenThreads.cpp
	g++ -std=c++11 -o tenThreads tenThreads.cpp

clean:
	rm -f oneThread fiveThreads tenThreads *.txt

run: ./oneThread ./fiveThreads ./tenThreads
	@./oneThread $(n)
	@./fiveThreads $(n)
	@./tenThreads $(n)