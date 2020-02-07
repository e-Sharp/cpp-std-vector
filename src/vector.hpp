#pragma once

#include<memory>
#include<stdexcept>

template<class Value, class Allocator = std::allocator<Value>>
class vector {
public:

	// types

	using value_type = Value;
	using allocator_type = Allocator;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = typename std::allocator_traits<Allocator>::size_type;
	using difference_type = typename std::allocator_traits<Allocator>::difference_type;
	using iterator = Value*;
	using const_iterator = const Value*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	// [vector.cons], construct/copy/destroy

	vector() noexcept(noexcept(Allocator()))
		: vector(Allocator())
	{}

	explicit
	vector(const Allocator&) noexcept
	{}

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

	vector(std::initializer_list<Value>, const Allocator& = Allocator());

	~vector()
	{}

	auto operator=(const vector& x) -> vector&;

	auto operator=(vector&& x) noexcept(
		allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
		allocator_traits<Allocator>::is_always_equal::value
	)  -> vector&;

	auto operator=(std::initializer_list<Value>) -> vector&;

	template<class InputIterator>
	auto assign(InputIterator first, InputIterator last) -> void;

	auto assign(size_type n, const Value& u) -> void;

	auto assign(std::initializer_list<Value>) -> void;

	auto get_allocator() const noexcept -> allocator_type;

	// iterators

	auto begin() noexcept -> iterator {
		return data();
	}

	auto begin() const noexcept -> const_iterator {
		auto mutable_this = const_cast<vector*>(this);
		return mutable_this->begin();
	}

	auto end() noexcept -> iterator {
		return data() + size();
	}

	auto end() const noexcept -> const_iterator {
		auto mutable_this = const_cast<vector*>(this);
		return mutable_this->end();
	}

	auto rbegin() noexcept -> reverse_iterator {
		return data() + size() - 1;
	}

	auto rbegin() const noexcept -> const_reverse_iterator {
		auto mutable_this = const_cast<vector*>(this);
		return mutable_this->rbegin();
	}

	auto rend() noexcept -> reverse_iterator {
		return data() - 1;
	}

	auto rend() const noexcept -> const_reverse_iterator {
		auto mutable_this = const_cast<vector*>(this);
		return mutable_this->rend();
	}

	auto cbegin() const noexcept -> const_iterator {
		return begin();
	}

	auto cend() const noexcept -> const_iterator {
		return end();
	}

	auto crbegin() const noexcept -> const_reverse_iterator {
		return rbegin();
	}

	auto crend() const noexcept -> const_reverse_iterator {
		return rend();
	}

	// [vector.capacity], capacity

	[[nodiscard]]
	auto empty() const noexcept -> bool {
		return size_ == 0;
	}

	auto size() const noexcept -> size_type {
		return size_;
	}

	auto max_size() const noexcept -> size_type {
		return std::allocator_traits<Allocator>::max_size();
	}

	auto capacity() const noexcept -> size_type {
		return capacity_;
	}

	auto resize(size_type sz) -> void {

	}

	auto resize(size_type sz, const Value& c) -> void;

	auto reserve(size_type new_capacity) -> void {
		if(new_capacity > max_size())
			throw std::length_error{"vector::reserve : new_capacity > max_size()"};
	}

	auto shrink_to_fit() -> void;

	// element access

	auto operator[](size_type n) -> reference;

	auto operator[](size_type n) const -> const_reference;

	auto at(size_type n) const -> const_reference;

	auto at(size_type n) -> reference;

	auto front() -> reference;

	auto front() const -> const_reference;

	auto back() -> reference;

	auto back() const -> const_reference;

	// [vector.data], data access

	auto data() noexcept -> Value*;

	auto data() const noexcept -> const Value*;

	// [vector.modifiers], modifiers

	template<class... Args>
	auto emplace_back(Args&&... args) -> reference;

	auto push_back(const Value& x) -> void;

	auto push_back(Value&& x) -> void;

	auto pop_back() -> void;

	template<class... Args>
	auto emplace(const_iterator position, Args&&... args) -> iterator;

	auto insert(const_iterator position, const Value& x) -> iterator;

	auto insert(const_iterator position, Value&& x) -> iterator;

	auto insert(const_iterator position, size_type n, const Value& x) -> iterator;

	template<class InputIterator>
	auto insert(
		const_iterator position,
		InputIterator first,
		InputIterator last
	) -> iterator;

	auto insert(const_iterator position, std::initializer_list<Value> il) -> iterator;

	auto erase(const_iterator position) -> iterator;

	auto erase(const_iterator first, const_iterator last) -> iterator;

	auto swap(vector&)
	noexcept(
		allocator_traits<Allocator>::propagate_on_container_swap::value ||
		allocator_traits<Allocator>::is_always_equal::value
	) -> void;

	auto clear() noexcept -> void;

private:

	unsigned capacity_;
	unsigned size_;

	Value* data_;
};

// template<class InputIterator, class Allocator = allocator<iter-value-type<InputIterator>>>
// vector(InputIterator, InputIterator, Allocator = Allocator())
// -> vector<iter-value-type<InputIterator>, Allocator>;

// swap

template<class Value, class Allocator>
void swap(vector<Value, Allocator>& x, vector<Value, Allocator>& y)
noexcept(noexcept(x.swap(y)));
