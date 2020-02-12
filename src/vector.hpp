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
	vector(const Allocator& with_allocator) noexcept
		: capacity_{0}
		, size_{0}

		, allocator_{with_allocator}
		, data_{nullptr}
	{}

	explicit
	vector(size_type with_size, const Allocator& with_allocator = Allocator())
		: capacity_{0}
		, size_{0}

		, allocator_{with_allocator}
		, data_{nullptr}
	{	
		resize(with_size);
	}

	vector(
		size_type with_size,
		const Value& with_value,
		const Allocator& with_allocator = Allocator()
	)
		: capacity_{0}
		, size_{0}

		, allocator_{with_allocator}
		, data_{nullptr}
	{
		resize(with_size, with_value);
	}

	template<class InputIterator>
	vector(
		InputIterator first, 
		InputIterator last, 
		const Allocator& = Allocator()
	);

	vector(const vector& x);

	vector(vector&& from_vector) noexcept
		: capacity_{from_vector.capacity()}
		, size_{from_vector.size()}

		, allocator{from_vector.get_allocator()}
		, data_{from_vector.data()}
	{
		from_vector.capacity_ = 0;
		from_vector.data_ = nullptr;
		from_vector.size_ = 0;
	}

	vector(const vector&, const Allocator&);

	vector(vector&&, const Allocator&);

	vector(std::initializer_list<Value>, const Allocator& = Allocator());

	~vector() {
		for(auto it = begin(); it != end(); ++it)
			std::allocator_traits<Allocator>::destroy(allocator_, it);
		std::allocator_traits<Allocator>::deallocate(allocator_, data(), capacity());
	}

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

	auto get_allocator() const noexcept -> allocator_type {
		return allocator_;
	}

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
		return std::allocator_traits<Allocator>::max_size(allocator_);
	}

	auto capacity() const noexcept -> size_type {
		return capacity_;
	}

	auto resize(size_type new_size) -> void {
		if(new_size < size()) {
			for(auto i = new_size; i < size(); ++i)
				std::allocator_traits<Allocator>::destroy(allocator_, begin() + i);
		}
		else {
			if(new_size > capacity())
				reserve(new_size);
			for(auto i = size(); i < new_size; ++i)
				std::allocator_traits<Allocator>::construct(allocator_, begin() + i);
		}
	}

	auto resize(size_type sz, const Value& to_copy) -> void {
		if(new_size < size()) {
			for(auto i = new_size; i < size(); ++i)
				std::allocator_traits<Allocator>::destroy(allocator_, begin() + i);
		}
		else {
			if(new_size > capacity())
				reserve(new_size);
			for(auto i = size(); i < new_size; ++i)
				std::allocator_traits<Allocator>::construct(allocator_, begin() + i, to_copy);
		}
	}

	auto reserve(size_type new_capacity) -> void {
		if(new_capacity > max_size())
			throw std::length_error{"vector::reserve : new_capacity > max_size()"};
		if(new_capacity > capacity()) {
			auto new_data = std::allocator_traits<Allocator>::allocate(allocator_, new_capacity);
			for(auto i = size_type{0}; i < size(); ++i) {
				std::allocator_traits<Allocator>::construct(allocator_, new_data + i, std::move(operator[](i)));
			}
			
			capacity_ = new_capacity;
		}
	}

	auto shrink_to_fit() -> void;

	// element access

	auto operator[](size_type index) -> reference {
		return *(begin() + index);
	}

	auto operator[](size_type index) const -> const_reference {
		auto mutable_this = const_cast<vector*>(this);
		return mutable_this->operator[](index);
	}

	auto at(size_type index) const -> const_reference {
		auto mutable_this = const_cast<vector*>(this);
		return mutable_this->at(index);
	}

	auto at(size_type index) -> reference {
		if(index >= size())
			throw std::out_of_range("vector::at : index > size()");
		return operator[](index);
	}

	auto front() -> reference {
		return *begin();
	}

	auto front() const -> const_reference {
		auto mutable_this = const_cast<vector*>(this);
		return mutable_this->front();
	}

	auto back() -> reference {
		return *(end() - 1);
	}

	auto back() const -> const_reference {
		auto mutable_this = const_cast<vector*>(this);
		return mutable_this->back();
	}

	// [vector.data], data access

	auto data() noexcept -> Value* {
		return data_;
	}

	auto data() const noexcept -> const Value* {
		auto mutable_this = const_cast<vector*>(this);
		return mutable_this->data();
	}

	// [vector.modifiers], modifiers

	template<class... Args>
	auto emplace_back(Args&&... args) -> reference;

	auto push_back(const Value& to_push) -> void {
		if(size() + 1 >= capacity())
			reserve(2 * capacity() + 1);
		std::allocator_traits<Allocator>::construct(allocator_, end(), to_push);
		size += 1;
	}

	auto push_back(Value&& x) -> void {
		if(size() + 1 >= capacity())
			reserve(2 * capacity() + 1);
		std::allocator_traits<Allocator>::construct(allocator_, end(), std::move(to_push));
		size += 1;
	}

	auto pop_back() -> void {
		size -= 1;
		std::allocator_traits<Allocator>::destroy(allocator_, end());
	}

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

	auto swap(vector& to_swap)
	noexcept(
		allocator_traits<Allocator>::propagate_on_container_swap::value ||
		allocator_traits<Allocator>::is_always_equal::value
	) -> void {
		std::swap(capacity_, to_swap.capacity_);
		std::swap(size_, to_swap.size_);
		std::swap(allocator_, to_swap.allocator_);
	}

	auto clear() noexcept -> void;

private:

	unsigned capacity_;
	unsigned size_;

	Allocator allocator_;
	Value* data_;
};

// template<class InputIterator, class Allocator = allocator<iter-value-type<InputIterator>>>
// vector(InputIterator, InputIterator, Allocator = Allocator())
// -> vector<iter-value-type<InputIterator>, Allocator>;

// swap

template<class Value, class Allocator>
void swap(vector<Value, Allocator>& x, vector<Value, Allocator>& y)
noexcept(noexcept(x.swap(y)));
