#pragma once

#include <set>
#include <mutex>
#include <memory>
#include <functional>
#include <initializer_list>

// TODO: Thoroughly test Atomic Set and its iterators
/**************************************************************
 * Atomic Set                                                 *
 * Warning: This set may be atomic, but its contents are not! *
 **************************************************************/
template <class T, class Compare = std::less<T>, class Alloc = std::allocator<T> >
class AtomicSet
{
private:
	std::mutex* dataLock;
	std::set<T, Compare, Alloc> data;
public:
	// Typedefs
	typedef typename std::set<T, Compare, Alloc>::key_compare key_compare;
	typedef typename std::set<T, Compare, Alloc>::value_compare value_compare;
	typedef typename std::set<T, Compare, Alloc>::allocator_type allocator_type;
	typedef typename std::set<T, Compare, Alloc>::iterator set_it;
	typedef typename std::set<T, Compare, Alloc>::reverse_iterator set_rit;
	typedef typename std::set<T, Compare, Alloc>::const_iterator set_cit;
	typedef typename std::set<T, Compare, Alloc>::const_reverse_iterator set_crit;

	// Default constructor
	explicit AtomicSet<T, Compare, Alloc>(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : dataLock(new std::mutex()), data(comp, alloc) {}
	explicit AtomicSet<T, Compare, Alloc>(const allocator_type& alloc) : dataLock(new std::mutex()), data(alloc) {}

	// Copy constructor
	AtomicSet<T, Compare, Alloc>(const AtomicSet<T, Compare, Alloc>& rhs) : dataLock(new std::mutex()), data(rhs.data) {}
	AtomicSet<T, Compare, Alloc>(const AtomicSet<T, Compare, Alloc>& rhs, const allocator_type& alloc) : dataLock(new std::mutex()), data(rhs.data, alloc) {}

	// Range constructor
	template <class InputIterator>
	AtomicSet<T, Compare, Alloc>(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : dataLock(new std::mutex()), data(first.it, last.it, comp, alloc) {}

	// Move constructor
	AtomicSet<T, Compare, Alloc>(const AtomicSet<T, Compare, Alloc>&& rhs) : dataLock(std::move(rhs.dataLock)), data(std::move(rhs.data)) { rhs.dataLock = NULL; }
	AtomicSet<T, Compare, Alloc>(const AtomicSet<T, Compare, Alloc>&& rhs, const allocator_type& alloc) : dataLock(std::move(rhs.dataLock)), data(std::move(rhs.data), alloc) { rhs.dataLock = NULL; }

	// Initializer list constructor
	AtomicSet<T, Compare, Alloc>(std::initializer_list<T> il, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : dataLock(new std::mutex()), data(il, comp, alloc) {}

	// Destructor
	~AtomicSet<T, Compare, Alloc>() { if (dataLock) delete dataLock; }

	// Return the comparison objects
	key_compare key_comp() const { return data.key_comp(); }
	value_compare value_comp() const { return data.value_comp(); }

	// Return the allocator
	allocator_type get_allocator() const { return data.get_allocator(); }

	// Return whether the set is empty or not
	bool empty() const {
		std::lock_guard<std::mutex> lock(*dataLock);
		return data.empty();
	}

	// Return the size of the set
	size_t size() const {
		std::lock_guard<std::mutex> lock(*dataLock);
		return data.size();
	}

	// Return the maximum size of the set
	size_t max_size() const {
		std::lock_guard<std::mutex> lock(*dataLock);
		return data.max_size();
	}

	// Erase an object from the set
	size_t erase(const T& val) {
		std::lock_guard<std::mutex> lock(*dataLock);
		return data.erase(val);
	}

	// Insert an initializer list into the set
	void insert(std::initializer_list<T> il) {
		std::lock_guard<std::mutex> lock(*dataLock);
		data.insert(il);
	}

	// Clear the set
	void clear() {
		std::lock_guard<std::mutex> lock(*dataLock);
		data.clear();
	}

	// Swap the contents with another set atomically
	void swap(AtomicSet<T, Compare, Alloc>& rhs) {
		std::lock_guard<std::mutex> lock1(*dataLock);
		std::lock_guard<std::mutex> lock2(rhs.*dataLock);
		data.swap(rhs.data);
	}

	// Count the number of times an element is found in the set (either 0 or 1)
	size_t count(const T& val) {
		std::lock_guard<std::mutex> lock(*dataLock);
		return data.count(val);
	}

	/**************************
	 * Atomic Set :: Iterator *
	 **************************/
	class iterator
	{
	private:
		set_it it;
		std::mutex* dataLock;

		// Constructor from set iterator
		iterator(set_it& rhs, std::mutex* dataLock) : it(rhs), dataLock(dataLock) {}
	public:
		// Constructors
		iterator() : it(), dataLock(NULL) {}
		iterator(const iterator& rhs) : it(rhs.it), dataLock(rhs.dataLock) {}

		// Increment / Decrement
		iterator& operator++()   { ++it; return *this; }
		iterator operator++(int) { return iterator(it++, dataLock); }
		iterator& operator--()   { --it; return *this; }
		iterator operator--(int) { return iterator(it--, dataLock); }

		// Assignment operator
		iterator& operator=(const iterator& rhs) { it = rhs.it; return *this; }

		// Comparison
		bool operator==(const iterator& rhs) const { return it == rhs.it; }
		bool operator!=(const iterator& rhs) const { return it != rhs.it; }

		// Dereference (doesn't allow modifying values)
		const T& operator*() {
			if (dataLock)
				std::lock_guard<std::mutex> lock(*dataLock);
			return *it;
		}

		// Structure Dereference (doesn't allow modifying values)
		const T* operator->() {
			if (dataLock)
				std::lock_guard<std::mutex> lock(*dataLock);
			return it.operator->();
		}

		// The Atomic Set is our friend!
		friend AtomicSet<T, Compare, Alloc>;
	};

	/***********************************
	 * Atomic Set :: Constant Iterator *
	 ***********************************/
	class const_iterator
	{
	private:
		set_cit it;
		std::mutex* dataLock;

		// Constructor from set iterator
		const_iterator(set_cit& rhs, std::mutex* dataLock) : it(rhs), dataLock(dataLock) {}
	public:
		// Constructors
		const_iterator() : it(), dataLock(NULL) {}
		const_iterator(const const_iterator& rhs) : it(rhs.it), dataLock(rhs.dataLock) {}

		// Increment / Decrement
		const_iterator& operator++()   { ++it; return *this; }
		const_iterator operator++(int) { return const_iterator(it++, dataLock); }
		const_iterator& operator--()   { --it; return *this; }
		const_iterator operator--(int) { return const_iterator(it--, dataLock); }

		// Assignment operator
		const_iterator& operator=(const const_iterator& rhs) { it = rhs.it; return *this; }

		// Comparison
		bool operator==(const const_iterator& rhs) const { return it == rhs.it; }
		bool operator!=(const const_iterator& rhs) const { return it != rhs.it; }

		// Dereference (doesn't allow modifying values)
		const T& operator*() {
			if (dataLock)
				std::lock_guard<std::mutex> lock(*dataLock);
			return *it;
		}

		// Structure Dereference (doesn't allow modifying values)
		const T* operator->() {
			if (dataLock)
				std::lock_guard<std::mutex> lock(*dataLock);
			return it.operator->();
		}

		// The Atomic Set is our friend!
		friend AtomicSet<T, Compare, Alloc>;
	};

	/**********************************
	 * Atomic Set :: Reverse Iterator *
	 **********************************/
	class reverse_iterator
	{
	private:
		set_rit it;
		std::mutex* dataLock;

		// Constructor from set iterator
		reverse_iterator(set_rit& rhs, std::mutex* dataLock) : it(rhs), dataLock(dataLock) {}
	public:
		// Constructors
		reverse_iterator() : it(), dataLock(NULL) {}
		reverse_iterator(const reverse_iterator& rhs) : it(rhs.it), dataLock(rhs.dataLock) {}

		// Increment / Decrement
		reverse_iterator& operator++()   { ++it; return *this; }
		reverse_iterator operator++(int) { return reverse_iterator(it++, dataLock); }
		reverse_iterator& operator--()   { --it; return *this; }
		reverse_iterator operator--(int) { return reverse_iterator(it--, dataLock); }

		// Assignment operator
		reverse_iterator& operator=(const reverse_iterator& rhs) { it = rhs.it; return *this; }

		// Comparison
		bool operator==(const reverse_iterator& rhs) const { return it == rhs.it; }
		bool operator!=(const reverse_iterator& rhs) const { return it != rhs.it; }

		// Dereference (doesn't allow modifying values)
		const T& operator*() {
			if (dataLock)
				std::lock_guard<std::mutex> lock(*dataLock);
			return *it;
		}

		// Structure Dereference (doesn't allow modifying values)
		const T* operator->() {
			if (dataLock)
				std::lock_guard<std::mutex> lock(*dataLock);
			return it.operator->();
		}

		// The Atomic Set is our friend!
		friend AtomicSet<T, Compare, Alloc>;
	};

	/*******************************************
	 * Atomic Set :: Constant Reverse Iterator *
	 *******************************************/
	class const_reverse_iterator
	{
	private:
		set_crit it;
		std::mutex* dataLock;

		// Constructor from set iterator
		const_reverse_iterator(set_crit& rhs, std::mutex* dataLock) : it(rhs), dataLock(dataLock) {}
	public:
		// Constructors
		const_reverse_iterator() : it(), dataLock(NULL) {}
		const_reverse_iterator(const const_reverse_iterator& rhs) : it(rhs.it), dataLock(rhs.dataLock) {}

		// Increment / Decrement
		const_reverse_iterator& operator++()   { ++it; return *this; }
		const_reverse_iterator operator++(int) { return const_reverse_iterator(it++, dataLock); }
		const_reverse_iterator& operator--()   { --it; return *this; }
		const_reverse_iterator operator--(int) { return const_reverse_iterator(it--, dataLock); }

		// Assignment operator
		const_reverse_iterator& operator=(const const_reverse_iterator& rhs) { it = rhs.it; return *this; }

		// Comparison
		bool operator==(const const_reverse_iterator& rhs) const { return it == rhs.it; }
		bool operator!=(const const_reverse_iterator& rhs) const { return it != rhs.it; }

		// Dereference (doesn't allow modifying values)
		const T& operator*() {
			if (dataLock)
				std::lock_guard<std::mutex> lock(*dataLock);
			return *it;
		}

		// Structure Dereference (doesn't allow modifying values)
		const T* operator->() {
			if (dataLock)
				std::lock_guard<std::mutex> lock(*dataLock);
			return it.operator->();
		}

		// The Atomic Set is our friend!
		friend AtomicSet<T, Compare, Alloc>;
	};

	// Return an iterator
	iterator begin() { return iterator(data.begin(), dataLock); }
	iterator end() { return iterator(data.end(), dataLock); }
	const_iterator cbegin() { return const_iterator(data.cbegin(), dataLock); }
	const_iterator cend() { return const_iterator(data.cend(), dataLock); }
	reverse_iterator rbegin() { return reverse_iterator(data.rbegin(), dataLock); }
	reverse_iterator rend() { return reverse_iterator(data.rend(), dataLock); }
	const_reverse_iterator crbegin() { return const_reverse_iterator(data.crbegin(), dataLock); }
	const_reverse_iterator crend() { return const_reverse_iterator(data.crend(), dataLock); }

	// Find a constant element
	const_iterator find(const T& val) const 
	{
		std::lock_guard<std::mutex> lock(*dataLock);
		return const_iterator(data.find(val), dataLock);
	}

	// Find an element
	iterator find(const T& val)
	{
		std::lock_guard<std::mutex> lock(*dataLock);
		return iterator(data.find(val), dataLock);
	}

	// Insert an element
	std::pair<iterator, bool> insert(const T& val) {
		std::lock_guard<std::mutex> lock(*dataLock);
		std::pair<set_it, bool> temp = data.insert(val);
		return std::make_pair(iterator(temp.first, dataLock), temp.second);
	}

	// Insert an element by moving it
	std::pair<iterator, bool> insert(T&& val) {
		std::lock_guard<std::mutex> lock(*dataLock);
		std::pair<set_it, bool> temp = data.insert(std::move(val));
		return std::make_pair(iterator(temp.first, dataLock), temp.second);
	}

	// Insert an element with a hint
	iterator insert(const_iterator& position, const T& val) {
		std::lock_guard<std::mutex> lock(*dataLock);
		return iterator(data.insert(position.it, val), dataLock);
	}

	// Insert an element with a hint by moving it
	iterator insert(const_iterator& position, T&& val) {
		std::lock_guard<std::mutex> lock(*dataLock);
		return iterator(data.insert(position.it, std::move(val)), dataLock);
	}

	// Insert a range of elements
	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		std::lock_guard<std::mutex> lock(*dataLock);
		data.insert(first.it, last.it);
	}

	// Erase an element an iterator is pointing to
	iterator erase(const_iterator position) {
		std::lock_guard<std::mutex> lock(*dataLock);
		return iterator(data.erase(position.it), dataLock);
	}

	// Erase a range of elements
	iterator erase(const_iterator first, const_iterator last) {
		std::lock_guard<std::mutex> lock(*dataLock);
		return iterator(data.erase(first.it, last.it), dataLock);
	}

	// Emplace a bunch of elements
	template <class... Args>
	std::pair<iterator, bool> emplace(Args&&... args) {
		std::lock_guard<std::mutex> lock(*dataLock);
		std::pair<set_it, bool> temp = data.emplace(args);
		return std::make_pair(iterator(temp.first, dataLock), temp.second);
	}

	// Emplace a bunch of elements, giving a hint
	template <class... Args>
	iterator emplace_hint(const_iterator position, Args&&... args) {
		std::lock_guard<std::mutex> lock(*dataLock);
		return iterator(data.emplace_hint(position.it, args), dataLock);
	}
};
