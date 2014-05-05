

//* 分配对齐的内存
forcedinline void* aligned_malloc( size_t length_byte_0 , size_t const algn_val_ispw2 )
{
	unlikely_if(length_byte_0==0) return nullptr; //这里跟标准的malloc有点不一样
	checkPowerOfTwo(algn_val_ispw2);
	size_t const algn_mask = algn_val_ispw2 - 1;

	char* const ptr = (char*)malloc( length_byte_0 + algn_val_ispw2 + sizeof( int ));
	
	unlikely_if(ptr == nullptr) throw std::bad_alloc();

	char* ptr2 = ptr + sizeof( int );
	char*const algnd_ptr = ptr2 + ( algn_val_ispw2 - ( (size_t)ptr2 & algn_mask ) );

	ptr2 = algnd_ptr - sizeof( int );
	*( (int *)ptr2 ) = (int)( algnd_ptr - ptr );

	return algnd_ptr;
}

//* 分配对齐的内存,并将值清零
forcedinline void* aligned_calloc( size_t length_byte_0 , size_t const algn_val_ispw2 )
{
	void* const p=aligned_malloc(length_byte_0,algn_val_ispw2);
	likely_if(p!=NULL) zeromem(p,length_byte_0);
	return p;
}

//* 释放对齐的内存,必须和aligned_malloc或者aligned_calloc一起使用,不能用来释放malloc的内存
forcedinline void aligned_free( void*const ptr_a )
{
	unlikely_if(ptr_a==nullptr) return;
	int*const ptr2 = (int*)ptr_a - 1;
	char* p = (char*)ptr_a;
	p -= *ptr2;
	free(p);
}
