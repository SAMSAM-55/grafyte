#pragma once

#include <functional>

#include "types.h"
#include "Scene/Managers/Shapes.h"

namespace grafyte {
    class Scene;
}

namespace grafyte::collision {
    struct GridCoord {
        int x;
        int y;

        bool operator==(const GridCoord &) const = default;
    };

    struct GridCoordHash {
        std::size_t operator()(const GridCoord& c) const {
            const std::size_t h1 = std::hash<int>{}(c.x);
            const std::size_t h2 = std::hash<int>{}(c.y);

            return h1 ^ (h2 << 1);
        }
    };

    class SpatialGrid {
    public:
        explicit SpatialGrid(const float cellSize): m_cellSize(cellSize) {}

        void clear() {
            m_cells.clear();
        }

        void insert(const types::ObjectId& id, const AABB& aabb);

        [[nodiscard]] std::vector<types::ObjectId> queryCandidates(const AABB& area) const;
    private:
        float m_cellSize;
        std::unordered_map<GridCoord, std::vector<types::ObjectId>, GridCoordHash> m_cells;

        [[nodiscard]] int toCell(const float value) const {return static_cast<int>(std::floor(value / m_cellSize));}
    };

    }

