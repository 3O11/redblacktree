#ifndef _RED_BLACK_TREE_H
#define _RED_BLACK_TREE_H

#include <memory>

//////////////////////////////////////////////////////////////////////////////
// DEBUG PREPARATION
//////////////////////////////////////////////////////////////////////////////

#if defined(RUNTIME_REFERENCE_DATA_STRUCTURE) || defined(ENABLE_FORCED_CHECKS)
#	include <vector>
#	include <algorithm>
#	define PROVIDE_DATA_STRUCTURE
#endif

#if defined(RUNTIME_CONSISTENCY_CHECKS) || defined(ENABLE_FORCED_CHECKS)
#	include <stack>
#	define PROVIDE_INVARIANT_CHECKS
#endif

#if defined(ENABLE_TREE_DUMP)
#	include <fstream>
#	include <string>
#	include <functional>
#endif

#if defined(RUNTIME_REFERENCE_DATA_STRUCTURE) || defined(RUNTIME_CONSISTENCY_CHECKS) || defined(ENABLE_FORCED_CHECKS)
#	include <cstdio>

#	ifdef _MSC_VER // TODO: better way to tell if Visual Studio is in use.
#		define ASSERT(x) if (!x) __debugbreak()
#	else
#		include <cassert>
#		define ASSERT(x) assert(x)
#	endif
#endif

//////////////////////////////////////////////////////////////////////////////
// ACCEPTED TYPE CONSTRAINT DEFINITION
//////////////////////////////////////////////////////////////////////////////

template <typename T>
concept Comparable = requires(T a, T b)
{
	a < b;
	a == b;
};

//////////////////////////////////////////////////////////////////////////////
// RED BLACK TREE DECLARATION
//////////////////////////////////////////////////////////////////////////////

template <Comparable T>
class RedBlackTree
{
private:
	struct Node
	{
		Node(const T& item)
			: Item(item), Black(false), LeftSize(0), Left(nullptr), Right(nullptr)
		{}

		T                          Item;
		bool                       Black;
		size_t                     LeftSize;
		std::unique_ptr<Node>      Left;
		std::unique_ptr<Node>      Right;

		bool IsBlack() const { return Black; }
		bool IsRed()   const { return !Black; }

		bool IsLeftBlack() const { return !Left || Left->IsBlack(); }
		bool IsLeftRed()   const { return Left && Left->IsRed(); }

		bool IsRightBlack() const { return !Right || Right->IsBlack(); }
		bool IsRightRed()   const { return Right && Right->IsRed(); }

		void MoveRedUp()
		{
			if (IsLeftRed() && IsRightRed())
			{
				SwitchColours();
			}
		}

		void SwitchColours()
		{
			if (Left)
			{
				Left->Black = !Left->Black;
			}

			if (Right)
			{
				Right->Black = !Right->Black;
			}

			Black = !Black;
		}

		static void     Fixup        (std::unique_ptr<Node>& node);
		static void     RotateLeft   (std::unique_ptr<Node>& node);
		static void     RotateRight  (std::unique_ptr<Node>& node);
		static void     MoveRedLeft  (std::unique_ptr<Node>& node);
		static void     MoveRedRight (std::unique_ptr<Node>& node);

		static bool     Insert       (std::unique_ptr<Node>& node, const T& item);
		static bool     Delete       (std::unique_ptr<Node>& node, const T& item);
		static bool     DeleteMin    (std::unique_ptr<Node>& node);
		static std::pair<size_t, std::reference_wrapper<const T>> Find (const Node* node, const T& item);
		static const T& At           (const Node* node, size_t index);
		static bool     Contains     (const Node* node, const T& item);

		inline static T s_default;
	};
public:
			 RedBlackTree ();

	bool     Insert       (const T& item);
	bool     Delete       (const T& item);
	bool     DeleteAt     (size_t index);
	void     Clear        ();

	std::pair<size_t, std::reference_wrapper<const T>> Find (const T& item) const;
	const T& At           (size_t index)  const;
	bool     Contains     (const T& item) const;

	bool     Empty        () const;
	size_t   Size         () const;
private:
	std::unique_ptr<Node>       m_root;
	size_t                      m_treeSize;
	T                           m_default;

#ifdef PROVIDE_DATA_STRUCTURE
	bool CheckContent () const;
	std::vector<T> m_reference;
#endif
#ifdef PROVIDE_INVARIANT_CHECKS
	bool CheckInvariants() const;
#endif
#ifdef ENABLE_FORCED_CHECKS
	template <typename U> friend bool ForceCheckInvariants(const RedBlackTree<U>& tree);
	template <typename U> friend bool ForceCheckContent(const RedBlackTree<U>& tree);
#endif
#ifdef ENABLE_TREE_DUMP
	template<typename U> friend void DumpTreeToFile(const std::string& filename, const RedBlackTree<U>& tree);
#endif
};

