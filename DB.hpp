#ifndef DB_HPP
#define DB_HPP

#include <fstream>
#include <utility>

// транспортный, пассажирский, военный
enum class PlaneType : unsigned char { PASSENGER, TRANSPORT, MILITARY };
struct Plane {
    unsigned char num_seats;
    unsigned char num_occupied;
    unsigned char type;
};

struct DBNode {
    Plane pln;
    DBNode *next;
};

struct Database {
    auto push_back(Plane &&pln) {
        if (m_begin == nullptr) {
            m_begin = new DBNode{std::move(pln), nullptr};
            m_end = m_begin;

            return;
        }

        m_end->next = new DBNode{std::move(pln), nullptr};
        m_end = m_end->next;
    }

    auto read(const std::filesystem::path file) {
        std::ifstream input{file};
        while (!input.eof()) {
            Plane temp;
            input >> temp.num_seats;
            input >> temp.num_occupied;
            input >> temp.type;

            this->push_back(std::move(temp));
        }
    }

    struct DBIterator {
        auto operator++(int) { current = current->next; }
        auto operator++() { current = current->next; }
        bool operator!=(const DBIterator &other) const = default;
        bool operator==(const DBIterator &other) const = default;
        auto operator*() const -> auto & { return *current; }
        DBNode *current;
    };

    Database() = default;
    Database(DBNode *start) : m_begin{start}, m_end{start} {}

    auto begin() { return DBIterator{m_begin}; }
    auto end() { return DBIterator{nullptr}; }
    DBNode *m_begin;
    DBNode *m_end;
};

#endif
