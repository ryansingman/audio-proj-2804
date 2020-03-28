build:
	g++ -o bin/main src/main.cpp src/beamformer/beamformer.cpp src/fft/fft.cpp src/vis/vis.cpp -lfftw3 -lm -I include/

clean:
	rm test/test_data/*
	rm bin/main

run:
	./bin/main

freq_sweep_gen:
	cd test; python3.5 test_data_gen.py sweep

.PHONY: build clean run freq_sweep_gen
