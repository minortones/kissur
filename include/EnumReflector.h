
#ifndef ENUM_REFLECTOR_H
#define ENUM_REFLECTOR_H

#include "Reflector.h"
#include "EnumString.h"

template<typename T>
class EnumReflector : public IReflector
{
public:
	EnumReflector();
	~EnumReflector();

	const char*		ToString(const void* pValue) const override;
	const char*		Typename() const override;
	kissU32			TypeID() const override;
};

template<typename T>
EnumReflector<T>::EnumReflector()
{}

template<typename T>
EnumReflector<T>::~EnumReflector()
{}

template<typename T>
const char* EnumReflector<T>::ToString( const void* pValue) const
{
	return enumToString( *static_cast<const T*>(pValue) );
}

template<typename T>
kissType EnumReflector<T>::TypeID() const		{ return TypeUID<T>::TypeID(); }

template<typename T>
const char* EnumReflector<T>::Typename() const	{ return TypeUID<T>::Typename(); }



/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
template<typename T>
IReflector* Refl::getReflectInterface(const enable_if_t< std::is_enum<T>::value, T>& pRHS)
{
	static EnumReflector<T> enumReflect;
	return &enumReflect;
}

#endif