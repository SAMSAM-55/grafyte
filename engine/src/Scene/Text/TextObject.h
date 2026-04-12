#pragma once

#include <memory>
#include <string>

#include "types.h"

namespace grafyte
{
class Scene;

class TextObject
{
  public:
    explicit TextObject(const std::shared_ptr<Scene> &scene, const types::ObjectId &id);
    ~TextObject();

    void setText(const std::string &text) const;
    void setScale(const float &scale) const;
    void setColor(const types::Color4 &color) const;
    void remove() const;

  private:
    std::weak_ptr<Scene> m_Scene;
    types::ObjectId m_Id;
};
} // namespace grafyte
