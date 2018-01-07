# About

# Prerequisites

## Hardware

Check [here](https://docs.unrealengine.com/latest/INT/GettingStarted/RecommendedSpecifications/)
for the minimum hardware recommended by Unreal Engine.

## Toolchain

You will need Unreal Engine 4 (UE4). We are attempting to use the latest version, which as of the time of this
writing is UE4.18.2.  

Windows users will need Visual Studio Community (we're using the latest version, 2017).
If you don't have UE4 or Visual Studio 2017 installed, these
[instructions](https://docs.unrealengine.com/latest/INT/Programming/Development/VisualStudioSetup/#visualstudio2017users) 
will help get you started. If you've already been using C++ to develop video games with
older versions of these tools (Visual Studio 2015, UE 4.16), we recommend sticking with those, as we've found that
the differences between Visual Studio 2015 and 2017 can cause problems for UE4 if you're not careful (inability
to generate a .sln file from a .uproject file, inability to compile source code, etc.).

Linux users should follow these [instructions](https://wiki.unrealengine.com/Building\_On\_Linux).  If (like us) you like
to build your projects from the bash command line, follow these
[instructions](https://forums.unrealengine.com/development-discussion/c-gameplay-programming/97022-linux-how-to-compile-c-scripts-from-terminal) to add a bash-shell command allowing you to do this.

# Build

You should first clone the UE4_OpencCV repository into your <b>Documents/Unreal Projects</b> folder, creating
that folder if it doesn't already exist.

Navigate to the UE4_OpenCV folder, right-click on
<b>UE4_OpenCV.uproject</b>, and select the menu item <b>Generate Visual Studio project files</b>.  This
should create a file <b>UE4_OpenCV.sln</b> and some new folders.
If you don't get the new .sln file, it's probably because UE4 cannot find a C++ compiler for
the version of Visual Studio you've installed (see discussion 
[here](https://docs.unrealengine.com/latest/INT/Programming/Development/VisualStudioSetup/#beforesetting-upyourue4-to-vsworkflow)).  Double-clicking on the .sln file should launch Visual Studio.  The first time you
launch Visual Studio, it can take several minutes to parse up all the C++ source
code for the UE4 engine.  

Once Visual Studio is done parsing the UE4 source code, do <b>Build / Build Solution</b>.  This will create
a <b>Binaries</b> folder.  Navigate to <b>ThirdParty/OpenCV/Libraries/Win64</b>, and copy/paste the 
file <b>opencv_world340.dll</b> to the new <b>Binaries</b> folder.  Finally, hit the <b>F5</b> key to launch
launch the UE4 editor, and press the Play button. In the upper-left of the display you should see a little 
camera view from the character's perspective, along with a report of the frame rate (FPS).
