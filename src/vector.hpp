#pragma once

template<class T, class Allocator = allocator<T>>
class vector {
public:
	// types
	using value_type             = T;
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
	constexpr vector() noexcept(noexcept(Allocator())) : vector(Allocator()) { }
	constexpr explicit vector(const Allocator&) noexcept;
	constexpr explicit vector(size_type n, const Allocator& = Allocator());
	constexpr vector(size_type n, const T& value, const Allocator& = Allocator());
	template<class InputIterator>
	  constexpr vector(InputIterator first, InputIterator last, const Allocator& = Allocator());
	constexpr vector(const vector& x);
	constexpr vector(vector&&) noexcept;
	constexpr vector(const vector&, const Allocator&);
	constexpr vector(vector&&, const Allocator&);
	constexpr vector(initializer_list<T>, const Allocator& = Allocator());
	constexpr ~vector();
	constexpr vector& operator=(const vector& x);
	constexpr vector& operator=(vector&& x)
	  noexcept(allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
			   allocator_traits<Allocator>::is_always_equal::value);
	constexpr vector& operator=(initializer_list<T>);
	template<class InputIterator>
	  constexpr void assign(InputIterator first, InputIterator last);
	constexpr void assign(size_type n, const T& u);
	constexpr void assign(initializer_list<T>);
	constexpr allocator_type get_allocator() const noexcept;

	// iterators
	constexpr iterator               begin() noexcept;
	constexpr const_iterator         begin() const noexcept;
	constexpr iterator               end() noexcept;
	constexpr const_iterator         end() const noexcept;
	constexpr reverse_iterator       rbegin() noexcept;
	constexpr const_reverse_iterator rbegin() const noexcept;
	constexpr reverse_iterator       rend() noexcept;
	constexpr const_reverse_iterator rend() const noexcept;

	constexpr const_iterator         cbegin() const noexcept;
	constexpr const_iterator         cend() const noexcept;
	constexpr const_reverse_iterator crbegin() const noexcept;
	constexpr const_reverse_iterator crend() const noexcept;

	// [vector.capacity], capacity
	[[nodiscard]] constexpr bool empty() const noexcept;
	constexpr size_type size() const noexcept;
	constexpr size_type max_size() const noexcept;
	constexpr size_type capacity() const noexcept;
	constexpr void      resize(size_type sz);
	constexpr void      resize(size_type sz, const T& c);
	constexpr void      reserve(size_type n);
	constexpr void      shrink_to_fit();

	// element access
	constexpr reference       operator[](size_type n);
	constexpr const_reference operator[](size_type n) const;
	constexpr const_reference at(size_type n) const;
	constexpr reference       at(size_type n);
	constexpr reference       front();
	constexpr const_reference front() const;
	constexpr reference       back();
	constexpr const_reference back() const;

	// [vector.data], data access
	constexpr T*       data() noexcept;
	constexpr const T* data() const noexcept;

	// [vector.modifiers], modifiers
	template<class... Args> constexpr reference emplace_back(Args&&... args);
	constexpr void push_back(const T& x);
	constexpr void push_back(T&& x);
	constexpr void pop_back();

	template<class... Args> constexpr iterator emplace(const_iterator position, Args&&... args);
	constexpr iterator insert(const_iterator position, const T& x);
	constexpr iterator insert(const_iterator position, T&& x);
	constexpr iterator insert(const_iterator position, size_type n, const T& x);
	template<class InputIterator>
	  constexpr iterator insert(const_iterator position,
								InputIterator first, InputIterator last);
	constexpr iterator insert(const_iterator position, initializer_list<T> il);
	constexpr iterator erase(const_iterator position);
	constexpr iterator erase(const_iterator first, const_iterator last);
	constexpr void     swap(vector&)
	  noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value ||
			   allocator_traits<Allocator>::is_always_equal::value);
	constexpr void     clear() noexcept;
};

template<class InputIterator, class Allocator = allocator<iter-value-type<InputIterator>>>
vector(InputIterator, InputIterator, Allocator = Allocator())
-> vector<iter-value-type<InputIterator>, Allocator>;

// swap
template<class T, class Allocator>
constexpr void swap(vector<T, Allocator>& x, vector<T, Allocator>& y)
noexcept(noexcept(x.swap(y)));
