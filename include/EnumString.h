// Class(es) to ease generating string names for enums.
// experimental.
// recursive hence potentially unsuitable for adequtely large/spaced-out enums. e.g 128 bit flags


#ifndef ENUMSTRING_H
#define ENUMSTRING_H

#include "Debug.h"

template<typename ENUMTYPE>
struct EnumString
{
	template<kiss32 ENUMID>
	static const char* get();
};

#define EXPORTENUMSTRING(EID)							\
	template<> template<>								\
	const char* EnumString<decltype(EID)>::get<EID>()	\
	{ return #EID; }									\


template<kiss32 FROM>
struct EnumLoop
{
	template<typename ENUMTYPE>
	static const char* get(ENUMTYPE pID);
};

#define BEGINENUMSTRING(ETYPE, EID)							\
	DECLARE_TYPENAME(ETYPE)									\
	EXPORTENUMSTRING(EID)									\
template<> template<>										\
inline const char* EnumLoop<EID-1>::get(decltype(EID) pID)	\
{															\
	return "MINIMUS_EXTREMUS";								\
}															\

#define FINISHENUMSTRING(EID)								\
	EXPORTENUMSTRING(EID)									\
template<> template<>										\
inline const char* EnumLoop<EID+1>::get(decltype(EID) pID)	\
{															\
	return "MAXIMUS_EXTREMUS";								\
}															\

template<typename ENUMTYPE> template<kiss32 ENUMID>
const char* EnumString<ENUMTYPE>::get()
{
	KS_ASSERT(0 && "This enum hasn't been exported/declared");
	return "undeclared";
}

template<kiss32 FROM> template<typename ENUMTYPE>
inline const char* EnumLoop<FROM>::get(ENUMTYPE pID)
{
	if (pID == FROM)
		return EnumString<ENUMTYPE>::get<FROM>();
	else if (pID < FROM)
		return EnumLoop<FROM - 1>::get(pID);
	else
		return EnumLoop<FROM + 1>::get(pID);
}

template<typename ETYPE>
inline const char* enumToString(ETYPE pID)			// recurse to the right templated version.
{
	return EnumLoop<0>::get<ETYPE>(pID);
}

#endif	//ENUMSTRING_H