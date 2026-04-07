//
// Created by samis on 4/6/2026.
//

#include "SpatialGrid.h"

#include <ranges>
#include <unordered_set>

#include "Scene/Scene.h"


namespace grafyte::collision {
    void SpatialGrid::insert(const types::ObjectId& id, const AABB& aabb) {
        const float minX = aabb.pos.x - aabb.width;
        const float maxX = aabb.pos.x + aabb.width;
        const float minY = aabb.pos.y - aabb.height;
        const float maxY = aabb.pos.y + aabb.height;

        const int cellMinX = toCell(minX);
        const int cellMaxX = toCell(maxX);
        const int cellMinY = toCell(minY);
        const int cellMaxY = toCell(maxY);

        for (int y = cellMinY; y <= cellMaxY; y++) {
            for (int x = cellMinX; x <= cellMaxX; x++) {
                m_cells[{x, y}].push_back(id);
            }
        }
    }

    std::vector<types::ObjectId> SpatialGrid::queryCandidates(const AABB &area) const {
        const float minX = area.pos.x - area.width;
        const float maxX = area.pos.x + area.width;
        const float minY = area.pos.y - area.height;
        const float maxY = area.pos.y + area.height;

        const int cellMinX = toCell(minX);
        const int cellMaxX = toCell(maxX);
        const int cellMinY = toCell(minY);
        const int cellMaxY = toCell(maxY);

        std::vector<types::ObjectId> result;
        std::unordered_set<types::ObjectId> seen;

        for (int y = cellMinY; y <= cellMaxY; y++) {
            for (int x = cellMinX; x <= cellMaxX; x++) {
                auto it = m_cells.find({x, y});
                if (it == m_cells.end()) continue;

                for (const auto& id: it->second) {
                    if (seen.insert(id).second) result.push_back(id);
                }
            }
        }

        return result;
    }

    void SpatialGrid::cleanDirty(const std::vector<types::ObjectId> &dirty) {
        for (const auto& id_to_remove: dirty) {
            for (auto& ids: m_cells | std::views::values) {
                std::vector<types::ObjectId>& vec = ids;
                // Remove all instances of the id
                const auto newEnd = std::remove(vec.begin(), vec.end(), id_to_remove);
                vec.erase(newEnd, vec.end());
            }
        }
    }
}
