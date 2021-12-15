all:
	mkdir build; cd build; cmake ..; make

clean:
	rm -Rvf build

