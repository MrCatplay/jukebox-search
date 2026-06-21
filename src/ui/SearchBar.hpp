#pragma once

#include <string>

#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/ui/TextInput.hpp>

namespace jsearch {
    class SearchBar final : public cocos2d::CCNode {
        public:
            static SearchBar* create(cocos2d::CCNode* popup);

        private:
            bool init(cocos2d::CCNode* popup);
            void applyFilter(const std::string& query);

            cocos2d::CCNode* m_popupRef = nullptr;
            geode::TextInput* m_input   = nullptr;
    };
}