DX11FrameLib
============

Basic DirectX 11 rendering framework in a static library.

Any executable project using this static library only needs to:
1) Include CAppBase.h.
2) Link dx11frame.lib.
3) Create a class that extends CAppBase.
4) Implement WinMain, instantiate your class, and call Run().

Earlier commits had WinMain in the static library. This is no longer the case. Caused more problems than it solved. Solutions to keep WinMain in lib have been discovered, but complicate things more than I'd like at this projects current development. Thanks for your help Scott!

randiskhan (Randolph Stone) git@randskhan.com
