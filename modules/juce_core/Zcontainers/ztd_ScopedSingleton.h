#ifndef ztd_ZscopedSingleton_h__
#define ztd_ZscopedSingleton_h__

ZTD_NAMESPACE_START;

/**********************************************************
*   ���ܵ�������main()֮ǰ���죬�ڳ������ʱ����
	ʹ�õ��������������¼�������:

	1.main����֮ǰ,��������ǵ��̵߳�
	2.������A�Ĺ��캯���п���ʹ�õ���B�����õ��Ĺ���˳��,����˳��͹���˳���෴.
	  ���,���ǲ�������ʹ�õ���,������A()��ʹ����B(��B��A�ȹ���),Ȼ������~B()��ʹ��A,��Ϊ~A()������~B()ִ��.
	3.ScopedSingleton<A,true>��ScopedSingleton<A,false>����һ������! ��Ϊͬһ�����������ܼ����������������new��
	4.������Ĺ��캯���в��������Լ�.
	5.����ѭ������

	@T: ��Ҫ��ɵ���������,��ע�Ᵽ֤���ͱ���Ķ��̷߳���,ScopedSingletonֻ��֤������Ĵ���������,�Լ�����֮��Ĺ���˳����̰߳�ȫ,����֤T������ʵİ�ȫ
	@DirectConstruction: ��Ϊtrueʱ,��static����λ��ֱ�Ӵ�����������,��Ϊfalseʱ,��static����ֻ��һ������ָ��,����T�����ڶ���.
************************************************************/
template<typename T,bool DirectConstruction = true>
class ScopedSingleton
{
private:
	//! ��false==DirectConstructionʱ,InstanceCreator���𴴽���������(ʹ��new)
	class InstanceCreator {
	public:
		forcedinline InstanceCreator() :m_ptr(new T) {};
		forcedinline ~InstanceCreator() { delete m_ptr; };
		forcedinline operator T&() noexcept { return *m_ptr;}
	private:
		T* RESTRICT const m_ptr;
		NONCOPYABLE_CLASS(InstanceCreator);
	};
	typedef typename type_if<DirectConstruction,T,InstanceCreator>::type CreateType; //�˴��ж���ֱ�Ӵ���static��������ʹ��new����
public:
	static forcedinline T& getInstance()
	{
		static CreateType m_instancePtr;
		m_dummyUser.DoNothing(); //�˴��ƺ���ģ���һ��bug,������һ����ܱ�֤main֮ǰm_instancePtrһ��������.
		return m_instancePtr;
	};
private:
	forcedinline ScopedSingleton(){ getInstance(); }
	forcedinline ~ScopedSingleton(){ getInstance(); }
	class DummyInstanceUser
	{
	public:
		forcedinline DummyInstanceUser(){ ScopedSingleton::getInstance(); }
		forcedinline ~DummyInstanceUser() { ScopedSingleton::getInstance(); }
		NONCOPYABLE_CLASS(DummyInstanceUser);
		void DoNothing(){}
	};
private:
	static DummyInstanceUser m_dummyUser;
	NONCOPYABLE_CLASS(ScopedSingleton);
};


template<typename T,bool DirectConstruction>
SELECT_ANY typename ScopedSingleton<T,DirectConstruction>::DummyInstanceUser ScopedSingleton<T,DirectConstruction>::m_dummyUser;

ZTD_NAMESPACE_END;

#endif // ztd_ZscopedSingleton_h__
