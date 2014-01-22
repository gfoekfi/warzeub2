#ifndef SINGLETON_H_
#define SINGLETON_H_


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

template <typename T>
class Singleton
{
public:
	static T* Inst()
	{
		if (!instance_)
			instance_ = new T;

		return instance_;
	}

	static void Kill()
	{
		if (instance_)
		{
			delete instance_;
			instance_ = 0;
		}
	}

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	static T* instance_;
};

template <typename T>
T* Singleton<T>::instance_ = 0;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif