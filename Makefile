all:
	@echo "  CPP  bin-6502"
	g++ 6502.cpp -o bin-6502
	@echo "  CPPD bin-6502-debug"
	g++ -pg 6502.cpp -o bin-6502-debug

clean:
	@echo " CLEAR bin-6502 bin-6502-debug gmon.out"
	@rm -rf bin-6502 bin-6502-debug gmon.out
