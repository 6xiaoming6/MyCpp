#pragma once


template<typename T>
class SharedPtr
{
private:
	//�򵥵ļ������ƿ�
	struct ControlBlock
	{
		ControlBlock() : SharedCounter(0), WeakCounter(0) {}
		ControlBlock(int shareCount, int weakCount) : SharedCounter(shareCount), WeakCounter(weakCount) {}
		int SharedCounter;
		int WeakCounter;
	};
	//��WeakPtrʹ�õ�˽�й��캯��
	explicit SharedPtr(T* ptr, ControlBlock* cb) : m_Raw(ptr), m_ControlBlock(cb) {}
public:
	SharedPtr() : m_Raw(nullptr), m_ControlBlock{ new ControlBlock() } {}
	explicit SharedPtr(T* ptr) : m_Raw(ptr), m_ControlBlock{ new ControlBlock(1, 1) } {}
	~SharedPtr()
	{
		Release();
	}
	//����������Ϳ�����ֵ
	SharedPtr(const SharedPtr& other)
		:m_Raw(other.m_Raw), m_ControlBlock(other.m_ControlBlock)
	{
		//������������һ
		m_ControlBlock->SharedCounter++;
	}
	SharedPtr& operator=(const SharedPtr& other)
	{
		if (this == &other)return *this;
		//�ͷŵ�ǰ�Լ�����Դ
		ReleaseShared();
		//ָ����˵�����
		m_Raw = other.m_Raw;
		m_ControlBlock = other.m_ControlBlock;
		m_ControlBlock->SharedCounter++;
	}
	//�����ƶ�������ƶ���ֵ
	SharedPtr(SharedPtr&& other) noexcept
		:m_Raw(other.m_Raw), m_ControlBlock(other.m_ControlBlock)
	{
		other.m_Raw = nullptr;
		other.m_ControlBlock = nullptr;
	}
	SharedPtr& operator=(SharedPtr&& other) noexcept
	{
		if (this == &other)return *this;
		//�ͷŵ�ǰ�Լ�����Դ
		ReleaseShared();
		//ָ����˵�����
		m_Raw = other.m_Raw;
		m_ControlBlock = other.m_ControlBlock;
		m_ControlBlock->SharedCounter++;
		//�����˵�������Ϊ��
		other.m_Raw = nullptr;
		other.m_ControlBlock = nullptr;
	}

	//�ⲿ��ȡԭʼָ��
	T* Get() { return m_Raw; }
	const T* Get() const { return m_Raw; }

	//��������ؼ򻯲���
	operator bool() const { return m_Raw != nullptr; }
	T& operator *() { return *m_Raw; }
	T* operator ->() { return m_Raw; }

private:
	//���캯������
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