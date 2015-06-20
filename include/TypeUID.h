
#ifndef TYPE_UID_H
#define TYPE_UID_H

#define NAME_MAX	64

typedef kissU32			kissType;

template<typename T>
struct TypeUID
{
	static kissType		TypeID();
	static const char*	Typename();
};

template<typename T>
inline kissType TypeUID<T>::TypeID()				{ static char mID; return kissType(&mID); }


template<>
inline const char* TypeUID<int>::Typename()			{ return "int"; }

template<>
inline const char* TypeUID<bool>::Typename()		{ return "bool"; }

template<>
inline const char* TypeUID<float>::Typename()		{ return "float"; }

template<typename T>
inline const char* TypeUID<T>::Typename()
{
	static char tName[ NAME_MAX ] = { 0 };
	if (tName[0] == 0)
		sprintf_s(tName, NAME_MAX, "%08x", TypeID());

	return tName;
}


#define DECLARE_TYPENAME(TType)					\
template<>										\
inline const char* TypeUID<TType>::Typename()	\
{ return #TType; }								\

#endif