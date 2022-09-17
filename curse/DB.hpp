#ifndef DB_HPP
#define DB_HPP

#include <filesystem>
#include <fstream>
#include <utility>

// транспортный, пассажирский, военный
struct Plane {
    unsigned short num_seats;
    unsigned short num_occupied;
    char type[20];  // NOLINT
};

struct DBNode {
    Plane plane;
    DBNode *next;
};

struct Database {
    constexpr auto push_back(const Plane &pln) noexcept -> void {
        if (m_begin == nullptr) [[unlikely]] {
            m_begin = new DBNode{pln, nullptr};  // NOLINT
            m_end = m_begin;

            return;
        }

        m_end->next = new DBNode{pln, nullptr};  // NOLINT
        m_end = m_end->next;
    }

    constexpr auto join(Database &other) noexcept -> void {
        m_end->next = other.m_begin;
        m_end = other.m_end;

        other.m_begin = nullptr;
        other.m_end = nullptr;
    }

    auto read(const std::filesystem::path &file) noexcept -> void {
        std::ifstream input{file};
        Plane temp{};
        while (input >> temp.num_seats >> temp.num_occupied >> temp.type)
            this->push_back(temp);
    }

    auto save(const std::filesystem::path &file) noexcept -> void {
        std::ofstream output{file};
        for (auto &node : *this)
            output << node.plane.num_seats << ' ' << node.plane.num_occupied
                   << ' ' << node.plane.type << '\n';
    }

    constexpr auto clear() noexcept -> void {
        DBIterator iter{m_begin};

        while (iter.current != nullptr) {
            auto *next{iter.current->next};
            delete iter.current;  // NOLINT
            iter.current = next;
        }
    }

    [[nodiscard]] constexpr auto isEmpty() const noexcept -> bool {
        return m_begin == nullptr;
    }

    struct DBIterator {
        constexpr auto operator++(int) noexcept -> void {
            current = current->next;
        }
        constexpr auto operator++() noexcept -> void {
            current = current->next;
        }
        constexpr auto operator!=(const DBIterator &other) const noexcept
            -> bool = default;
        constexpr auto operator==(const DBIterator &other) const noexcept
            -> bool = default;
        constexpr auto operator*() const noexcept -> DBNode & {
            return *current;
        }
        DBNode *current;
    };

    constexpr Database() noexcept = default;

    // Правило 5
    constexpr Database(const Database &) noexcept = delete;
    constexpr Database(Database &&) noexcept = delete;
    constexpr auto operator=(const Database &) noexcept -> Database & = delete;
    constexpr auto operator=(Database &&) noexcept -> Database & = delete;
    constexpr ~Database() { this->clear(); }

    [[nodiscard]] constexpr auto begin() const noexcept -> DBIterator {
        return DBIterator{m_begin};
    }
    [[nodiscard]] static constexpr auto end() noexcept -> DBIterator {
        return DBIterator{nullptr};
    }

   private:
    DBNode *m_begin;
    DBNode *m_end;
};

#endif
