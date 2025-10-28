#pragma once


template<typename T>
class SharedPtr
{
private:
	//简单的计数控制块
	struct ControlBlock
	{
		ControlBlock() : SharedCounter(0), WeakCounter(0) {}
		ControlBlock(int shareCount, int weakCount) : SharedCounter(shareCount), WeakCounter(weakCount) {}
		int SharedCounter;
		int WeakCounter;
	};
	//给WeakPtr使用的私有构造函数
	explicit SharedPtr(T* ptr, ControlBlock* cb) : m_Raw(ptr), m_ControlBlock(cb) {}
public:
	SharedPtr() : m_Raw(nullptr), m_ControlBlock{ new ControlBlock() } {}
	explicit SharedPtr(T* ptr) : m_Raw(ptr), m_ControlBlock{ new ControlBlock(1, 1) } {}
	~SharedPtr()
	{
		Release();
	}
	//处理拷贝构造和拷贝赋值
	SharedPtr(const SharedPtr& other)
		:m_Raw(other.m_Raw), m_ControlBlock(other.m_ControlBlock)
	{
		//共有引用数加一
		m_ControlBlock->SharedCounter++;
	}
	SharedPtr& operator=(const SharedPtr& other)
	{
		if (this == &other)return *this;
		//释放当前自己的资源
		ReleaseShared();
		//指向别人的数据
		m_Raw = other.m_Raw;
		m_ControlBlock = other.m_ControlBlock;
		m_ControlBlock->SharedCounter++;
	}
	//处理移动构造和移动赋值
	SharedPtr(SharedPtr&& other) noexcept
		:m_Raw(other.m_Raw), m_ControlBlock(other.m_ControlBlock)
	{
		other.m_Raw = nullptr;
		other.m_ControlBlock = nullptr;
	}
	SharedPtr& operator=(SharedPtr&& other) noexcept
	{
		if (this == &other)return *this;
		//释放当前自己的资源
		ReleaseShared();
		//指向别人的数据
		m_Raw = other.m_Raw;
		m_ControlBlock = other.m_ControlBlock;
		m_ControlBlock->SharedCounter++;
		//将别人的数据置为空
		other.m_Raw = nullptr;
		other.m_ControlBlock = nullptr;
	}

	//外部获取原始指针
	T* Get() { return m_Raw; }
	const T* Get() const { return m_Raw; }

	//运算符重载简化操作
	operator bool() const { return m_Raw != nullptr; }
	T& operator *() { return *m_Raw; }
	T* operator ->() { return m_Raw; }

private:
	//构造函数调用
	void Release()
	{
		if (m_ControlBlock)
		{
			if (--m_ControlBlock->SharedCounter == 0)
			{
				delete m_Raw;
				m_Raw = nullptr;
				ReleaseWeak();
			}
		}
	}

	void ReleaseShared()
	{
		if (m_ControlBlock)
		{
			if (--(m_ControlBlock->SharedCounter) == 0)
			{
				delete m_Raw;
				m_Raw = nullptr;
			}
		}
	}
	void ReleaseWeak()
	{
		if (m_ControlBlock)
		{
			if (--(m_ControlBlock->WeakCounter) == 0)
			{
				delete m_ControlBlock;
				m_ControlBlock = nullptr;
			}
		}
	}

private:
	T* m_Raw;
	ControlBlock* m_ControlBlock;

	friend class WeakPtr<T>;
};