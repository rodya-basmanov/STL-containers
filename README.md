# SkipList Container

A C++11 compliant Skip List container implementation that follows the Standard Template Library (STL) style. This implementation provides an ordered associative container with logarithmic complexity for search, insertion, and deletion operations.

## Overview

Skip List is a probabilistic data structure that allows O(log n) search, insertion, and deletion complexity. It's an alternative to balanced trees like Red-Black trees or AVL trees but with simpler implementation. The structure consists of multiple layers of linked lists, where each layer skips a certain number of elements from the layer below, allowing for faster traversal.

This implementation includes:
- Full iterator support (forward iterators)
- Standard container concepts compliance
- Error handling
- Standard operators
- Comprehensive testing

## Requirements

- C++11 compatible compiler
- Google Test (for testing)
- Make

## Building

### Build Main Program

```bash
make
```

The built executable will be in `bin/skiplist`.

### Build and Run Tests

```bash
make test
```

This will build and run all tests using Google Test.

### Clean

```bash
make clean
```

## Container Features

The `SkipList` container implements:

- Standard constructors (default, copy, move)
- Assignment operators (copy, move)
- Element access: `front()`
- Iterators: `begin()`, `end()` and their const variants
- Capacity: `empty()`, `size()`
- Modifiers: `clear()`, `insert()`, `emplace()`, `erase()`, `swap()`
- Lookup: `find()`, `contains()`
- Comparison operators: `==`, `!=`