//////////////////////////////////////////////////////////////////////////////
// REDBLACKTREE::NODE MEMDER FUNCTION DEFINITIONS
//////////////////////////////////////////////////////////////////////////////

template<Comparable T>
inline void RedBlackTree<T>::Node::Fixup (std::unique_ptr<Node>& node)
{
	if (node->IsRightRed() && node->IsLeftBlack())
	{
		RotateLeft(node);
	}

	if (node->IsLeftRed() && node->Left->IsLeftRed())
	{
		RotateRight(node);
	}

	node->MoveRedUp();
}

template<Comparable T>
inline void RedBlackTree<T>::Node::RotateLeft (std::unique_ptr<Node>& node)
{
	std::unique_ptr<Node> newTop = std::move(node->Right);

	// Do the actual rotation
	node->Right = std::move(newTop->Left);
	newTop->Left = std::move(node);

	// Fix colours
	bool colourTemp = newTop->Left->Black;
	newTop->Left->Black = newTop->Black;
	newTop->Black = colourTemp;

	// Fix left-subtree sizes
	// node's left subtree count will stay the same
	// but newTop's leftsubtree size will have to increase
	// by node's left subtree size + 1
	newTop->LeftSize += newTop->Left->LeftSize + 1;

	node = std::move(newTop);
}

template<Comparable T>
inline void RedBlackTree<T>::Node::RotateRight (std::unique_ptr<Node>& node)
{
	std::unique_ptr<Node> newTop = std::move(node->Left);

	// Do the actual rotation
	node->Left = std::move(newTop->Right);
	newTop->Right = std::move(node);

	// Fix colours
	bool colourTemp = newTop->Right->Black;
	newTop->Right->Black = newTop->Black;
	newTop->Black = colourTemp;

	// Fix left-subtree sizes
	// newTop's left subtree size stays the same
	// but node's left subtree size decreases by newTop's leftSize + 1
	newTop->Right->LeftSize -= newTop->LeftSize + 1;

	node = std::move(newTop);
}

template<Comparable T>
inline void RedBlackTree<T>::Node::MoveRedLeft (std::unique_ptr<Node>& node)
{
	node->SwitchColours();
	if (node->Right && node->Right->IsLeftRed())
	{
		RotateRight(node->Right);
		RotateLeft(node);
		node->SwitchColours();
	}
}

template<Comparable T>
inline void RedBlackTree<T>::Node::MoveRedRight (std::unique_ptr<Node>& node)
{
	node->SwitchColours();
	if (node->Left && node->Left->IsLeftRed())
	{
		RotateRight(node);
		node->SwitchColours();
	}
}

template<Comparable T>
inline bool RedBlackTree<T>::Node::Insert (std::unique_ptr<Node>& node, const T& item)
{
	if (node->Item == item)
	{
		return false;
	}

	bool inserted = true;
	if (item < node->Item)
	{
		if (!node->Left)
		{
			node->Left = std::make_unique<Node>(item);
			++node->LeftSize;
		}
		else
		{
			inserted = Insert(node->Left, item);
			node->LeftSize += inserted;
		}
	}
	else
	{
		if (!node->Right)
		{
			node->Right = std::make_unique<Node>(item);
		}
		else
		{
			inserted = Insert(node->Right, item);
		}
	}

	Fixup(node);
	return inserted;
}

template<Comparable T>
inline bool RedBlackTree<T>::Node::Delete (std::unique_ptr<Node>& node, const T& item)
{
	if (!node)
	{
		return false;
	}

	bool deleted = false;
	if (item < node->Item)
	{
		if (node->Left && node->Left->IsBlack() && node->Left->IsLeftBlack())
		{
			MoveRedLeft(node);
		}

		deleted = Delete(node->Left, item);
		node->LeftSize -= static_cast<size_t>(deleted);
	}
	else
	{
		if (node->IsLeftRed())
		{
			RotateRight(node);
		}

		if (node->Item == item && !node->Right)
		{
			node.reset();
			return true;
		}

		if (node->IsRightBlack() && node->Right && node->Right->IsLeftBlack())
		{
			MoveRedRight(node);
		}

		if (node->Item == item) {
			// Find the minimum node of right subtree
			Node* rightMin = node->Right.get();
			while (rightMin->Left) rightMin = rightMin->Left.get();

			// Swap the values of (this subtree) root and the minimum value of the right subtree, then delete minimum of right subtree
			node->Item = rightMin->Item;
			rightMin->Item = item;

			deleted = DeleteMin(node->Right);
		}
		else {
			deleted = Delete(node->Right, item);
		}
	}

	Fixup(node);
	return deleted;
}

