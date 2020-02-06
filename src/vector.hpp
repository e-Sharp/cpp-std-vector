#pragma once

template<class Value, class Allocator = allocator<Value>>
class vector {
public:
	// types
	using value_type             = Value;
	using allocator_type         = Allocator;
	using pointer                = typename allocator_traits<Allocator>::pointer;
	using const_pointer          = typename allocator_traits<Allocator>::const_pointer;
	using reference              = value_type&;
	using const_reference        = const value_type&;
	using size_type              = implementation-defined; // see [container.requirements]
	using difference_type        = implementation-defined; // see [container.requirements]
	using iterator               = implementation-defined; // see [container.requirements]
	using const_iterator         = implementation-defined; // see [container.requirements]
	using reverse_iterator       = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	// [vector.cons], construct/copy/destroy

	vector() noexcept(noexcept(Allocator()))
		: vector(Allocator())
	{}

	explicit
	vector(const Allocator&) noexcept;

	explicit
	vector(size_type n, const Allocator& = Allocator());

	vector(size_type n, const Value& value, const Allocator& = Allocator());

	template<class InputIterator>
	vector(
		InputIterator first, 
		InputIterator last, 
		const Allocator& = Allocator()
	);

	vector(const vector& x);

	vector(vector&&) noexcept;

	vector(const vector&, const Allocator&);

	vector(vector&&, const Allocator&);

	vector(initializer_list<Value>, const Allocator& = Allocator());

	~vector();

	vector& operator=(const vector& x);

	vector& operator=(vector&& x)
	noexcept(
		allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
		allocator_traits<Allocator>::is_always_equal::value
	);

	vector& operator=(initializer_list<Value>);

	template<class InputIterator>
	void assign(InputIterator first, InputIterator last);

	void assign(size_type n, const Value& u);

	void assign(initializer_list<Value>);

	allocator_type get_allocator() const noexcept;

	// iterators

	iterator begin() noexcept;

	const_iterator begin() const noexcept;

	iterator end() noexcept;

	const_iterator end() const noexcept;

	reverse_iterator rbegin() noexcept;

	const_reverse_iterator rbegin() const noexcept;

	reverse_iterator rend() noexcept;

	const_reverse_iterator rend() const noexcept;

	const_iterator cbegin() const noexcept;

	const_iterator cend() const noexcept;

	const_reverse_iterator crbegin() const noexcept;

	const_reverse_iterator crend() const noexcept;

	// [vector.capacity], capacity

	[[nodiscard]]
	bool empty() const noexcept;

	size_type size() const noexcept;

	size_type max_size() const noexcept;

	size_type capacity() const noexcept;

	void resize(size_type sz);

	void resize(size_type sz, const Value& c);

	void reserve(size_type n);

	void shrink_to_fit();

	// element access

	reference operator[](size_type n);

	const_reference operator[](size_type n) const;

	const_reference at(size_type n) const;

	reference at(size_type n);

	reference front();

	const_reference front() const;

	reference back();

	const_reference back() const;

	// [vector.data], data access

	Value* data() noexcept;

	const Value* data() const noexcept;

	// [vector.modifiers], modifiers

	template<class... Args> reference emplace_back(Args&&... args);

	void push_back(const Value& x);

	void push_back(Value&& x);

	void pop_back();

	template<class... Args>
	iterator emplace(const_iterator position, Args&&... args);

	iterator insert(const_iterator position, const Value& x);

	iterator insert(const_iterator position, Value&& x);

	iterator insert(const_iterator position, size_type n, const Value& x);

	template<class InputIterator>
	iterator insert(
		const_iterator position,
		InputIterator first,
		InputIterator last
	);

	iterator insert(const_iterator position, initializer_list<Value> il);

	iterator erase(const_iterator position);

	iterator erase(const_iterator first, const_iterator last);

	void swap(vector&)
	noexcept(
		allocator_traits<Allocator>::propagate_on_container_swap::value ||
		allocator_traits<Allocator>::is_always_equal::value
	);

	void clear() noexcept;
};

template<class InputIterator, class Allocator = allocator<iter-value-type<InputIterator>>>
vector(InputIterator, InputIterator, Allocator = Allocator())
-> vector<iter-value-type<InputIterator>, Allocator>;

// swap

template<class Value, class Allocator>
void swap(vector<Value, Allocator>& x, vector<Value, Allocator>& y)
noexcept(noexcept(x.swap(y)));
