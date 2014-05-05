#ifndef ztd_ZcountedSingleton_h__
#define ztd_ZcountedSingleton_h__

ZTD_NAMESPACE_START;

/*****************************************************************************************************************
�̰߳�ȫ�����ü����������ڵ�һ��User����ʱ����ʵ���������һ��User����ʱ����ʵ��.
ֻ�е�һ����User������ʱ,������ᱻ����,�������κ��߳�,�����̻߳��Զ��ȴ���һ��
User�������.ʹ��User����������:

1.�κ�Userʵ��������main����֮�󱻴���.
2.User�����Ǿ�̬����,���� static User a;
3.User������������ó���INT_MAX

CountedSingleton��ʹ����ScopedSingleton��ͬ,��ʵ���ڵ�һ��User����ʱ����,�����һ��User����ʱ����,CountedSingleton
����ֱ��getInstance(),��User����getInstance().getInstance()���൱��һ��ָ�����,��User��
���캯�����ܻ�������ȴ�.������������T��Ҫ��Ϊ����,����X��Ҫ����,��ֱ����X��private�̳�CountedSingleton<T>::User
����.
******************************************************************************************************************/
template<typename T>
class CountedSingleton
{
public:
	friend class User;
	class User
	{
	public:
		NONCOPYABLE_CLASS(User);
		inline User()
		{
			if ( unlikely(++ CountedSingleton::m_counter == 1) ) {
				CountedSingleton::m_instancePtr = new T;
			} else {
				int i=0;
				while ( unlikely(CountedSingleton::m_instancePtr.get()==nullptr) ) {
					if(++i==40) { 
						i=0;
						Thread::sleep(20);
					}
				}
			}
		}
		inline virtual ~User()
		{
			if ( unlikely(--CountedSingleton::m_counter ==0) ) {
				T*const k = CountedSingleton::m_instancePtr.get();
				CountedSingleton::m_instancePtr = nullptr;
				delete k;
			}
		}
		forcedinline T& getInstance() noexcept
		{
			return *CountedSingleton::m_instancePtr.get();
		}
	};
protected:
	NONCOPYABLE_CLASS(CountedSingleton);
	CountedSingleton()=default;
	~CountedSingleton() { jassert(m_counter.get()==0); }
private:
	static Atomic<T*> m_instancePtr;
	static Atomic<int> m_counter;
};


template<typename T>
SELECT_ANY Atomic<T*> CountedSingleton<T>::m_instancePtr; //AtomicĬ�Ϲ����0

template<typename T>
SELECT_ANY Atomic<int> CountedSingleton<T>::m_counter;

ZTD_NAMESPACE_END;

#endif // ztd_ZcountedSingleton_h__
