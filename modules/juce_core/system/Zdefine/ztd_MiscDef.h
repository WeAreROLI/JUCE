#ifndef ztd_miscDef_h__
#define ztd_miscDef_h__

/******************************************************
* ͳһƽ̨��غ궨��
*******************************************************/
#if JUCE_MSVC
#	define noinline __declspec(noinline)
#	define SELECT_ANY __declspec( selectany )
#	define RESTRICT __restrict
#	define ASSUME(cond) __assume(cond)
#	define ASSUME_PTR_SIMD(x) __assume(((intptr_t)x)%16==0)
#	define ASSUME_SIZE_SIMD(x) __assume(x%4==0)
#	define likely(x) (x)
#	define unlikely(x) (x)
#	define likely_if(x) if(x)
#	define unlikely_if(x) if(x)
#	define likely_while(x) while(likely(x))
#	define unlikely_while(x) while(unlikely(x))
#	define ALIGNOF(x) __alignof(x)
#else
#	define noinline __attribute__ ((noinline))
#	define SELECT_ANY __attribute__((selectany))
#	define RESTRICT __restrict__
#	define ASSUME(cond) do { if (!(cond)) __builtin_unreachable(); } while (0) //�˴�����������
#	define ASSUME_PTR_SIMD(x) __builtin_assume_aligned(x,16)
#	define ASSUME_SIZE_SIMD(x) ASSUME(x%4==0)
#	define likely(x) __builtin_expect(!!(x),1)
#	define unlikely(x)  __builtin_expect(!!(x),0)
#	define likely_if(x) if(likely(x))
#	define unlikely_if(x) if(unlikely(x))
#	define likely_while(x) while(likely(x))
#	define unlikely_while(x) while(unlikely(x))
#	define ALIGNOF(x) __alignof__(x)
#endif

#	define ZASSERT(x) if(!(x)) { juce_breakDebugger; JUCE_ANALYZER_NORETURN }

/********************************************************
*	NEVER_TOUCH��,���˺������Զ������(Ҳ��Ӧ��)������λ�ÿ���debugʱ���,releaseʱ�Ż��ٶ�
********************************************************/
#if JUCE_DEBUG
#	define NEVER_TOUCH jassertfalse
#else
#	define NEVER_TOUCH ASSUME(0)
#endif

#define DEBUG_BREAK juce_breakDebugger

#endif // ztd_miscDef_h__
