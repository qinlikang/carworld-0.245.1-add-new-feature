#pragma once
template<class T>
class Singleton
{
public:

	Singleton(void)
		:m_Instance(get_instance())
	{	}

	~Singleton(void)
	{
	}

	static T& get_instance()
	{
		static T instance;
		return instance;
	}
private:
	T& m_Instance;
	Singleton(const Singleton&)	{}

};

