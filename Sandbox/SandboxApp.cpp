#include <iostream>

#define CS_CALCU_TIME_COST 1
#include "ProfilingMacro/CS_Profiling.h"

double Add(double a, double b) {
	return a + b;
}

double Mutiple(double a, double b) {
	return a * b;
}

#define CS_CALCU_TIME_COST 1

int main() {

	CS_START_TIMER("Add")
		Add(13, 14);
	CS_STOP_TIMER("Add")


	CS_START_TIMER("Add")
		Add(1, 2);
	CS_STOP_TIMER("Add")


	CS_START_TIMER("Multiple")
		Mutiple(3.0, 4.0);
	CS_STOP_TIMER("Multiple")

	for (auto i = CODE_SNIPPET::activeTimers.begin(); i != CODE_SNIPPET::activeTimers.end(); ++i) {
		std::cout << i->first << " time cost = " << static_cast<double>(i->second->GetTotalDuration()) / 1000.0 << " ms\n";
	}

	std::cout << "total time cost: "<<static_cast<double>(CODE_SNIPPET::timerManager.GetTotalTime()) / 1000.0 << std::endl;

	return 0;
}