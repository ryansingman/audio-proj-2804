build:
	g++ -o bin/main src/main.cpp src/beamformer/beamformer.cpp src/fft/fft.cpp src/vis/vis.cpp -lfftw3 -lm -I include/

clean:
	rm test/test_data/*
	rm bin/main

run:
	./bin/main

.PHONY: build clean run
