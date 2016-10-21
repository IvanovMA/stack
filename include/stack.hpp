#include <iostream>

template <typename T> 
class stack
{
public: 
	stack();                        /* noexcept */
	stack(const stack&);            /* strong */
	~stack();                       /* noexcept */
	size_t count() const;           /* noexcept */
	void push(T const &);           /* strong */
	const T& top() const;          		/* strong */
	void pop();                     /* strong */
	stack& operator=(const stack&); /* strong */
	bool empty() const;					/* noexcept */
private:
	T * array_;
	size_t array_size_;
	size_t count_;
}; 

template<typename T>
T* copy_mas(const T *p1, size_t c, size_t s)
{
	T *p2 = new T[s];
	try
	{ 
		std::copy(p1, p1 + c, p2); 
	}
	catch (...)
	{ 
		delete[] p2; 
		throw; 
	}
	return p2;
}

template <typename T>
stack<T>::stack() : array_(nullptr), array_size_(0), count_(0){};

template <typename T>
stack<T>::stack(const stack& x) : array_size_(x.array_size_), count_(x.count_), array_ (copy_mas(x.array_, x.count_, x.array_size_)){};

template <typename T>
stack<T>::~stack() 
{
	delete[] array_;
}

template <typename T>
size_t stack<T>::count()const 
{
	return count_;
}

template <typename T>
void stack<T>::push(T const &a)
{
	if (count_ == array_size_)
	{
		newarray_size_=array_size_*2 + (count_==0);
		T *p = copy_mas(array_, count_,newarray_size_);
		delete [] array_;
		array_=p;
		array_size_ =newarray_size_;
	}
	array_[count_] = a;
	++count_;
}

template <typename T>
const T& stack<T>::top() const
{
	if (count_ > 0) return array_[count_ -1];
	else throw("Stack is empty");
}

template <typename T>
void stack<T>::pop()
{
	if (count_> 0) 
		--count_;
	else throw ("Stack is empty");
}

template<typename T>
stack<T>& stack<T>::operator=(const stack& b){
	if (this != &b){
		T *p = array_;
		array_ = copy_mas(b.array_, b.count_, b.array_size_);
		delete[] p;
		count_ = b.count_;
		array_size_ = b.array_size_;
	}
	return *this;
}

template<typename T>
bool stack<T>::empty() const
{ 
	return(count_ == 0); 
} 