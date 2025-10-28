#pragma once

template<typename T>
class UniquePtr
{
public:
	UniquePtr() : m_Raw(nullptr) {}
	//必须显示调用，防止发生隐式转换
	explicit UniquePtr(T* ptr) : m_Raw(ptr) {}
	~UniquePtr()
	{
		//析构时释放内存
		if (m_Raw) { delete m_Raw; m_Raw = nullptr; }
	}

	//不允许资源拷贝，拷贝构造和赋值运算符都删除
	UniquePtr(const UniquePtr& other) = delete;
	UniquePtr& operator=(const UniquePtr& other) = delete;

	//只能通过移动构造或移动赋值实现资源转移
	UniquePtr(UniquePtr&& other) noexcept
	{
		m_Raw = other.m_Raw;
		other.m_Raw = nullptr;
	}
	UniquePtr& operator = (UniquePtr&& other) noexcept
	{
		//避免自赋值导致误删
		if (this == &other) return *this;
		if (m_Raw)delete m_Raw;
		m_Raw = other.m_Raw;
		other.m_Raw = nullptr;
	}

	//外部获取原始指针
	T* Get() { return m_Raw; }
	const T* Get() const { return m_Raw; }

	//运算符重载简化操作
	operator bool() const { return m_Raw != nullptr; }
	T& operator *() { return *m_Raw; }
	T* operator ->() { return m_Raw; }


private:
	//原始指针
	T* m_Raw;
};