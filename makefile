./bin/n-graph-miner : obj bin obj/utility.o obj/consDes.o obj/readfrmfile.o obj/countingNew.o obj/candidateGen.o obj/filterParserNew.o obj/mainfile.o
	g++ -o ./bin/n-graph-miner ./obj/utility.o obj/consDes.o obj/readfrmfile.o obj/countingNew.o obj/candidateGen.o obj/filterParserNew.o obj/mainfile.o
obj/utility.o : src/utility.cpp src/super.h
	g++ -c src/utility.cpp -o obj/utility.o
obj/consDes.o : src/consDes.cpp src/super.h
	g++ -c src/consDes.cpp -o obj/consDes.o
obj/readfrmfile.o : src/readfrmfile.cpp src/super.h
	g++ -c src/readfrmfile.cpp -o obj/readfrmfile.o
obj/countingNew.o : src/countingNew.cpp src/super.h
	g++ -c src/countingNew.cpp -o obj/countingNew.o
obj/candidateGen.o : src/candidateGen.cpp src/super.h
	g++ -c src/candidateGen.cpp -o obj/candidateGen.o
obj/filterParserNew.o : src/filterParserNew.cpp src/super.h
	g++ -c src/filterParserNew.cpp -o obj/filterParserNew.o
obj/mainfile.o : src/mainfile.cpp src/super.h
	g++ -c src/mainfile.cpp -o obj/mainfile.o
obj:
	mkdir -p obj
bin:
	mkdir -p bin
clean:
	rm ./obj/*.o bin/*
