#include "SpatialGrid.h"

#include <algorithm>
#include <ranges>
#include <unordered_set>

#include "Scene/Scene.h"

namespace grafyte::collision
{
void SpatialGrid::insert(const types::ObjectId &id, const AABB &aabb)
{
    const float minX = aabb.pos.x - aabb.width;
    const float maxX = aabb.pos.x + aabb.width;
    const float minY = aabb.pos.y - aabb.height;
    const float maxY = aabb.pos.y + aabb.height;

    const int cellMinX = toCell(minX);
    const int cellMaxX = toCell(maxX);
    const int cellMinY = toCell(minY);
    const int cellMaxY = toCell(maxY);

    auto &objectCells = m_ObjectsCells[id];
    objectCells.clear();
    objectCells.reserve(static_cast<std::size_t>((cellMaxX - cellMinX + 1) * (cellMaxY - cellMinY + 1)));

    for (int y = cellMinY; y <= cellMaxY; y++)
    {
        for (int x = cellMinX; x <= cellMaxX; x++)
        {
            m_Cells[{x, y}].push_back(id);
            objectCells.push_back({x, y});
        }
    }
}

std::vector<types::ObjectId> SpatialGrid::queryCandidates(const AABB &area) const
{
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

    for (int y = cellMinY; y <= cellMaxY; y++)
    {
        for (int x = cellMinX; x <= cellMaxX; x++)
        {
            auto it = m_Cells.find({x, y});
            if (it == m_Cells.end())
                continue;

            for (const auto &id : it->second)
            {
                if (seen.insert(id).second)
                    result.push_back(id);
            }
        }
    }

    return result;
}

void SpatialGrid::cleanDirty(const std::vector<types::ObjectId> &dirty)
{
    for (const auto &idToRemove : dirty)
    {
        const auto cellsIt = m_ObjectsCells.find(idToRemove);
        if (cellsIt == m_ObjectsCells.end())
            continue;

        for (const auto &ids : cellsIt->second)
        {
            const auto cellIt = m_Cells.find(ids);
            if (cellIt == m_Cells.end())
                continue;

            // Remove all instances of the id from the affected cell only.
            auto &vec = cellIt->second;
            const auto newEnd = std::ranges::remove(vec, idToRemove).begin();
            vec.erase(newEnd, vec.end());
        }

        m_ObjectsCells.erase(cellsIt);
    }
}
} // namespace grafyte::collision
