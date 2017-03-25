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
	template<class F> static void performInParallel(
			F const& functor, unsigned int leftIndex, unsigned int rightIndex,
			unsigned char maxParallelism = std::thread::hardware_concurrency(),
			unsigned char parallilismLvl = 1);

	template<class F> static void performInParallel(F const& functor);
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

		std::thread parallelProcessing(
			[&functor, midIndex, rightIndex, maxParallelism, parallelismLvl]()
			{
				performInParallel(functor,
						midIndex, rightIndex,
						maxParallelism, maxParallelism);
			});

		performInParallel(functor,
					leftIndex, midIndex,
					maxParallelism, parallelismLvl);

		parallelProcessing.join();
	}
	else
	{
		//Use the sequential function
		functor(leftIndex, rightIndex);
	}
}

//------------------------------------------------------------------------------
template<class F> void ParallelTool::performInParallel(F const& functor)
//------------------------------------------------------------------------------
{
		//Launch a thread for the functor
		std::thread parallelProcessing(
			[&functor]()
			{
				functor();
			});
}

#endif // PARALLELTOOL_H