template<Comparable T>
inline bool RedBlackTree<T>::Node::DeleteMin (std::unique_ptr<Node>& node)
{
	if (node->IsLeftBlack() && node->Left && node->Left->IsLeftBlack())
	{
		MoveRedLeft(node);
	}

	if (!node->Left)
	{
		node.reset();
		return true;
	}
	else
	{
		DeleteMin(node->Left);
	}

	--node->LeftSize;
	Fixup(node);
	return true;
}

template<Comparable T>
inline std::pair<size_t, std::reference_wrapper<const T>> RedBlackTree<T>::Node::Find (const Node* node, const T& item)
{
	if (!node)
	{
		return std::make_pair((size_t)-1, std::ref(s_default));
	}

	if (item == node->Item)
	{
		return std::make_pair(node->LeftSize, std::ref(node->Item));
	}

	if (item < node->Item)
	{
		return Find(node->Left.get(), item);
	}
	else
	{
		auto foundPair = Find(node->Right.get(), item);
		foundPair.first += node->LeftSize;
		return foundPair;
	}
}

template<Comparable T>
inline const T& RedBlackTree<T>::Node::At (const Node* node, size_t index)
{
	if (!node)
	{
		return s_default;
	}

	if (node->LeftSize == index)
	{
		return node->Item;
	}

	if (index < node->LeftSize)
	{
		return At(node->Left.get(), index);
	}
	else
	{
		return At(node->Right.get(), index - (node->LeftSize + 1));
	}
}

template<Comparable T>
inline bool RedBlackTree<T>::Node::Contains (const Node* node, const T& item)
{
	if (!node)
	{
		return false;
	}

	if (item == node->Item)
	{
		return true;
	}

	if (item < node->Item)
	{
		return Contains(node->Left.get(), item);
	}
	else
	{
		return Contains(node->Right.get(), item);
	}
}

//////////////////////////////////////////////////////////////////////////////
// REDBLACKTREE MEMBER FUNCTION DEFINITIONS
//////////////////////////////////////////////////////////////////////////////

template<Comparable T>
inline RedBlackTree<T>::RedBlackTree()
	: m_root(nullptr), m_treeSize(0), m_default(0)
{}

template<Comparable T>
inline bool RedBlackTree<T>::Insert(const T& item)
{
	bool insertResult = false;
	if (this->Empty())
	{
		m_root = std::make_unique<Node>(item);
		m_treeSize = 1;
		insertResult = true;
	}
	else
	{
		if (Node::Insert(m_root, item))
		{
			m_treeSize++;
			insertResult = true;
		}
	}

#ifdef PROVIDE_DATA_STRUCTURE
	if (!std::count(m_reference.begin(), m_reference.end(), item))
	{
		m_reference.insert(std::upper_bound(m_reference.begin(), m_reference.end(), item), item);
	}
#endif
#ifdef RUNTIME_REFERENCE_DATA_STRUCTURE
	ASSERT(CheckContent());
#endif

	return insertResult;
}

template<Comparable T>
inline bool RedBlackTree<T>::Delete(const T& item)
{
	bool deleteResult = Node::Delete(m_root, item);
	m_treeSize -= deleteResult;

#ifdef PROVIDE_DATA_STRUCTURE
	auto found = std::find(m_reference.begin(), m_reference.end(), item);
	if (found != m_reference.end())
	{
		m_reference.erase(found);
	}
#endif

	return deleteResult;
}

template<Comparable T>
inline bool RedBlackTree<T>::DeleteAt(size_t index)
{
	return Delete(At(index));
}

template<Comparable T>
inline void RedBlackTree<T>::Clear()
{
	m_root = nullptr;
	m_treeSize = 0;

#ifdef PROVIDE_DATA_STRUCTURE
	m_reference.clear();
#endif
}


template<Comparable T>
inline std::pair<size_t, std::reference_wrapper<const T>> RedBlackTree<T>::Find(const T& item) const
{
	return Node::Find(m_root.get(), item);
}

template<Comparable T>
inline const T& RedBlackTree<T>::At(size_t index) const
{
	return Node::At(m_root.get(), index);
}

template<Comparable T>
inline bool RedBlackTree<T>::Contains(const T& item) const
{
	return Node::Contains(m_root.get(), item);
}

