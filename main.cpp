
#include "AppLayer/GLApplication.h"
#include "EnumReflector.h"
#include "CyclicConcurrentQueue.h"
#include <thread>
#include <chrono>
#include <mutex>


typedef std::chrono::steady_clock hrclock;
typedef std::chrono::microseconds microseconds;

#define kissEnum(name, ...) enum name { __VA_ARGS__, __COUNT};

enum FStates
{
	fsHang = -2,
	fsInvalid = -1,
	fsDead,
	fsBoot,
	fsLive
};

BEGINENUMSTRING(FStates, fsInvalid)
EXPORTENUMSTRING(fsDead)
EXPORTENUMSTRING(fsBoot)
FINISHENUMSTRING(fsLive)

template <int v>
struct Int2Type
{
	enum { value = v };
};

#define USE_ALIGNED_QUEUE_DATA 1

#if USE_ALIGNED_QUEUE_DATA
struct page
{
	page() {}
	page(int id) {}
	page& operator=(page&& other)	{ memcpy(dsc, other.dsc, CACHE_LINE_SIZE); return *this; }
	char dsc[CACHE_LINE_SIZE];
};
#else
typedef char page;
#endif

template<typename T>
struct cache_line_storage
{
	T data;
	char pad[CACHE_LINE_SIZE > sizeof(T) ? CACHE_LINE_SIZE - sizeof(T) : 1];
};


//////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN ENTRY
//////////////////////////////////////////////////////////////////////////////////////////////////////
CyclicConcurrentQueue<page> gQueue(1048575);	//1048576 pot
CyclicConcurrentQueue<page> gNPOTQueue(1048576);

#define NUM_CONSUMERS					0
#define NUM_PRODUCERS					3
#define TOTAL_NUM_TASKS					1048500
static const int MAX_PRODUCER_TASKS		= TOTAL_NUM_TASKS / NUM_PRODUCERS;

#if NUM_PRODUCERS < 5
ksU32 CyclicConcurrentQueue<page>::CONTEXT_SWITCH_LATENCY = 20000;
#else
ksU32 CyclicConcurrentQueue<page>::CONTEXT_SWITCH_LATENCY = 1;
#endif

#if NUM_PRODUCERS == 1
#define macro_enqueue		enqueue_singlethreaded
#else
#define macro_enqueue		enqueue
#endif

static volatile bool gKickProducerThreads(false);
static volatile bool gKickConsumerThreads(true);
static volatile bool gRunTheJewels(true);
static volatile cache_line_storage<bool> gCompletionSignal[NUM_PRODUCERS] = {};

CyclicConcurrentQueue<page>* gCurrentQueue(nullptr);

//static std::mutex gPrintMtx;

void producer(int id)
{
	while (gRunTheJewels)
	{
		while (!gKickProducerThreads)
		{}

		const auto t0 = hrclock::now();
		
		WRITE_BARRIER;
		for (int i = 0; i < MAX_PRODUCER_TASKS; ++i)
		{
			gCurrentQueue->macro_enqueue(page());
		}
		READ_BARRIER;

		const auto t1 = hrclock::now();
		auto us = std::chrono::duration_cast<microseconds>(t1 - t0).count();
		printf("%lluus\t", us);

		gCompletionSignal[id].data = true;

		while (gCompletionSignal[id].data == true)
		{
			THREAD_SWITCH;
		}
	}
}
void consumer(CyclicConcurrentQueue<page>& pQueue)
{
	int consumed(0);
	bool isValid(false);
	while (isValid || gKickConsumerThreads)
	{
		auto result = pQueue.dequeue();
		isValid = *result;
		if (isValid)
		{
			++consumed;
		}
		else
			THREAD_SWITCH;
	}

	//printf("total: %d\n", consumed);
}

void evalProducers(ks::Array<std::thread>& pThreads, CyclicConcurrentQueue<page>& pQueue)
{
	for (int i = 0; i < NUM_PRODUCERS; ++i)
		gCompletionSignal[i].data = false;

	pQueue.clear();
	gCurrentQueue = &pQueue;

	WRITE_BARRIER;
	gKickProducerThreads = true;

	for (int i = 0; i < NUM_PRODUCERS; ++i)
	{
		while (gCompletionSignal[i].data == false)
		{
			THREAD_SLEEP(5);
		}
	}

	gKickProducerThreads = false;
	printf("\n\n");
}

int main( int argc, char *argv[] )
{
	auto state = fsDead;
	{
		ks::Array<std::thread> prodthreads(NUM_PRODUCERS);
		ks::Array<std::thread> consthreads;

		for (int i = 0; i < NUM_PRODUCERS; ++i)
		{
			prodthreads.push_back(std::thread{ producer, i });
			prodthreads[i].detach();
		}


		printf("NON-POWER OF TWO QUEUE\n");
		evalProducers(prodthreads, gNPOTQueue);

		printf("NON-POWER OF TWO QUEUE\n");
		evalProducers(prodthreads, gNPOTQueue);

		printf("NON-POWER OF TWO QUEUE\n");
		evalProducers(prodthreads, gNPOTQueue);

		printf("POWER OF TWO QUEUE\n");
		evalProducers(prodthreads, gQueue);

		printf("POWER OF TWO QUEUE\n");
		evalProducers(prodthreads, gQueue);

		printf("POWER OF TWO QUEUE\n");
		evalProducers(prodthreads, gQueue);

		gRunTheJewels = false;	// close threads
		for (int i = 0; i < NUM_PRODUCERS; ++i)
			gCompletionSignal[i].data = false;


		EnumReflector<FStates> erDefault;
		ks::Array< EnumReflector<FStates> > psuedos;
		psuedos.push_back(erDefault);
		psuedos.resize(11, erDefault);
		{
			ks::Array<ks::Array< EnumReflector<FStates>>*> bay(4);
			bay.push_back(&psuedos);
			bay.insert(bay.begin(), *bay.rbegin());

			ks::Array<ks::Array< EnumReflector<FStates>>*> baymax;
			baymax.explicit_copy(bay);

			auto name = (*baymax[0])[0].Typename();
		}
		auto name = psuedos[0].ToString(&state);
	}

	GLApplication* app = new GLApplication();
	Service<GLApplication>::Register( app );

	auto pstate		= fsDead;
	auto stateName	= Refl( pstate ).ToString();
	auto typeN		= Refl( pstate ).Typename();
	//typeN			= Refl( app ).Typename();
	
	app->init(argc, argv);
	app->go();

	delete app;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// ENDOF MAIN
//////////////////////////////////////////////////////////////////////////////////////////////////////