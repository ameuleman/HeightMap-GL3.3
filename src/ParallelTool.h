#ifndef PARALLELTOOL_H
#define PARALLELTOOL_H

/**
*******************************************************************************
*
*  @file       ParallelTool.h
*
*  @brief      Class to handle functions to simplify parallel processing
*
*  @author     Andréas Meuleman
*******************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <thread>


//==============================================================================
/**
*  @class  ParallelTool
*  @brief  ParallelTool is Class to lhandle functions to simplify parallel processing
*/
//==============================================================================
class ParallelTool
{
public:
	//--------------------------------------------------------------------------
	///Perform a function in parallel
	/**
	*  @param functor: the functor to apply
	*  @param leftIndex: the index of the begining of the part where processing is needed
	*  @param rightIndex: the index of the end of the part where processing is needed
	*  @param maxParallelism: maximum number of thread launched at the same time.
	*	@default: std::thread::hardware_concurrency() (number of concurrent threads supported)
	*  @param parallelismLvl: current level of parallelism @default: 1
	*/
	//--------------------------------------------------------------------------
	template<class F> static void performInParallel(
			F const& functor, unsigned int leftIndex, unsigned int rightIndex,
			unsigned char maxParallelism = std::thread::hardware_concurrency(),
			unsigned char parallelismLvl = 1);

};

//------------------------------------------------------------------------------
template<class F> void ParallelTool::performInParallel(
		F const& functor, unsigned int leftIndex, unsigned int rightIndex,
		unsigned char maxParallelism, unsigned char parallelismLvl)
//------------------------------------------------------------------------------
{
	parallelismLvl *= 2;

	//If we can increase parralelism, we split the workload into to threads
	if(parallelismLvl <= maxParallelism)
	{
		unsigned int midIndex((unsigned int)(rightIndex / 2));

		//launch a thread that perform the functor on the left side of the array
		std::thread parallelProcessing(
			[&functor, midIndex, rightIndex, maxParallelism, parallelismLvl]()
			{
				performInParallel(functor,
						midIndex, rightIndex,
						maxParallelism, maxParallelism);
			});

		//perform the functor on the right side
		performInParallel(functor,
					leftIndex, midIndex,
					maxParallelism, parallelismLvl);

		//wait for the first thread to end processing
		parallelProcessing.join();
	}
	else
	{
		//Use the sequential function
		functor(leftIndex, rightIndex);
	}
}


#endif // PARALLELTOOL_H
