#pragma once
#include <memory>
//-----------------------------------std::function�ļ�ʵ��

template<typename Ret, typename ...Arg>
class Function;


template<typename Ret, typename ...Arg>
class Function<Ret(Arg...)>
{
private:
	struct Callable
	{
		virtual ~Callable() = default;
		virtual Ret invoke(Arg&&... args) = 0;
		virtual Callable* clone() const = 0;
	};

	template<typename T>
	struct CallObject : Callable
	{
		T func;
		CallObject(const T& t) :func(t) {}
		virtual Ret invoke(Arg&&... args) override
		{
			return func(std::forward<Arg>(args)...);
		}
		virtual Callable* clone() const override
		{
			return new CallObject<T>(func);
		}
	};
private:
	Callable* ptr;
public:
	Function() :ptr(nullptr) {}
	template<typename T>
	Function(T&& t) :ptr{ new CallObject<std::decay_t<T>>(std::forward<T>(t)) } {}

	//������ֵ���õĿ������캯���͸�ֵ����
	Function(const Function& other)
	{
		ptr = other.ptr->clone();
	}
	Function& operator=(const Function& other)
	{
		if (ptr)delete ptr;
		ptr = other.ptr->clone();

		return *this;
	}
	//������ֵ���õ��ƶ����캯�����ƶ���ֵ����
	Function(Function&& other) noexcept
	{
		ptr = other.ptr;
		other.ptr = nullptr;
	}
	Function& operator=(Function&& other) noexcept
	{
		if (ptr)delete ptr;
		ptr = other.ptr;
		other.ptr = nullptr;

		return *this;
	}
	//����()�����
	Ret operator()(Arg&& ...args)
	{
		return ptr->invoke(std::forward<Arg>(args)...);
	}
	~Function()
	{
		if (ptr)delete ptr;
	}


};
