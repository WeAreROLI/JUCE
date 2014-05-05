#ifndef ztd_typeDef_h__
#define ztd_typeDef_h__

// using std::is_same;
// using std::is_base_of;
// using std::is_standard_layout;
// using std::is_trivial;
// using std::remove_cv;
// using std::remove_const;
// using std::remove_pointer;
// using std::is_reference;
// using std::tuple;
// using std::get;
// using std::initializer_list;
// using std::function;

ZTD_NAMESPACE_START;

typedef unsigned int uint;

typedef __m128 m128;
typedef __m128d m128d;
typedef __m128i m128i;
typedef ptrdiff_t intc; //һ���,������int������size������,������x64����������ô��,���Զ���һ���з���size_t

ZTD_NAMESPACE_END;

#endif // ztd_typeDef_h__
