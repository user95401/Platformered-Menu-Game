#### Mod developer was banned on Geode for dumb reasons, but he still keeps his mods up-to-date. <br>See for working latest versions at [releases page](/releases)!
> Yes, u can't find it in geode servers, download it and install manually.

# Platformered Menu Game
Player in main menu is in Platformer Mode now!

That mod modify `MenuGameLayer` that being used in `MenuLayer`.
MenuLayer is a 2nd layer that you see when game was just launched.

In `MenuGameLayer::update` method, that calls on every frame, mod freezes background and ground. That can be disabled in settings.

In `MenuGameLayer::tryJump` method, that calls every ~100ms, mod set player object variable that stands for platformer mode. And also randomly changes variables about holding left/right.

Mod have also updating schedule that make player object run away from mouse. It can be disabled in settings.

<img src="logo.png" width="150" alt="the mod's logo" />

*Updated logo.png to change mod's icon (yippee)*

## Getting started
We recommend heading over to [the getting started section on geode docs](https://docs.geode-sdk.org/getting-started/) for useful info on what to do next.

## Build instructions
For more info, see [our docs](https://docs.geode-sdk.org/getting-started/create-mod#build)
```sh
# Assuming you have the Geode CLI set up already
geode build
```

# Resources
* [Geode SDK Documentation](https://docs.geode-sdk.org/)
* [Geode SDK Source Code](https://github.com/geode-sdk/geode/)
* [Geode CLI](https://github.com/geode-sdk/cli)
* [Bindings](https://github.com/geode-sdk/bindings/)
* [Dev Tools](https://github.com/geode-sdk/DevTools)
