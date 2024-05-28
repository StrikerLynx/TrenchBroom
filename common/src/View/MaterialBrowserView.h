/*
 Copyright (C) 2010-2017 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "NotifierConnection.h"
#include "Renderer/FontDescriptor.h"
#include "Renderer/GLVertexType.h"
#include "View/CellView.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

class QScrollBar;

namespace TrenchBroom::Assets
{
class Material;
class MaterialCollection;
} // namespace TrenchBroom::Assets

namespace TrenchBroom::View
{

class GLContextManager;
class MapDocument;
using MaterialGroupData = std::string;

enum class MaterialSortOrder
{
  Name,
  Usage
};

class MaterialBrowserView : public CellView
{
  Q_OBJECT
private:
  std::weak_ptr<MapDocument> m_document;
  bool m_group = false;
  bool m_hideUnused = false;
  MaterialSortOrder m_sortOrder = MaterialSortOrder::Name;
  std::string m_filterText;

  const Assets::Material* m_selectedTexture = nullptr;

  NotifierConnection m_notifierConnection;

public:
  MaterialBrowserView(
    QScrollBar* scrollBar,
    GLContextManager& contextManager,
    std::weak_ptr<MapDocument> document);
  ~MaterialBrowserView() override;

  void setSortOrder(MaterialSortOrder sortOrder);
  void setGroup(bool group);
  void setHideUnused(bool hideUnused);
  void setFilterText(const std::string& filterText);

  const Assets::Material* selectedTexture() const;
  void setSelectedTexture(const Assets::Material* selectedTexture);

  void revealTexture(const Assets::Material* texture);

private:
  void reloadTextures();

  void doInitLayout(Layout& layout) override;
  void doReloadLayout(Layout& layout) override;

  void addTexturesToLayout(
    Layout& layout,
    const std::vector<const Assets::Material*>& textures,
    const Renderer::FontDescriptor& font);
  void addTextureToLayout(
    Layout& layout,
    const Assets::Material* texture,
    const Renderer::FontDescriptor& font);

  std::vector<const Assets::MaterialCollection*> getCollections() const;
  std::vector<const Assets::Material*> getTextures(
    const Assets::MaterialCollection& collection) const;
  std::vector<const Assets::Material*> getTextures() const;

  std::vector<const Assets::Material*> filterTextures(
    std::vector<const Assets::Material*> textures) const;
  std::vector<const Assets::Material*> sortTextures(
    std::vector<const Assets::Material*> textures) const;

  void doClear() override;
  void doRender(Layout& layout, float y, float height) override;
  bool doShouldRenderFocusIndicator() const override;
  const Color& getBackgroundColor() override;

  void renderBounds(Layout& layout, float y, float height);
  const Color& textureColor(const Assets::Material& texture) const;
  void renderTextures(Layout& layout, float y, float height);

  void doLeftClick(Layout& layout, float x, float y) override;
  QString tooltip(const Cell& cell) override;
  void doContextMenu(Layout& layout, float x, float y, QContextMenuEvent* event) override;

  const Assets::Material& cellData(const Cell& cell) const;
signals:
  void textureSelected(const Assets::Material* texture);
};

} // namespace TrenchBroom::View
