#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
using namespace geode::prelude;

#define SETTING(type, key_name) Mod::get()->getSettingValue<type>(key_name)

#include  <random>
#include  <iterator>
namespace geode::utils {
    template<typename Iter, typename RandomGenerator>
    Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(g));
        return start;
    };
    template<typename Iter>
    Iter select_randomly(Iter start, Iter end) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return select_randomly(start, end, gen);
    }
    template<typename Iter>
    auto select_randomly(Iter iter) {
        return *select_randomly(iter.begin(), iter.end());
    }
    bool rndb(int variants = 2) {
        auto varsVec = std::vector<bool>();
        auto tempb = true;
        auto tempvariants = variants;
        while (tempvariants > 0) {
            tempb = !tempb;
            tempvariants = tempvariants - 1;
            varsVec.push_back(tempb);
        }
        auto rtn = *select_randomly(varsVec.begin(), varsVec.end());
        //log::debug("{}({}) = {} of {}", __func__, variants, rtn, varsVec);
        return rtn;
    }
};

#include <Geode/modify/MenuGameLayer.hpp>
class $modify(MenuGameLayerExt, MenuGameLayer) {
    $override void update(float p0) {
        MenuGameLayer::update(p0);

        if (SETTING(bool, "Freeze Background")) {
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

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObjectExt, PlayerObject) {
    void updatePlatMovements(float) {
        auto plr = this;

        plr->m_isPlatformer = 1;

        //random l/r going
        if (rndb()) plr->m_holdingLeft = rndb();
        else plr->m_holdingRight = rndb();

        //dont go left a lot
        plr->m_holdingLeft = plr->getPosition().x < 5 ? false : plr->m_holdingLeft;
        plr->m_holdingRight = (plr->getPosition().x < 5 and SETTING(bool, "Off-screen Protection")) ? true : plr->m_holdingRight;//go back to screen!
    }
    void runaway(float) {
        auto plr = this;
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
    $override bool init(int p0, int p1, GJBaseGameLayer * p2, cocos2d::CCLayer * p3, bool p4) {
        if (!PlayerObject::init(p0, p1, p2, p3, p4)) return false;

        if (typeinfo_cast<MenuGameLayer*>(p3)) {

            if (SETTING(bool, "Run Away From Mouse")) {
                this->schedule(schedule_selector(PlayerObjectExt::runaway), 0.001);
            };
            this->schedule(schedule_selector(PlayerObjectExt::updatePlatMovements), 0.35f);
            this->m_isPlatformer = 1;

        }

		return true;
    }
};