template<Comparable T>
inline bool RedBlackTree<T>::Empty() const
{
	return m_treeSize == 0;
}

template<Comparable T>
inline size_t RedBlackTree<T>::Size() const
{
	return m_treeSize;
}

//////////////////////////////////////////////////////////////////////////////
// DEBUG FUNCTION DEFINITIONS
//////////////////////////////////////////////////////////////////////////////

#ifdef PROVIDE_DATA_STRUCTURE
template<typename T>
inline bool RedBlackTree<T>::CheckContent() const
{
	if (m_reference.size() != m_treeSize)
	{
		printf("Tree size different from reference!\n");
		return false;
	}

	for (size_t i = 0; i < m_reference.size(); i++)
	{
		T found = At(i);
		if (m_reference[i] != found)
		{
			printf("Found item not equal to reference at index: %d\n", (int)i);
			return false;
		}
	}

	return true;
}
#endif

#ifdef PROVIDE_INVARIANT_CHECKS
template<typename T>
inline bool RedBlackTree<T>::CheckInvariants() const
{
	if (!m_root)
	{
		return true;
	}

	int64_t blackDepth = -1;
	struct StackFrame
	{
		Node* node = nullptr;
		int        blackDepth = 0;
	};

	std::stack<StackFrame> stack;
	stack.push({ m_root.get(), 0 });

	while (!stack.empty())
	{
		StackFrame currentFrame = stack.top();
		Node* currentNode = currentFrame.node;
		stack.pop();

		// Check two red edges in a row (doesn;t need to hold for root)
		if (!(currentNode == m_root.get()) && currentNode->IsRed() && (currentNode->IsLeftRed() || currentNode->IsRightRed()))
		{
			printf("Found two neighbouring red edges!\n");
			return false;
		}

		// Check that every path from root to 
		if (!currentNode->Left && !currentNode->Right)
		{
			if (blackDepth != -1)
			{
				if (currentFrame.blackDepth != blackDepth)
				{
					printf("Found two paths from root to leaf with different black node counts!\n");
					return false;
				}
			}
			else
			{
				blackDepth = currentFrame.blackDepth;
			}
		}

		// Single red edge must point left
		if (currentNode->IsLeftBlack() && currentNode->IsRightRed())
		{
			printf("Found a single red edge going right!\n");
			return false;
		}

		// Every leaf node is blakc by default, no need to check

		if (currentNode->Left)
		{
			stack.push({ currentNode->Left.get(), currentFrame.blackDepth + (currentNode->IsLeftBlack() ? 1 : 0)});
		}

		if (currentNode->Right)
		{
			stack.push({ currentNode->Right.get(), currentFrame.blackDepth + (currentNode->IsRightBlack() ? 1 : 0)});
		}
	}

	return true;
}
#endif

#ifdef ENABLE_FORCED_CHECKS
template <typename T>
inline bool ForceCheckInvariants(const RedBlackTree<T>& tree)
{
	return tree.CheckInvariants();
}

template <typename T>
inline bool ForceCheckContent(const RedBlackTree<T>& tree)
{
	return tree.CheckContent();
}

#	define FORCE_CHECKS(tree) (ForceCheckInvariants(tree) && ForceCheckContent(tree))

#else
#	define FORCE_CHECKS(tree) 1
#endif

#ifdef ENABLE_TREE_DUMP
template <typename T>
inline void DumpTreeToFile(const std::string& filename, const RedBlackTree<T>& tree)
{
	static const std::function<void(std::ostream&, const typename RedBlackTree<T>::Node*)> dumpHelper =
		[&](std::ostream& output, const typename RedBlackTree<T>::Node* node)
		{
			if (node->Left)
			{
				output << (node->IsLeftRed() ? "edge[color=red];\n" : "edge[color=black];\n");
				output << "\"Value: " << node->Item << "\\n LeftSize: " << node->LeftSize <<"\" -> \"Value: " << node->Left->Item << "\\n LeftSize: " << node->Left->LeftSize << "\"\n";
				dumpHelper(output, node->Left.get());
			}

			if (node->Right)
			{
				output << (node->IsRightRed() ? "edge[color=red];\n" : "edge[color=black];\n");
				output << "\"Value: " << node->Item << "\\n LeftSize: " << node->LeftSize << "\" -> \"Value: " << node->Right->Item << "\\n LeftSize: " << node->Right->LeftSize << "\"\n";
				dumpHelper(output, node->Right.get());
			}
		};

	std::ofstream output{filename};
	output << "digraph G {\n";
	dumpHelper(output, tree.m_root.get());
	output << "}\n";
	output.close();
}
#endif

#endif
