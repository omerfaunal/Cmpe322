all: oneThread fiveThread tenThread

oneThread: oneThread.cpp
	g++ -std=c++11 -o oneThread oneThread.cpp
fiveThread: fiveThread.cpp
	g++ -std=c++11 -o fiveThread fiveThread.cpp
tenThread: tenThread.cpp
	g++ -std=c++11 -o tenThread tenThread.cpp

clean:
	rm -f oneThread fiveThread tenThread *.txt

run: ./oneThread ./fiveThread ./tenThread
	@./oneThread $(n)
	@./fiveThread $(n)
	@./tenThread $(n)