#ifndef ztd_Zstack_h__
#define ztd_Zstack_h__

ZTD_NAMESPACE_START;

template<typename T>
class Stack
{
public:
	//* ����һ���յ�ջ
	Stack();
	//* ����һ��ջ,ע��! �����������κ���,��ջ���е�Ԫ�ر�ջ������������
	~Stack() = default;
	//* ����һ��ջ���������뱾ջ��,��Ϊ��ȫ��ͬ��other��pop��push����ջ,���Ӷ�ΪO(n).
	Stack(Stack<T>& other):Stack(){ *this<<other; }
	//* ����һ�����е��������뱾ջ��,��Ϊ��ȫ��ͬ��other��pop��push����ջ,���Ӷ�ΪO(1).
	Stack(Queue<T>& other):Stack(){ *this<<other; }
	//* ����һ��ZlockfreeStack�е��������뱾ջ��,��Ϊ��ȫ��ͬ��other��pop��push����ջ,���Ӷ�O(1),��������ֻ��ҪO(1)�ε�ԭ�Ӳ���
	Stack(LockfreeStack<T>& other):Stack(){ *this<<other; }
	//* ����һ��ZlockfreeQueue�е��������뱾ջ��,��Ϊ��ȫ��ͬ��other��pop��push����ջ,���Ӷ�O(1),��������ֻ��ҪO(1)�ε�ԭ�Ӳ���
	Stack(LockfreeQueue<T>& other):Stack(){ *this<<other; }
	//* ����һ��ջ������׷�ӵ���ջ��,��Ϊ��ȫ��ͬ��other��pop��push����ջ,���Ӷ�ΪO(n).
	Stack& operator<<( Stack& other );
	//* ����һ�����е�����׷�ӵ���ջ��,��Ϊ��ȫ��ͬ��other��pop��push����ջ,���Ӷ�ΪO(1).
	Stack& operator<<( Queue<T>& other );
	Stack& operator<<( DubList<T>& other );
	//* ����һ��ZlockfreeStack�е�����׷�ӵ���ջ��,��Ϊ��ȫ��ͬ��other��pop��push����ջ,���Ӷ�O(1),��������ֻ��ҪO(1)�ε�ԭ�Ӳ���
	Stack& operator<<( LockfreeStack<T>& other );
	//* ����һ��ZlockfreeQueue�е�����׷�ӵ���ջ��,��Ϊ��ȫ��ͬ��other��pop��push����ջ,���Ӷ�O(1),��������ֻ��ҪO(1)�ε�ԭ�Ӳ���
	Stack& operator<<( LockfreeQueue<T>& other );
	//* ��һ���ڵ�push�뵱ǰջ
	void Push(T* obj);
	//* ��ջ��popһ���ڵ�,����ɹ�����true,ptrΪpop���Ǹ��ڵ��ָ��,ʧ�ܷ���false,ͬʱptr��ֵΪδ����
	bool Pop(T*& ptr);
	//* ���ջ�Ƿ�Ϊ��
	bool isEmpty() const;
	//* ��ջ��Ϊ��,���ջ���нڵ������Ȩ,�벻Ҫ������
	void setEmpty();
	//* ����ջ�����нڵ�,��һЩ����,������ɾ���ڵ�!! ɾ���ڵ���ʹ��popEach����forEachPop
	template<typename Func> void forEach(const Func&& func);
	//* ����ջ�����нڵ�,�ڵ�һ�����ҳɹ�(func����1)ʱ,���ټ�������
	template<typename Func> void forEachFind(const Func&& func);
	//* �����ڵ�,�ڵ�һ�����ҳɹ�ʱ(func����1)ʱ,���ýڵ��ջ��pop,(��ջ����Ӱ��)��������,����2��ֹͣ����,����0��������Ҷ���pop��ǰ�ڵ�
	template<typename Func> T* forEachPop(const Func&& func);
	//* �����ڵ�,func���������нڵ�,��С�����ջ������Щ�ڵ������Ȩ,pop��ʱ��ҪС�Ĵ���
	template<typename Func> void popEach(const Func&& func);
	//* �����ڵ�,�ڷ���falseʱ,ֹͣpop
	template<typename Func> T* popEachInRange(const Func&& func);
	//* ɾ�����нڵ�,�ǳ�Σ��,ʹ���߱���ȷ����ջ�еĽڵ�ȫ���ǿ���ɾ����,����������objCache������������
	void deleteAllNode();
private:
	friend class Queue<T>;
	friend class LockfreeStack<T>;
	friend class LockfreeQueue<T>;
	T* m_tail;
	ListNode<T> m_dummy;
	Stack& operator=(Stack&)=delete;
	JUCE_LEAK_DETECTOR(Stack);
};

template<typename T>
template<typename Func>
void Stack<T>::forEach(const Func&& func)
{
	for(T* k = m_tail; k != (T*)&m_dummy; k = k->m_next) func(*k);
}

template<typename T>
template<typename Func>
void Stack<T>::forEachFind(const Func&& func)
{
	for(T* k = m_tail; k != (T*)&m_dummy; k = k->m_next) {
		if(!func(*k)) break;
	}
}

template<typename T>
template<typename Func>
T* Stack<T>::forEachPop(const Func&& func)
{
	//static_assert( is_same<result_of<Func>::type,int>::value,"Func must return int" ); GCC.....
	for(T* k = m_tail; k != (T*)&m_dummy;) {
		T*const temp = k->m_next;
		if(func(*k)) break;
		k = temp;
	}
}

template<typename T>
template<typename Func>
void Stack<T>::popEach(const Func&& func)
{
	for(T* k = nullptr; Pop(k); func(k));
}

template<typename T>
void Stack<T>::deleteAllNode()
{
	popEach([](T*const k){ delete k; });
}

//=======================================================================================================

template<typename T>
void Stack<T>::setEmpty()
{
	m_tail = (T*)&m_dummy;
	m_dummy.m_next = (T*)&m_dummy;
}

template<typename T>
bool Stack<T>::isEmpty() const
{
	return m_tail == (T*)&m_dummy;
}

template<typename T>
Stack<T>::Stack()
{
	setEmpty();
}

template<typename T>
void Stack<T>::Push(T* obj)
{
	obj->m_next = m_tail;
	m_tail = obj;
}

template<typename T>
bool Stack<T>::Pop(T*& ptr)
{
	T*const k = m_tail;
	m_tail = k->m_next;
	ptr = k;
	return k != (T*)&m_dummy;
}

ZTD_NAMESPACE_END;

#endif // ztd_Zstack_h__
