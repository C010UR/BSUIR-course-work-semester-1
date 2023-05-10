#pragma once

#include <exception>
#include <functional>
#include <queue>
#include <utility>

template <typename T, typename priority_t>
class PriorityQueue {
   private:
    typedef std::pair<priority_t, T> PQElement;

    std::priority_queue<PQElement, std::vector<PQElement>,
                        std::greater<PQElement>>
        elements;

   public:
    /**
     * @brief Check if queue is empty
     *
     * @return true
     * @return false
     */
    inline bool isEmpty() const { return this->elements.empty(); };

    /**
     * @brief Put `item` with `priority` into queue
     *
     * @param item
     * @param priority
     */
    inline void enqueue(T item, priority_t priority) {
        this->elements.emplace(priority, item);
    };

    /**
     * @brief Gets `item` with the lowest priority
     *
     * @return T
     */
    T dequeue() {
        T best_item = this->elements.top().second;
        elements.pop();
        return best_item;
    };
};
