DX11Frame
=========

Basic DirectX 11.0 rendering framework.

The primary purpose of this project is for me to get more practice with DriectX 11 rendering. I'm keeping this project at DirectX 11.0, as I need it to run on Windows 7 machines that have not had the update to allow DirectX 11.1.

Any executable project using this static library only needs to:
1) Include CAppBase.h.
2) Link dx11frame.lib.
3) Create a class that extends CAppBase.
4) Implement WinMain, instantiate your class, and call Run().

This project is dependant on DirectX Tool Kit (http://directxtk.codeplex.com/). A simple DirectXTK SDK (headers, libraries, and tools) is included in the repository.

randiskhan (Randolph Stone) git@randskhan.com
	
