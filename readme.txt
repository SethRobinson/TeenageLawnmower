Teenage Lawnmower Source code readme
-----------------------------

This is an old indie 3D "simulation" originally released in 2002 or 2003.  It was a 2003 IGF finalist.

It includes a 3D engine Seth wrote, as well as a scripting engine Seth wrote.  (Same engine that was used in Funeral Quest)

Note: You can edit the .c files directly in game/script and game/scriptg to massively modify the game without actually recompiling the real binaries, the scripting system can use .c files directory. I've modified the build slightly so it won't "compile" and pack them into a resource file so it's easier to play with if anyone wanted.  If you're wondering why you can't find any .cpp files for squirrels and stuff, it's because it was all done in script files, not the .cpp base game files.

If you google it, you might be able to find a postmortem written about it on codedojo.com, my website.  (I plan to write it ...)

The sound/art is trickier so I can't really write a license to that as some of it is licensed from sound effect libraries and such.  I didn't include Photoshop and 3ds max files as they aren't required to play - all required game data is included.

The main engine/game is called SW and the editor is called SWED.

Linking/libs requirements: (I only tested the Main game, worked with Visual Studio C++ 2005)

Main game (SW): 

- DirectX 8 SDK
- FMOD 3.75

Editor (SWED):

 - DirectX 8 SDK
 - FMOD 3.75
 - ODE Physics (a version from 2003?)  The engine supports a lot more than what was used in TLM.
 - MFC (might be an optional install with Visual C++)


Do what you want with the source (the parts I wrote I least).
Feel free to port it to any system or distribute however.  Please don't charge for it.
Teenage Lawnmower is still Copyright 2003 Robinson Technologies, so if you distribute this yourself you should probably use a slightly different name.
 
-Seth A. Robinson (seth@rtsoft.com)

www.codedojo.com
www.rtsoft.com

Additional credits:

- The "Oil" mod was written by Dan Walm, mods were disabled but I included it in the mods directory
- Nearly all models and artwork was created by Akiko Robinson
- Japanese translation by Akiko Robinson
- Compressed File Library 3 (CFL) created by Jari Komppa

