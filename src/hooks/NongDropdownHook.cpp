#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/modify/FLAlertLayer.hpp>

#include "ui/SearchBar.hpp"

using namespace geode::prelude;

class $modify(JBSearchLayer, FLAlertLayer) {
    void show() {
        if (m_mainLayer && m_mainLayer->getChildByIDRecursive("manifest-label") && !m_mainLayer->getChildByIDRecursive("jbs-search-bar")) {
            if (jsearch::SearchBar* bar = jsearch::SearchBar::create(this)) {
                bar->setID("jbs-search-bar");
                m_mainLayer->addChildAtPosition(bar, Anchor::Bottom, {0.f, bar->getContentHeight() * 0.5f + 2.f});
                bar->setZOrder(20);
            }
        }

        FLAlertLayer::show();
    }
};