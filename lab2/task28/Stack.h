#pragma once
#include <algorithm>
#include <stdexcept>

template <typename T>
class Stack
{
public:
	bool Empty() const;
	T GetTop() const;
	void Push(T str);
	void Pop();
	void Clear();
	size_t GetSize() const;

	Stack() = default;
	~Stack();
	Stack(Stack const& stack);
	Stack(Stack&& stack) noexcept;
	Stack& operator =(Stack const& stack);
	Stack& operator =(Stack&& stack) noexcept;
private:
	struct StackNode
	{
		StackNode(T initValue, StackNode* nextNode)
			: value(std::move(initValue))
			, next(nextNode)
		{}

		T value;
		StackNode* next;
	};

	StackNode* m_top = nullptr;
	size_t m_size = 0;
};

template <typename T>
bool Stack<T>::Empty() const
{
	return m_top == nullptr;
}

template <typename T>
T Stack<T>::GetTop() const
{
	if (Empty())
	{
		throw std::logic_error("No elements in stack");
	}

	return m_top->value;
}

template <typename T>
void Stack<T>::Push(T item)
{
	if (m_size == SIZE_MAX)
	{
		throw std::logic_error("Stack is full of elements");
	}

	m_top = new StackNode(std::move(item), m_top);
	m_size++;
}

template <typename T>
void Stack<T>::Pop()
{
	if (Empty())
	{
		throw std::logic_error("No elements in stack");
	}

	StackNode* top = m_top;
	m_top = m_top->next;
	m_size--;
	delete top;
}

template <typename T>
void Stack<T>::Clear()
{
	while (!Empty())
	{
		Pop();
	}
}

template <typename T>
size_t Stack<T>::GetSize() const
{
	return m_size;
}

template <typename T>
Stack<T>::~Stack()
{
	Clear();
}

template <typename T>
Stack<T>::Stack(Stack<T> const& stack)
	: m_size(0)
{
	StackNode* node = stack.m_top;
	if (node == nullptr)
	{
		m_top = nullptr;
		return;
	}
	m_top = new StackNode(node->value, nullptr);
	m_size++;
	StackNode* prev = m_top;
	while (node->next != nullptr)
	{
		node = node->next;
		try
		{
			prev->next = new StackNode(node->value, nullptr);
		}
		catch (std::exception& exception)
		{
			Clear();
			throw;
		}
		m_size++;
		prev = prev->next;
	}
}

template <typename T>
Stack<T>::Stack(Stack<T>&& stack) noexcept
	: m_top(stack.m_top)
	, m_size(stack.m_size)
{
	stack.m_top = nullptr;
	stack.m_size = 0;
}

template <typename T>
Stack<T>& Stack<T>::operator =(Stack<T> const& stack)
{
	if (&stack != this)
	{
		Stack stackToCopy(stack);
		std::swap(m_top, stackToCopy.m_top);
		std::swap(m_size, stackToCopy.m_size);
	}

	return *this;
}

template <typename T>
Stack<T>& Stack<T>::operator =(Stack<T>&& stack) noexcept
{
	if (&stack != this)
	{
		Clear();
		m_top = stack.m_top;
		m_size = stack.m_size;
		stack.m_top = nullptr;
		stack.m_size = 0;
	}

	return *this;
}