//////////////////////////////////////////////////////////////////////////
///
//	C++ Delegate HEADER FILE
//
///
///	@file		BZ_Lua.h
///	@author		Nnanna Kama
///	@date		14/01/2013
///	@brief		Templated Functors / C++ Delegates.
///	@remarks	These are basically classes that hold instances of
///				other classes and corresponding methods to be invoked as callbacks.
///				Circumventing the need for static callbacks & acts akin to C# delegates.
///				Defines different parameterized functors, more can be added as needed.
///				They all accept a ReturnType, including void.
///
//////////////////////////////////////////////////////////////////////////




#include "common/global/beelzebub.h"



//////////////////////////////////////////////////////////////////////////
// Takes no arguments, has a return type. E.g, for getters.
// <ClassType, ReturnType>
//////////////////////////////////////////////////////////////////////////
template <class ClassType, typename ReturnType>
class TDelegate0
{
	typedef ReturnType (ClassType::*Method)();

public:
	TDelegate0(ClassType** instance, Method method) : mInstanceRef(instance), mCallback(method)
	{}
	
	ReturnType operator() (void)
	{
		BZ_ASSERT( (*mInstanceRef) && "This callback instance done delete, son!" );

		if ( (*mInstanceRef) )
			return ( (*mInstanceRef)->*mCallback )();

		return (ReturnType)NULL;
	}
private:
	ClassType**	mInstanceRef;
	Method		mCallback;
};

//////////////////////////////////////////////////////////////////////////
// Accepts one argument/argument-type and has a return-type.
// <ClassType, ReturnType, ArgumentType>
//////////////////////////////////////////////////////////////////////////
template <class ClassType, typename ReturnType, typename Arg1>
class TDelegate1
{
	typedef ReturnType (ClassType::*Method)(const Arg1);

public:
	TDelegate1(ClassType** instance, Method method) : mInstanceRef(instance), mCallback(method)
	{}

	ReturnType operator() (const Arg1 arg1)
	{
		BZ_ASSERT( (*mInstanceRef) && "This callback instance done delete, son!" );

		if ( (*mInstanceRef) )
			return ( (*mInstanceRef)->*mCallback )(arg1);

		return (ReturnType)NULL;
	}

private:
	ClassType**	mInstanceRef;
	Method		mCallback;
};

//////////////////////////////////////////////////////////////////////////
//	TDELEGATEFUNC method
//	<ClassType, ReturnType, Arg1, FUNCTION>
//////////////////////////////////////////////////////////////////////////
template <class ClassType, typename ReturnType, typename Arg1, class FN>
class TDelegateFunc
{
public:
	TDelegateFunc(ClassType** instance, FN& function ) : mInstanceRef(instance), mFunction(function)
	{}

	ReturnType operator() (const Arg1 arg1)
	{
		return mFunction( (*mInstanceRef), arg1 );
	}

private:
	ClassType** mInstanceRef;
	FN			mFunction;

};





//////////////////////////////////////////////////////////////////////////
//	TEST DRIVER CLASSES
//////////////////////////////////////////////////////////////////////////

class TestClass1
{
public:
	void PrintStuff (const bzText* stuffToPrint) { bz_Debug_PrintToDebugger(stuffToPrint); }
};


class TestClass2
{
public:
	TestClass2() : mID(sIDCounter++)
	{}

	TestClass2( bzS32 id ) : mID(id)
	{}

	bzS32	getID()							{ return mID; }
	void	setID(const bzS32 id )			{ mID = id; }
	bzS32	incrementID(const bzS32 id)		{ sIDCounter = id; mID = sIDCounter++; return sIDCounter; }

private:
	bzS32 mID;
	static bzS32 sIDCounter;
};


void unleashTheKraken()
{
	TestClass1 printer;
	TestClass1* pointerToPrinter = &printer;

	TDelegate1<TestClass1, void, const bzText*> printDelegate( &pointerToPrinter, &TestClass1::PrintStuff );

	printDelegate( "stupid elephant" );
	printDelegate( "stop that chicken" );

	TestClass2* identifier = new TestClass2();
	TDelegate0<TestClass2, bzS32>			idGetter( &identifier, &TestClass2::getID );
	TDelegate1<TestClass2, void, bzS32>		idSetter( &identifier, &TestClass2::setID );
	TDelegate1<TestClass2, bzS32, bzS32>	increase( &identifier, &TestClass2::incrementID );

	bzS32 id = idGetter();
	id = increase(3);
	idSetter(2);

	delete identifier; identifier = NULL;
	idSetter(5);
	id = idGetter();

	char idString[4];
	sprintf_s(idString, "%d", id);
	printDelegate( idString );
}

template<class T, class ReturnType>
ReturnType getMeAGetterDelegate( T& pGetter )
{
	ReturnType val = pGetter();

	return val;
}