#include <random>
#include <string>
#include <gtest/gtest.h>

#define ENABLE_FORCED_CHECKS
#include "RedBlackTree.h"

TEST(RedBlackTree, InsertIncreasingSmall)
{
	RedBlackTree<int64_t> tree;

	for (size_t i = 0; i < 1000; ++i)
	{
		tree.Insert(i);

		if (tree.Size() % 100 == 0)
		{
			EXPECT_EQ(1, FORCE_CHECKS(tree));
		}
	}

	EXPECT_EQ(1, FORCE_CHECKS(tree));
}

TEST(RedBlackTree, InsertDecreasingSmall)
{
	RedBlackTree<int64_t> tree;

	for (size_t i = 1000; i > 0; --i)
	{
		tree.Insert(i);

		if (tree.Size() % 100 == 0)
		{
			EXPECT_EQ(1, FORCE_CHECKS(tree));
		}
	}

	EXPECT_EQ(1, FORCE_CHECKS(tree));
}

TEST(RedBlackTree, InsertIncreasingLarge)
{
	RedBlackTree<int64_t> tree;

	for (size_t i = 0; i < 100000; ++i)
	{
		tree.Insert(i);

		if (tree.Size() % 10000 == 0)
		{
			EXPECT_EQ(1, FORCE_CHECKS(tree));
		}
	}

	EXPECT_EQ(1, FORCE_CHECKS(tree));
}

TEST(RedBlackTree, InsertStatusReturn)
{
	RedBlackTree<int64_t> tree;

	for (size_t i = 0; i < 100000; ++i)
	{
		EXPECT_EQ(1, tree.Insert(i));
		EXPECT_NE(1, tree.Insert(i));
	}

	EXPECT_EQ(1, FORCE_CHECKS(tree));
}

TEST(RedBlackTree, InsertDecreasingLarge)
{
	RedBlackTree<int64_t> tree;

	for (size_t i = 100000; i > 0; --i)
	{
		tree.Insert(i);

		if (tree.Size() % 10000 == 0)
		{
			EXPECT_EQ(1, FORCE_CHECKS(tree));
		}
	}

	EXPECT_EQ(1, FORCE_CHECKS(tree));
}

TEST(RedBlackTree, FuzzyInsert)
{
	RedBlackTree<int64_t> tree;
	std::random_device rd;
	std::mt19937_64 e2(rd());
	std::uniform_int_distribution<int64_t> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));


	for (size_t i = 0; i < 100000; i++)
	{
		tree.Insert(dist(e2));

		if (tree.Size() % 10000 == 0)
		{
			EXPECT_EQ(1, FORCE_CHECKS(tree));
		}
	}
}

TEST(RedBlackTree, FuzzyContains)
{
	RedBlackTree<int64_t> tree;
	std::random_device rd;
	std::mt19937_64 e2(rd());
	std::uniform_int_distribution<int64_t> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));
	for (size_t i = 0; i < 100000; i++) tree.Insert(dist(e2));

	for (size_t i = 0; i < tree.Size(); i++)
	{
		EXPECT_EQ(1, tree.Contains(tree.At(i)));
	}
}

TEST(RedBlackTree, FuzzyFindAndSize)
{
	std::vector<int64_t> items;
	RedBlackTree<int64_t> tree;
	std::random_device rd;
	std::mt19937_64 e2(rd());
	std::uniform_int_distribution<int64_t> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));
	for (size_t i = 0; i < 100000; i++)
	{
		auto item = dist(e2);
		tree.Insert(item);
		items.push_back(item);
		EXPECT_EQ(items.size(), tree.Size());
	}

	for (size_t i = 0; i < tree.Size(); i++)
	{
		EXPECT_NE((size_t)-1, tree.Find(items[i]).first);
		EXPECT_EQ(items[i], tree.Find(items[i]).second);
	}
}

TEST(RedBlackTree, DeleteIncreasing)
{
	RedBlackTree<int64_t> tree;
	for (size_t i = 0; i < 100000; i++)
	{
		tree.Insert(i);
	}

	for (size_t i = 0; i < 100000; i++)
	{
		tree.Delete(i);

		if (i % 10000 == 0)
		{
			EXPECT_EQ(1, FORCE_CHECKS(tree));
		}
	}
}

TEST(RedBlackTree, DeleteDecreasingAndOutOfBounds)
{
	RedBlackTree<int64_t> tree;
	for (size_t i = 1; i < 100000; i++)
	{
		tree.Insert(i);
	}

	for (size_t i = 200000; i > 0; --i)
	{
		tree.Delete(i);

		if (i % 10000 == 0)
		{
			EXPECT_EQ(1, FORCE_CHECKS(tree));
		}
	}

	EXPECT_EQ(0, tree.Size());
}

TEST(RedBlackTree, FuzzyDelete)
{
	RedBlackTree<int64_t> tree;
	std::random_device rd;
	std::mt19937_64 e2(rd());
	std::uniform_int_distribution<int64_t> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));
	for (size_t i = 0; i < 100000; i++)
	{
		auto item = dist(e2);
		tree.Insert(item);
	}

	for (size_t i = 0; i < 100000; i++)
	{
		tree.Delete(tree.At(dist(e2)) % tree.Size());

		if (i % 10000 == 0)
		{
			EXPECT_EQ(1, FORCE_CHECKS(tree));
		}
	}
}

TEST(RedBlackTree, FuzzyInsertDelete)
{
	RedBlackTree<int64_t> tree;
	std::random_device rd;
	std::mt19937_64 e2(rd());
	std::uniform_int_distribution<int64_t> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));

	for (size_t i = 0; i < 100000; i++)
	{
		tree.Insert(dist(e2));
		tree.Delete(dist(e2));

		if (i % 10000 == 0)
		{
			EXPECT_EQ(1, FORCE_CHECKS(tree));
		}
	}
}
