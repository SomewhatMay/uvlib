# uvlib &middot; ![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/SomewhatMay/uvlib/c-cpp.yml?logo=githubactions&logoColor=ffffff&style=plastic) ![GitHub release (latest by date)](https://img.shields.io/github/release/SomewhatMay/uvlib?logo=github&style=plastic) ![License](https://img.shields.io/github/license/SomewhatMay/uvlib?logo=apache&style=plastic) ![GitHub code size](https://img.shields.io/github/languages/code-size/SomewhatMay/uvlib?style=plastic)

uvlib is an open-source PROS library that brings the Command-Based Programming paradigm in an intuitive and creative way.
* **Declarative**: You're more than just a programmer; you're a designer and an engineer. Just explain what you want done, let uvlib worry about the how.
* **Command-Based**: Rediscover the DRY principle—the oldest, and perhaps most important, programming concept—all over again in PROS. Keep your code clean, organized, and reusable across multiple seasons.
* **Extensible**: Built to grow with your projects. Easily integrate new features, customize behaviors, and adapt uvlib to your evolving needs.

## 📦 Installation 
### ✅ Automatic Installation (recommended)
Installing is as simple as 1, 2, and 3:
```bash
pros c add-depot uvlib https://raw.githubusercontent.com/SomewhatMay/uvlib/refs/heads/depot/stable.json # add the remote depot
pros c apply uvlib # install uvlib as a template
```

### ⬇️ Manual installation (not recommended)
1. Download the [latest release from GitHub releases](https://github.com/SomewhatMay/uvlib/releases)
2. Unzip the compressed archive
3. Copy and paste the contents of the file into your project directory

### 🔄️ Updating
If you've already got uvlib and would like to switch to the latest version, simply run:
```bash
pros c update
```

### ❌ Uninstalling
We're sorry to see you go. If you're encountering an issue, be sure to [create an issue](https://github.com/SomewhatMay/uvlib/issues). To uninstall, simply run:
```bash
pros uninstall uvlib
```

## 📖 Documentation
*Currently under construction. See imbedded comments in code.*

## 🔥 Inspiration
Much of this project was inspired by [WPILib](https://docs.wpilib.org/en/stable/index.html), the library trusted by the FIRST Robotics Competition. Many of the design decisions were made to align with WPILib's Command-Based Programming structure. You can learn more about the paradigm [here](https://docs.wpilib.org/en/stable/docs/software/commandbased/index.html).
