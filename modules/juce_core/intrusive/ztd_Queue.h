#ifndef ztd_Zqueue_h__
#define ztd_Zqueue_h__

ZTD_NAMESPACE_START;


template<typename T>
class Queue
{
public:
	//* ����һ���յĶ���
	Queue();
	//* ����һ������,ע��! �����������κ���,��ջ���е�Ԫ�ر�ջ������������
	~Queue()=default;
	//* ����һ��ջ��������ӵ���������,��Ϊ��ȫ��ͬ��other��pop��push��������,���Ӷ�ΪO(n).
	Queue(Stack<T>& other):Queue(){ *this<<other; }
	//* ����һ�����е�������ӵ���������,��Ϊ��ȫ��ͬ��other��pop��push��������,���Ӷ�ΪO(1).
	Queue(Queue& other):Queue(){ *this<<other; }
	//* ����һ��ZlockfreeStack�е����׷�ӵ���������,��Ϊ��ȫ��ͬ��other��pop��push��������,���Ӷ�O(1),��������ֻ��ҪO(1)�ε�ԭ�Ӳ���
	Queue(LockfreeStack<T>& other):Queue(){ *this<<other; }
	//* ����һ��ZlockfreeQueue�е����׷�ӵ���������,��Ϊ��ȫ��ͬ��other��pop��push��������,���Ӷ�O(1),��������ֻ��ҪO(1)�ε�ԭ�Ӳ���
	Queue(LockfreeQueue<T>& other):Queue(){ *this<<other; }
	//* ����һ��ջ������׷�ӵ���������,��Ϊ��ȫ��ͬ��other��pop��push��������,���Ӷ�ΪO(n).
	Queue& operator<<( Stack<T>& other );
	//* ����һ�����е�����׷�ӵ���������,��Ϊ��ȫ��ͬ��other��pop��push��������,���Ӷ�ΪO(1).
	Queue& operator<<( Queue<T>& other );
	Queue& operator<<( DubList<T>& other );
	//* ����һ��ZlockfreeStack�е�����׷�ӵ���������,��Ϊ��ȫ��ͬ��other��pop��push��������,���Ӷ�O(1),��������ֻ��ҪO(1)�ε�ԭ�Ӳ���
	Queue& operator<<( LockfreeStack<T>& other );
	//* ����һ��ZlockfreeQueue�е�����׷�ӵ���������,��Ϊ��ȫ��ͬ��other��pop��push��������,���Ӷ�O(1),��������ֻ��ҪO(1)�ε�ԭ�Ӳ���
	Queue& operator<<( LockfreeQueue<T>& other );
	Queue& operator<<( LockfreeDubList<T>& other );
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
	friend class Stack<T>;
	friend class DubList<T>;
	friend class LockfreeStack<T>;
	friend class LockfreeQueue<T>;
	ListNode<T> m_head;
	T* m_tail;
	Queue& operator=(Queue&)=delete;
	JUCE_LEAK_DETECTOR(Queue);
};

//=======================================================================================================

template<typename T>
void Queue<T>::deleteAllNode() 
{
	popEach([](T*const k){ 
		delete k; 
	}); 
}



template<typename T>
void Queue<T>::setEmpty()
{
	m_head.next() = (T*)&m_head;
	m_tail = (T*)&m_head;
}

template<typename T>
bool Queue<T>::isEmpty() const
{
	return m_tail == (T*)&m_head;
}

template<typename T>
Queue<T>::Queue()
{
	setEmpty();
}

template<typename T>
void Queue<T>::Push(T* obj)
{
	obj->m_next = m_tail->m_next; //head
	m_tail->m_next = obj;
	m_tail = obj;
}

template<typename T>
bool Queue<T>::Pop(T*& ptr)
{
	T*const head = m_tail->m_next;
	m_tail->m_next = head->m_next;
	ptr = head;
	return (T*)&m_head != head;
}

ZTD_NAMESPACE_END;

#endif // ztd_Zqueue_h__
