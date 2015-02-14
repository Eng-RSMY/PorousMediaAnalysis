// Copyright (c) 2013 Vasili Baranau
// Distributed under the MIT software license
// See the accompanying file License.txt or http://opensource.org/licenses/MIT

#ifndef Core_Headers_BaseOrderedPriorityQueue_h
#define Core_Headers_BaseOrderedPriorityQueue_h

#include "HeapUtilities.h"
#include "Macros.h"

namespace Core
{
    template<class TRandomAccessIterator, class TCompare>
    class BaseOrderedPriorityQueue
    {
    private:
        // This class may be used with stl libraries for indirect comparison.
        // TODO: Merge with EnergyService::IndexesComparer
        class IndexesComparer
        {
        public:
            TRandomAccessIterator values;
            TCompare compare;

        public:
            bool operator()(size_t i, size_t j)
            {
                return compare(values[i], values[j]);
            }
        };

        class InitialIndexProvider
        {
        public:
            // The initial index of the value in "valueIndexes" vector is simply the value itself,
            // because the vector was constructed as [0 1 2 3...].
            size_t operator()(size_t value)
            {
                return value;
            }
        };

    private:
        IndexesComparer indexesComparer;
        InitialIndexProvider initialIndexProvider;
        std::vector<size_t> valueIndexes;
        std::vector<size_t> heapPermutation;

    public:
        BaseOrderedPriorityQueue()
        {
        }

        // Have a separate Initialize method to allow
        // a. interface extraction
        // b. creating (if stored by value) or passing the queue (by value or reference) in client constructors, initializing it later
        void Initialize(TRandomAccessIterator first, TRandomAccessIterator lastExclusive, TCompare compare)
        {
            size_t count = lastExclusive - first;
            valueIndexes.resize(count);
            VectorUtilities::FillLinearScale(0, &valueIndexes);

            indexesComparer.values = first;
            indexesComparer.compare = compare;
            HeapUtilities::Make(valueIndexes.begin(), valueIndexes.end(), indexesComparer, initialIndexProvider, &heapPermutation);
        }

        size_t GetTopIndex() const
        {
            return valueIndexes[0];
        }

        void HandleUpdate(size_t valueIndex)
        {
            size_t currentIndexInHeap = heapPermutation[valueIndex];
            HeapUtilities::HandleUpdate(valueIndexes.begin(), valueIndexes.end(), indexesComparer, initialIndexProvider, currentIndexInHeap, &heapPermutation);
        }

        ~BaseOrderedPriorityQueue()
        {
        }

    private:
        DISALLOW_COPY_AND_ASSIGN(BaseOrderedPriorityQueue);
    };
}

#endif /* Core_Headers_BaseOrderedPriorityQueue_h */
