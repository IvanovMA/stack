#include <iostream>


template <typename T1, typename T2>
void construct(T1 * ptr, T2 const & value) {
    new(ptr) T1 (value);
}

template <typename T>
void destroy(T * ptr) noexcept
{
    ptr->~T();
}

template <typename FwdIter>
void destroy(FwdIter first, FwdIter last) noexcept
{
    for (; first != last; ++first) {
        destroy(&*first);
    }
}

template <typename T>
class allocator
{
protected:
    allocator(size_t size = 0);
    ~allocator();
    auto swap(allocator & other) -> void;
    
    allocator(allocator const &) = delete;
    auto operator =(allocator const &) -> allocator & = delete;
    
    T * ptr_;
    size_t size_;
    size_t count_;
};

template <typename T>
allocator<T>::allocator(size_t size) : ptr_((T*)(operator new(size*sizeof(T)))), size_(size), count_(0){}

template<typename T>
allocator<T>::~allocator()
{
	operator delete(ptr_);
}

template<typename T>
auto allocator<T>::swap(allocator & other) -> void {
	std::swap(ptr_, other.ptr_);
	std::swap(count_, other.count_);
	std::swap(size_, other.size_);
}

template <typename T> 
class stack : private allocator<T>
{
public: 
	stack(size_t size = 0);         /* noexcept */
	stack(const stack&);            /* strong */
	~stack();                       /* noexcept */
	size_t count() const;           /* noexcept */
	void push(T const &);           /* strong */
	const T& top() const;          		/* strong */	
	void pop();                     /* strong */
	auto operator=(stack const & right)->stack &; /* strong */
	bool empty() const;					/* noexcept */
}; 

template <typename T>
stack<T>::stack(size_t size) :allocator<T>(size){};

template <typename T>
stack<T>::stack(const stack& x) :allocator<T>(x.size_){
	for (size_t i = 0; i < x.count_; i++) construct(allocator<T>::ptr_ + i, x.ptr_[i]);
	allocator<T>::count_ = x.count_;
};

template <typename T>
stack<T>::~stack() 
{
	destroy(allocator<T>::ptr_, allocator<T>::ptr_ + allocator<T>::count_);
}

template <typename T>
size_t stack<T>::count()const 
{
	return allocator<T>::count_;
}

template <typename T>
void stack<T>::push(T const &item)
{
	if (allocator<T>::count_ == allocator<T>::size_) {
		size_t array_size = allocator<T>::size_ * 2 + (allocator<T>::size_ == 0);
		stack<T> temp(array_size);
		while (temp.count() < allocator<T>::count_) temp.push(allocator<T>::ptr_[temp.count()]); 
		this->swap(temp);
	}
	construct(allocator<T>::ptr_ + allocator<T>::count_, item);
	++allocator<T>::count_;
}

template <typename T>
const T& stack<T>::top() const
{
	if (allocator<T>::count_ == 0) {
		throw ("Stack is empty!");
	}
	return allocator<T>::ptr_[allocator<T>::count_ - 1];
}

template <typename T>
void stack<T>::pop()
{
	if (allocator<T>::count_ == 0) {
		throw ("Stack is empty!");
	} else {
		allocator<T>::count_--;
	}
}

template<typename T>
auto stack<T>::operator=(stack const & right) -> stack & 
{
	if (this != &right) {
	stack<T> temp (right.size_);
	while (temp.count_ < right.count_){
		construct(temp.ptr_ + temp.count_, right.ptr_[temp.count_]);
		++temp.count_;
	}	
	this -> swap(temp);
	}
	return *this;
}

template<typename T>
bool stack<T>::empty() const{
	if (allocator<T>::count_ == 0){
		return true;
	} else{
		return false;
	}
}
