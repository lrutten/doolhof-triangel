all:
	mkdir build; cd build; cmake ..; make

clean:
	rm -Rvf build
	rm -Rvf cmake-build-debug/*

