#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>

#include "RedBlackTree.h"

class GlobalStopwatch
{
public:
	GlobalStopwatch(const std::string& funcName)
	{
		m_start = std::chrono::high_resolution_clock::now();
		m_funcName = funcName;
	}

	~GlobalStopwatch()
	{
		s_times[m_funcName] += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_start).count() / 1000.0;
	}

	static double TotalDuration(const std::string& funcName)
	{
		return s_times[funcName];
	}

	static inline std::unordered_map<std::string, double> s_times{};

private:
	std::chrono::high_resolution_clock::time_point m_start;
	std::string m_funcName;
};

#define STOPWATCH(x) GlobalStopwatch __x__(x)

int main()
{
	size_t sampleSize = 100000;
	size_t sampleAverage = 100;
	int64_t sth = 0;

	for (size_t iter = 0; iter < sampleAverage; ++iter)
	{
		std::random_device rd;
		std::mt19937_64 e2(rd());
		std::uniform_int_distribution<int64_t> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));

		std::vector<int64_t> nums;
		for (size_t i = 0; i < sampleSize; i++)
		{
			nums.push_back(dist(e2));
		}

		RedBlackTree<int64_t> tree;
		{
			STOPWATCH("RedBlackTree<int64_t>.Insert()");
			for (auto num : nums) tree.Insert(num);
		}
		std::set<int64_t> ref;
		{
			STOPWATCH("std::set<int64_t>.insert()");
			for (auto num : nums) ref.insert(num);
		}

		std::shuffle(nums.begin(), nums.end(), std::default_random_engine{ rd() });

		{
			STOPWATCH("RedBlackTree<int64_t>.Find()");
			for (auto num : nums) tree.Find(num);
		}
		{
			STOPWATCH("std::set<int64_t>.find()");
			for (auto num : nums) { auto x = ref.find(num); if (x != ref.end()) sth = *x; }
		}

		std::shuffle(nums.begin(), nums.end(), std::default_random_engine{ rd() });

		{
			STOPWATCH("RedBlackTree<int64_t>.Delete()");
			for (auto num : nums) tree.Delete(num);
		}
		{
			STOPWATCH("std::set<int64_t>.erase()");
			for (auto num : nums) ref.erase(num);
		}
	}

	std::cout << "Sample size " << sampleSize << " and " << sampleAverage << " repetitions.\n";
	std::vector<std::pair<std::string, double>> measured{ GlobalStopwatch::s_times.begin(), GlobalStopwatch::s_times.end() };
	std::sort(measured.begin(), measured.end());
	for (auto&&[name, time] : measured)
	{
		std::cout << name << std::string((sth % 2 + 34) - name.size(), ' ') << " took " << time / sampleAverage << "ms on average.\n";
	}
}
