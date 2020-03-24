build:
	g++ -o main main.cpp beamformer/beamformer.cpp fft/fft.cpp -lfftw3 -lm -I . -I ./beamformer -I ./fft

clean:
	rm test/test_data/*
	rm main

run:
	./main

.PHONY: build clean run
