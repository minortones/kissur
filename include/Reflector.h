
#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <type_traits>
#include "TypeUID.h"

class IReflector
{
public:
	virtual ~IReflector()	{}
	virtual const char*		ToString(const void* pValue) const = 0;
	virtual const char*		Typename() const = 0;
	virtual kissType		TypeID() const = 0;
};


template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

class Refl
{
public:
	template<typename T>
	Refl(const T& pRHS)			{ mInterface = getReflectInterface<T>(pRHS); mClient = &pRHS; }

	template<typename T>
	Refl(const T* pRHS)			{ mInterface = getReflectInterface<T>(pRHS); mClient = pRHS; }

	~Refl()						{}

	const char*		ToString() const		{ return mInterface->ToString( mClient ); }
	const char*		Typename() const		{ return mInterface->Typename(); }
	kissType		TypeID() const			{ return mInterface->TypeID(); }

	const IReflector* operator->() const	{ return mInterface; }

private:
	IReflector*		mInterface;
	const void*		mClient;

	template<typename T>
	static IReflector* getReflectInterface(const enable_if_t< std::is_enum<T>::value, T>& pRHS);	// enum specialisation

	template<typename T>
	static IReflector* getReflectInterface(const T& pRHS);
};


#endif