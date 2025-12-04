.PHONY: clean

day1.exe: day1.ml
	dune build day1.exe
	cp _build/default/day1.exe day1.exe

day3.exe: day3.cpp
	g++ -o day3.exe day3.cpp

clean:
	rm -f day1.exe day3.exe
	dune clean
