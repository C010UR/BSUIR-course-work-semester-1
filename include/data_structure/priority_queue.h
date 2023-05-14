#pragma once

#include <queue>
#include <stdexcept>

template <typename T, typename priority_t> class PriorityQueue
{
  private:
    typedef std::pair<priority_t, T> PQElement;

    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;

  public:
    /**
     * @brief Check if queue is empty
     *
     * @return true
     * @return false
     */
    inline bool empty() const
    {
        return this->elements.empty();
    }

    /**
     * @brief Put `item` with `priority` into queue
     *
     * @param item
     * @param priority
     */
    inline void push(T item, priority_t priority)
    {
        this->elements.emplace(priority, item);
    }

    /**
     * @brief Gets `item` with the lowest priority
     *
     * @return T
     */
    T pop()
    {
        if (this->empty())
        {
            throw std::out_of_range("Out of range exception: Cannot pop from priority queue. Priority queue is empty.");
        }

        T best_item = this->elements.top().second;
        elements.pop();
        return best_item;
    }
};
