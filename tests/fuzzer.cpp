#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

#define ENABLE_FORCED_CHECKS
#define ENABLE_INDEV
#define ENABLE_TREE_DUMP

#include "RedBlackTree.h"

template <typename T, typename U>
std::string concat(T a, U b)
{
	std::stringstream ss;
	ss << a << b;
	return ss.str();
}

void dumpOpSequence(const std::string& filename, const std::vector<std::string>& opSequence)
{
	std::ofstream output(filename);
	for (size_t i = 0; i < opSequence.size(); i++)
	{
		output << opSequence[i] << "\n";
	}
	output.close();
}

int main()
{
	RedBlackTree<int64_t> tree;
	std::random_device rd;
	std::mt19937_64 e2(rd());
	std::uniform_int_distribution<int64_t> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));
	std::vector<std::string> opSequence;

	for (size_t j = 0; j < 10000; j++)
	{
		std::cout << "Starting new tree.\n";

		for (size_t i = 0; i < 100000; i++)
		{
			int64_t item = dist(e2);
			if (item % 5 >= 2 || tree.Size() == 0)
			{
				tree.Insert(dist(e2));
				opSequence.push_back(concat("I ", item));
			}
			else
			{
				tree.Delete(item % tree.Size());
				opSequence.push_back(concat("D ", item));
			}

			if ((i + 1) % 10000 == 0)
			{
				std::cout << "Current iteration is " << j + 1 << ". " << i + 1 << " operations done. Tree size is " << tree.Size() << ".\n";
			}

			if ((i + 1) % 10000 == 0)
			{
				std::cout << "Checking tree invariants and content.\n";
				if (FORCE_CHECKS(tree))
				{
					std::cout << "Checks passed.\n";
				}
				else
				{
					std::cout << "Failed check, dump tree and exit!\n";
					DumpTreeToFile("fail.txt", tree);
					dumpOpSequence("op_seq.txt", opSequence);
					return 0;
				}
			}
		}

		tree.Clear();
		opSequence.clear();
	}

}
