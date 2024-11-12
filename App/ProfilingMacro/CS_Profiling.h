#pragma once
#include <chrono>
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>

/*
	usage:
	#define CS_CALCU_TIME_COST 1
	#include "CS_Profiling.h"


	CS_START_TIMER("label1")
	doSthA();
	CS_STOP_TIMER("label1")

	CS_START_TIMER("label1")
	doSthB();
	CS_STOP_TIMER("label1")

	CS_START_TIMER("label2")
	doSthC();
	CS_STOP_TIMER("label2")


	for (auto i = CODE_SNIPPET::activeTimers.begin(); i != CODE_SNIPPET::activeTimers.end(); ++i) {
		std::cout<<i->first<<" time cost = "<<static_cast<double>(i->second->GetTotalDuration()) / 1000.0<<" ms\n";
	}

	std::cout << "total time cost: "<<static_cast<double>(CODE_SNIPPET::timerManager.GetTotalTime()) / 1000.0 << std::endl;


*/

namespace CODE_SNIPPET {
	// 计时器

	class InstrumentationTimer {
	public:
		InstrumentationTimer()
			: m_Stopped(false) {
			Start();
		}

		void Start() {
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
			m_Stopped = false;
		}

		void Stop() {
			if (!m_Stopped) {
				auto endTimepoint = std::chrono::high_resolution_clock::now();
				long long duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimepoint - m_StartTimepoint).count();
				m_TotalDuration += duration;
				m_Stopped = true;
			}
		}

		long long GetTotalDuration() const {
			return m_TotalDuration;
		}

	private:
		std::chrono::high_resolution_clock::time_point m_StartTimepoint;
		bool m_Stopped;
		long long m_TotalDuration = 0;
	};

	class TimerManager {
	public:
		std::shared_ptr<InstrumentationTimer> GetTimer(const std::string& label) {
			std::lock_guard<std::mutex> lock(m_Mutex);
			if (m_Timers.find(label) == m_Timers.end()) {
				m_Timers[label] = std::make_shared<InstrumentationTimer>();
			}
			return m_Timers[label];
		}

		long long GetTotalTime() const {
			long long total = 0;
			for (const auto& pair : m_Timers) {
				total += pair.second->GetTotalDuration();
			}
			return total;
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<InstrumentationTimer>> m_Timers;
		mutable std::mutex m_Mutex;
	};

	// 全局 TimerManager 实例
	TimerManager timerManager;


	// 用于管理计时器的容器
	std::unordered_map<std::string, std::shared_ptr<InstrumentationTimer>> activeTimers;

}

// 是否启用profiling
//#define CS_CALCU_TIME_COST 1

#if CS_CALCU_TIME_COST
#define CS_START_TIMER(label) \
	::CODE_SNIPPET::activeTimers[label] = ::CODE_SNIPPET::timerManager.GetTimer(label);\
	::CODE_SNIPPET::activeTimers[label]->Start();
#define CS_STOP_TIMER(label) \
	if (::CODE_SNIPPET::activeTimers.find(label) != ::CODE_SNIPPET::activeTimers.end()) { \
		::CODE_SNIPPET::activeTimers[label]->Stop(); \
	}

#else
#define CS_START_TIMER(label) 
#define CS_STOP_TIMER(label) 
#endif

