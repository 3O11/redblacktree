# RedBlackTree

This is an implementation of a Red-Black Tree provided as a single include
"library".

## Installation

As this is a CMake project, installation should be as easy as putting this
directory into your project tree and adding a single line

```cmake
add_subdirectory(<path-to>/redblacktree)
```

Finally, to be able to include the data structure in your sources, you need to
add the library to the correct target in your cmake project.

```cmake
target_link_libraries(<your-project> RedBlackTree)
```

## Usage

All that is necessary to have full access to all functionality is this line:

```cpp
#include <RedBlackTree.h>
```

This header contains the full implementation of the data structure.
The public interface looks like this:

```cpp
template <typename T>
class RedBlackTree
{
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
};
```

#### Insert

Adds an element to the tree, if there already is one, the insertion is ignored.
Returns `true` if item was inserted and `false` otherwise.

#### Delete

Deleted the element from the data structure, provided it is contained within.
Returns `true` if item was deleted and `false` otherwise.

#### DeleteAt

Deletes the `k`-th element from the data-structure, provided it is in bounds.
Returns `true` if item was inserted and `false` otherwise.

#### Clear

Completely empties the data structure.

#### Find

Tries to find the specified element. Returns a pair of an index and a `const &`
to it. If the item isn't contained, returns the max value of `size_t` and a reference
to a default constructed item.

#### At

Returns the element contained at the provided index, or the default value, if
the index is out of bounds.

#### Contains

Returns `true` if the element is contained, or `false` if it isn't.

#### Empty

Returns `true` if the tree is empty.

#### Size

Returns the count of the elements contained in the tree.

## Additional debug options

There are also some tools provided for debugging. They can be enabled with
these flags:

#### ENABLE_FORCED_CHECKS

Gives access to the `FORCE_CHECKS()` macro, which executes consistency checks
on the tree provided as parameter. In case it's necessary to check either only
invariant consistency or internal data consistency, those checks are available
as these functions:

```cpp
bool ForceCheckInvariants(const RedBlackTree<T>& tree);
bool ForceCheckContent(const RedBlackTree<T>& tree);
```

#### ENABLE_TREE_DUMP

Gives access to the `DumpTreeToFile(filename, tree)` function, which serializes
the tree into a format which can be visualized with e.g.
https://dreampuf.github.io/GraphvizOnline/

There are a few more flags, which are not recommended for use, either because
they slow the program down way too much, or because they are not completely
finished.
