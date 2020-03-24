build:
	g++ -o main main.cpp beamformer/beamformer.cpp fft/fft.cpp vis/vis.cpp -lfftw3 -lm -I . -I ./beamformer -I ./fft -I ./vis

clean:
	rm test/test_data/*
	rm main

run:
	./main

.PHONY: build clean run
