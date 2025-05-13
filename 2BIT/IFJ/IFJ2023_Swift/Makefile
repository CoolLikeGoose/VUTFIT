all:
	mkdir -p build
	rm -r ./build/*
	cd build && cmake ..
	cd build && cmake --build . -j 10
	mv ./build/*IFJ* ./: