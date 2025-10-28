#pragma once

template<typename T>
class UniquePtr
{
public:
	UniquePtr() : m_Raw(nullptr) {}
	//������ʾ���ã���ֹ������ʽת��
	explicit UniquePtr(T* ptr) : m_Raw(ptr) {}
	~UniquePtr()
	{
		//����ʱ�ͷ��ڴ�
		if (m_Raw) { delete m_Raw; m_Raw = nullptr; }
	}

	//��������Դ��������������͸�ֵ�������ɾ��
	UniquePtr(const UniquePtr& other) = delete;
	UniquePtr& operator=(const UniquePtr& other) = delete;

	//ֻ��ͨ���ƶ�������ƶ���ֵʵ����Դת��
	UniquePtr(UniquePtr&& other) noexcept
	{
		m_Raw = other.m_Raw;
		other.m_Raw = nullptr;
	}
	UniquePtr& operator = (UniquePtr&& other) noexcept
	{
		//�����Ը�ֵ������ɾ
		if (this == &other) return *this;
		if (m_Raw)delete m_Raw;
		m_Raw = other.m_Raw;
		other.m_Raw = nullptr;
	}

	//�ⲿ��ȡԭʼָ��
	T* Get() { return m_Raw; }
	const T* Get() const { return m_Raw; }

	//��������ؼ򻯲���
	operator bool() const { return m_Raw != nullptr; }
	T& operator *() { return *m_Raw; }
	T* operator ->() { return m_Raw; }


private:
	//ԭʼָ��
	T* m_Raw;
};