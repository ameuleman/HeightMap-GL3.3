#ifndef PARALLELTOOL_H
#define PARALLELTOOL_H

#include <thread>


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
        functor(leftIndex, rightIndex);
    }
}

//------------------------------------------------------------------------------
template<class F> void ParallelTool::performInParallel(F const& functor)
//------------------------------------------------------------------------------
{
        std::thread parallelProcessing(
            [&functor]()
            {
                functor();
            });
}

#endif // PARALLELTOOL_H
