#include "SearchBar.hpp"

#include <algorithm>
#include <cctype>
#include <string>

#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/cocos/label_nodes/CCLabelBMFont.h>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

namespace jsearch {
    namespace {
        bool matchesQuery(const std::string& query, const std::string& name, const std::string& artist) {
            auto lower = [](std::string s) {
                std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
                return s;
            };
            
            const std::string q = lower(query);
            return lower(name).find(q) != std::string::npos || lower(artist).find(q) != std::string::npos;
        }
    }

    SearchBar* SearchBar::create(cocos2d::CCNode* popup) {
        auto ret = new SearchBar();

        if (ret->init(popup)) {
            ret->autorelease();
            return ret;
        }
        delete ret;

        return nullptr;
    }

    bool SearchBar::init(cocos2d::CCNode* popup) {
        if (!CCNode::init()) { return false; }

        m_popupRef = popup;
        this->ignoreAnchorPointForPosition(false);
        this->setAnchorPoint({0.5f, 0.5f});

        m_input = TextInput::create(290.f, "Search...", "bigFont.fnt");
        m_input->setMaxCharCount(64);
        m_input->setCallback([this](const std::string& text) { this->applyFilter(text); });
        this->addChild(m_input);

        const cocos2d::CCSize sz = m_input->getContentSize();
        this->setContentSize(sz);
        m_input->setPosition(sz / 2.f);

        if (cocos2d::extension::CCScale9Sprite* bg = cocos2d::extension::CCScale9Sprite::create("square02_001.png")) {
            bg->setContentSize(sz + cocos2d::CCSize{4.f, 4.f});
            bg->setColor({30, 15, 5});
            bg->setOpacity(255);
            bg->setZOrder(-1);
            this->addChildAtPosition(bg, Anchor::Center);
        }

        return true;
    }

    void SearchBar::applyFilter(const std::string& query) {
        ScrollLayer* scrollLayer = typeinfo_cast<ScrollLayer*>(m_popupRef->getChildByIDRecursive("list"));
        if (!scrollLayer) { return; }

        cocos2d::CCLayer* content = scrollLayer->m_contentLayer;
        if (!content) { return; }

        cocos2d::CCArray* children = content->getChildren();
        if (!children || children->count() == 0) { return; }

        const bool active = !query.empty();
        const unsigned int count = children->count();

        for (unsigned int i = 0; i < count; ++i) {
            cocos2d::CCNode* cell = static_cast<cocos2d::CCNode*>(children->objectAtIndex(i));
            cocos2d::CCNode* nameNode   = cell->getChildByIDRecursive("song-info-label");
            cocos2d::CCNode* artistNode = cell->getChildByIDRecursive("artist-label");

            bool show;
            if (!nameNode && !artistNode) {
                show = !active;
            } else if (!active) {
                show = true;
            } else {
                std::string name, artist;
                if (cocos2d::CCLabelBMFont* lbl = typeinfo_cast<cocos2d::CCLabelBMFont*>(nameNode)) { name = lbl->getString(); }
                if (cocos2d::CCLabelBMFont* lbl = typeinfo_cast<cocos2d::CCLabelBMFont*>(artistNode)) { artist = lbl->getString(); }
                show = matchesQuery(query, name, artist);
            }

            cell->setVisible(show);
            cell->setScaleY(show ? 1.f : 0.f);
        }

        SimpleAxisLayout* layout = SimpleColumnLayout::create()
            ->setMainAxisDirection(AxisDirection::TopToBottom)
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisScaling(active ? AxisScaling::None : AxisScaling::Grow)
            ->setGap(5.f);
        layout->ignoreInvisibleChildren(active);
        content->setLayout(layout);
        content->updateLayout();
        content->setPositionY(
            -content->getContentHeight() + scrollLayer->getContentHeight());
    }
}