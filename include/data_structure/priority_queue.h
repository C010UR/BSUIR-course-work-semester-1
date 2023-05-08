#pragma once

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
    inline bool empty() const { return this->elements.empty(); };

    inline void put(T item, priority_t priority) {
        this->elements.emplace(priority, item);
    };

    T get() {
        T best_item = this->elements.top().second;
        elements.pop();
        return best_item;
    };
};
