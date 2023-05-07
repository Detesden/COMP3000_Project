# <a id="top"></a>COMP3000 - Reverse Engineering Shenanigans 🌌

[![C++](https://img.shields.io/badge/language-C%2B%2B-%23f34b7d.svg?style=for-the-badge&logo=appveyor)](https://en.wikipedia.org/wiki/C%2B%2B)
[![Windows](https://img.shields.io/badge/platform-Windows-0078d7.svg?style=for-the-badge&logo=appveyor)](https://en.wikipedia.org/wiki/Microsoft_Windows)
[![x86](https://img.shields.io/badge/arch-x86-red.svg?style=for-the-badge&logo=appveyor)](https://en.wikipedia.org/wiki/X86)

---

## 🌐 〢 Index
- [📖・Overview](#overview)
- [🎯・Features](#features)
- [⚠️・Warning](#warning)
- [🚀・Getting started](#start)
  - [📥・Download](#download)
  - [🔨・Building](#build)
  - [💻・Running](#run)
- [📚・Frameworks & Libraries](#libraries)
- [⚙️・Coding & Testing environment](#testEnv)
- [📝・Credits & Resources](#credits)
- [🎞️・Video Walkthrough](#walkthrough)

---

## <a id="overview"></a>📖 Overview

<p align="center">
  <img src="https://user-images.githubusercontent.com/72513569/236650454-e3d1a12e-c269-4922-b81a-0268d39aa19e.gif" />
</p>

The developed software presented by the project is a dynamic-link library that can be injected into a popular online game Counter-Strike: Global Offensive and run seamlessly inside its memory space. The game is a first-person shooter; therefore, the software offers features appropriate to that specific genre. The project aims to open up opportunities for fun experiences that would otherwise be unachievable by conventional means. Although it has grounds for potential misuse or negative consequences, it is meant and intended to be utilized in a positive and neutral setting.<br><br>

> This repository contains both the source code of the DLL cheat and the personal Injector.

---

## <a id="features"></a>🎯 Features

**Gameplay-assisting**
```
- Aim Bot
- Trigger Bot
- ESP
- Anti-Flash
```

**Neutral**
```
- Skin Changer (weapons only)
- Bunny Hop
- Static crosshair
```

---

## <a id="warning"></a>⚠️ Warning

The utilization of the cheating software presented by this project violates the game’s Terms of Service, which is punishable with a VAC ban on the user account. The user should take full responsibility and liability for any repercussions, given the inherently compromising nature of cheating.<br><br>

> To prevent that, CS:GO must be launched in ***insecure mode***.<br><br>
> 
> From Steam library, right-click on the CS:GO<br>
> Select 'Properties...'<br>
> In the LAUNCH OPTIONS dialogue box type '-insecure'<br>

---

## <a id="start"></a>🚀 Getting started

This section describes how to utilize the software.<br>
There are instructions for both downloading the prebuilt software and building it individually with Visual Studio.

### <a id="download"></a>📥 Download
- Head out to the [Releases](https://github.com/VladBurcaUNI/COMP3000_Project/releases) page and [download](https://github.com/VladBurcaUNI/COMP3000_Project/releases/download/v1.0.0/RevEngCSGO.rar) the latest release.
- Extract the archive.<br><br>

### <a id="build"></a>🔨 Building
- Clone the repository
- Open the **.sln** file
- Set the build configuration to **Release -> x86**
- Build the solution (Ctrl + Shift + B)<br><br>

 🔍 Getting the necessary files
- Go into ***\COMP3000_Project-main\bin\Win32\Release*** and copy:
  - *Cheat_DLL.dll*
  - *Injector.exe*<br><br>

### <a id="run"></a>💻 Running
- Open the CS:GO<br><br>

 💉 Injecting
- Run the ***Injector.exe***
- Follow the instructions.<br><br>

⌨️ **Software Keybinds**
| Key    | Action                         |
|:------:|--------------------------------|
| Insert | Show / Hide the menu           |
| END    | Eject the DLL                  |

---

## <a id="libraries"></a>📚 Frameworks & Libraries
[ImGui](https://github.com/ocornut/imgui) - The software's interface<br>
[MinHook](https://github.com/TsudaKageyu/minhook) - Hooking library<br>

---

## <a id="testEnv"></a>⚙️ Coding & Testing environment
**Operating system:** Windows 10 x64<br>
**IDE:** Visual Studio 2022 (v143)<br>
**C++ Language Standard:** C++23<br>

---

## <a id="credits"></a>📝 Credits & Resources
[UnKnoWnCheaTs](https://www.unknowncheats.me/) - Game Hacking forum with a ton of tutorials.<br>
[ReversEngineer](https://revers.engineering/) - Main learning source for applying Assembly knowledge into reverse engineering.<br>
[jbremer](https://jbremer.org/x86-api-hooking-demystified/) - Hooking techniques.<br>
[Source SDK 2013](https://github.com/ValveSoftware/source-sdk-2013) - Insights on the game engine's SDK.<br>

---

## <a id="walkthrough"></a>🎞️ Video Walkthrough
[YouTube](https://youtu.be/)

---

<p align="center">
<a href="#top">Back to the top</a>
</p>
