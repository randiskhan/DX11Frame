DX11Frame
=========

Basic DirectX 11.0 rendering framework.

The primary purpose of this project is for me to get more practice with DriectX 11 rendering. I'm keeping this project at DirectX 11.0, as I need it to run on Windows 7 machines that have not had the update to allow DirectX 11.1.

Any executable project using this static library only needs to:
1) Include CAppBase.h.
2) Link dx11frame.lib.
3) Create a class that extends CAppBase.
4) Implement WinMain, instantiate your class, and call Run().

This project is dependant on DirectX Tool Kit (http://directxtk.codeplex.com/). A simple DirectXTK SDK (headers, libraries, and tools) is included in the repository built by me from DirectXTK source unmodified.

randiskhan (Randolph Stone) git@randskhan.com

150223: Although this project lately has been used more for playing with the math to generate and render cycloids (specifically hypotrochoids), the goal is still to keep the engine as a library for use in other projects of mine.

150212: Added a #if to CApp.cpp to use windowed@800x800 mode when running a debug build, fullscreen@1280x1024 when running release build. This made it easier for me to switch between the two.
