/*
 Copyright (C) 2010-2014 Kristian Duske
 
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

#ifndef __TrenchBroom__WorldReader__
#define __TrenchBroom__WorldReader__

#include "IO/MapReader.h"

namespace TrenchBroom {
    namespace Model {
        class BrushContentTypeBuilder;
    }
    
    namespace IO {
        class WorldReader : public MapReader {
        private:
            const Model::BrushContentTypeBuilder* m_brushContentTypeBuilder;
            Model::World* m_world;
        public:
            WorldReader(const char* begin, const char* end, const Model::BrushContentTypeBuilder* brushContentTypeBuilder, Logger* logger = NULL);
            WorldReader(const String& str, const Model::BrushContentTypeBuilder* brushContentTypeBuilder, Logger* logger = NULL);

            Model::World* read(const BBox3& worldBounds);
        private: // implement MapReader interface
            Model::ModelFactory* initialize(Model::MapFormat::Type format, const BBox3& worldBounds);
            Model::Node* onWorldspawn(const Model::EntityAttribute::List& attributes, const ExtraAttributes& extraAttributes);
            void onWorldspawnFilePosition(size_t lineNumber, size_t lineCount);
            void onLayer(Model::Layer* layer);
            void onNode(Model::Node* parent, Model::Node* node);
            void onUnresolvedNode(const ParentInfo& parentInfo, Model::Node* node);
            void onBrush(Model::Node* parent, Model::Brush* brush);
        };
    }
}

#endif /* defined(__TrenchBroom__WorldReader__) */
