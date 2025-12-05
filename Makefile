.PHONY: clean

day1.exe: day1.ml
	dune build day1.exe
	cp _build/default/day1.exe day1.exe

day3.exe: day3.cpp
	g++ -o day3.exe day3.cpp

day4.exe: day4.ml
	dune build day4.exe
	cp _build/default/day4.exe day4.exe

clean:
	rm -f day1.exe day3.exe day4.exe
	dune clean
