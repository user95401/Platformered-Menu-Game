#include <_main.hpp>
#include <_updater.hpp>

#include <Geode/modify/MenuGameLayer.hpp>
class $modify(MenuGameLayerExt, MenuGameLayer) {
    void runaway(float) {
        auto plr = this->m_playerObject;
        //runaway from mouse
        auto mousePos = getMousePos();
        auto playrPos = plr->getPosition();
        auto mousezone = CCSize(1, 1) * SETTING(double, "Mouse Zone");
        auto mouseRect = CCRect(mousePos - mousezone, mousePos + mousezone);
        auto playrRect = plr->getObjectRect();
        if (playrRect.intersectsRect(mouseRect)) {
            //log::warn("{} intersectsRect {}", playrRect, mouseRect);
            plr->m_holdingLeft = playrPos.x < mousePos.x;
            plr->m_holdingRight = playrPos.x > mousePos.x;
            if (!plr->m_isSpider && !plr->m_isBall) plr->m_jumpBuffered = playrPos.y + (mousezone.height / 2) > mousePos.y;
        }
    }
    $override bool init() {
        auto init_result = MenuGameLayer::init();

        if (SETTING(bool, "Runaway From Mouse")) {
            this->schedule(schedule_selector(MenuGameLayerExt::runaway), 0.001);
        };

        return init_result;
    }
    $override void update(float p0) {
        MenuGameLayer::update(p0);

        if (SETTING(bool, "Freese Background")) {
            m_backgroundSpeed = 0.1;
            findFirstChildRecursive<CCNode>(m_groundLayer,
                [](CCNode* node) {
                    auto parent_id = node->getParent()->getID();
                    auto __this_id = node->getID();
                    if (string::contains(parent_id, "ground-sprites")) {
                        //node->stopAllActions();
                        node->setPosition(CCPointZero);
                    }

                    return false;
                }
            );
        };
    }
    $override void tryJump(float p0) {
        MenuGameLayer::tryJump(p0);
        auto plr = this->m_playerObject;
        
        plr->m_isPlatformer = 1;
        //random l/r going
        if (rndb()) plr->m_holdingLeft = rndb();
        else plr->m_holdingRight = rndb();
        //dont go left a lot
        plr->m_holdingLeft = plr->getPosition().x < 5 ? false : plr->m_holdingLeft;
        plr->m_holdingRight = plr->getPosition().x < 5 ? true : plr->m_holdingRight;//go back to screen!
        
    }
    $override void resetPlayer() {
        MenuGameLayer::resetPlayer();

        auto plr = this->m_playerObject;
        if (!plr) return;

        if (SETTING(bool, "No Wave") and plr->m_isDart) return this->resetPlayer();
        if (SETTING(bool, "No Swing") and plr->m_isSwing) return this->resetPlayer();

        if (SETTING(bool, "Support Jetpacks") and plr->m_isShip) {

            std::vector<int> ids;
            for (int i = 0; i <= GameManager::get()->countForType(IconType::Jetpack); i++)
                ids.push_back(i);

            plr->updatePlayerJetpackFrame(*select_randomly(ids.begin(), ids.end()));

        }
    }
};