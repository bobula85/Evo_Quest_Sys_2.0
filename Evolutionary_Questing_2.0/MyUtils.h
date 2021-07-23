#pragma once
#include <chrono>
#include <iostream>
#include <fstream>
#include <algorithm>

struct TimeProfileResult
{
	std::string Name;
	long long Start, End;
};

struct TimeProfilingSession
{
	std::string Name;
};

class TimeProfiler
{
private:
	TimeProfilingSession* m_CurrentSession;
	std::ofstream m_OutputStream;
	int m_ProfileCount;

public:
	TimeProfiler()
		: m_CurrentSession(nullptr), m_ProfileCount(0)
	{
	}

	void TimeBeginSession(const std::string& name, const std::string& filePath = "TimeProfileResult.json")
	{
		m_OutputStream.open(filePath);
		TimeWriteHeader();
		m_CurrentSession = new TimeProfilingSession{ name };
	}

	void TimeWriteResult(const TimeProfileResult& result)
	{
		if (m_ProfileCount++ > 0)
			m_OutputStream << ",";

		std::string name = result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputStream << "{";
		m_OutputStream << "\"cat\":\"function\",";
		m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
		m_OutputStream << "\"name\":\"" << name << "\",";
		m_OutputStream << "\"ph\":\"X\",";
		m_OutputStream << "\"pid\":0,";
		m_OutputStream << "\"tid\":0,";
		m_OutputStream << "\"ts\":" << result.Start;
		m_OutputStream << "}";

		m_OutputStream.flush();
	}

	void TimeEndSession()
	{
		TimeWriteFooter();
		m_OutputStream.close();
		delete m_CurrentSession;
		m_CurrentSession = nullptr;
		m_ProfileCount = 0;
	}

	void TimeWriteHeader()
	{
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_OutputStream.flush();
	}

	void TimeWriteFooter()
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}

	static TimeProfiler& Get()
	{
		static TimeProfiler* instance = new TimeProfiler();

		return *instance;
	}
};





class Timer
{
public:
	Timer(const char* name)
		: m_Name(name),  m_Stopped(false)
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if(!m_Stopped)
			stop();
	}

	void stop()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		auto duration = end - start; 
		double ms = duration * 0.001; 

		TimeProfiler::Get().TimeWriteResult({ m_Name, start, end });

		m_Stopped = false;
	}

private:
	const char* m_Name;
	bool m_Stopped; 

	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;

};