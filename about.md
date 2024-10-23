# Platformered Menu Game
Player in main menu is in Platformer Mode now!

That mod modify `MenuGameLayer` that being used in `MenuLayer`.
MenuLayer is a 2nd layer that you see when game was just launched.

In `MenuGameLayer::update` method, that calls on every frame, mod freezes background and ground. That can be disabled in settings.

In `MenuGameLayer::tryJump` method, that calls every ~100ms, mod set player object variable that stands for platformer mode. And also randomly changes variables about holding left/right.

Mod have also updating schedule that make player object run away from mouse. It can be disabled in settings.