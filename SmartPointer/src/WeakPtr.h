#pragma once
template<typename T>
class WeakPtr
{
public:
	WeakPtr() : m_Raw(nullptr), m_ControlBlock(nullptr) {}
	WeakPtr(const SharedPtr<T>& sPtr)
		: m_Raw(sPtr.m_Raw), m_ControlBlock(sPtr.m_ControlBlock)
	{
		m_ControlBlock->WeakCounter++;
	}
	~WeakPtr() { Release(); }
	//拷贝构造和拷贝赋值
	WeakPtr(const WeakPtr& other)
		:m_Raw(other.m_Raw), m_ControlBlock(other.m_ControlBlock)
	{
		m_ControlBlock->WeakCounter++;
	}
	WeakPtr& operator=(const WeakPtr& other)
	{
		if (this == &other)return *this;
		//释放自己的占用
		Release();
		//指向别人的资源
		m_Raw = other.m_Raw;
		m_ControlBlock = other.m_ControlBlock;
	}
	//移动构造和移动赋值
	WeakPtr(WeakPtr&& other) noexcept
		:m_Raw(other.m_Raw), m_ControlBlock(other.m_ControlBlock)
	{
		other.m_Raw = nullptr;
		other.m_ControlBlock = nullptr;
	}
	WeakPtr& operator=(WeakPtr&& other) noexcept
	{
		if (this == &other)return *this;
		//释放自己的占用
		Release();
		//指向别人的资源
		m_Raw = other.m_Raw;
		m_ControlBlock = other.m_ControlBlock;
		//将别人的资源置空
		other.m_Raw = nullptr;
		other.m_ControlBlock = nullptr;
	}

	//辅助的函数
	inline bool Expired() { return m_ControlBlock->WeakCounter == 0; }
	inline SharedPtr<T> Lock() { return SharedPtr<T>(m_Raw, m_ControlBlock); }

private:
	void Release()
	{
		if (m_ControlBlock)
		{
			if (--m_ControlBlock->WeakCounter == 0 && m_ControlBlock->SharedCounter == 0)
			{
				delete m_ControlBlock;
				m_ControlBlock = nullptr;
			}
		}

	}

private:
	T* m_Raw;
	ControlBlock* m_ControlBlock;
};