LOD Mixer for PES/WE                                            July 8, 2010
=============================================================================
Version 1.9


1. SUPPORTED GAME VERSIONS
--------------------------
PC: PES5, PES5 Demo 2(no voice), PES4, WE8I. 
XBOX: WE8I, PES5, PES5-beta, WE9

NOTE: Not all features are supported in all game versions on all platforms.
For example, "crowd-fix for all cameras" and League/Cup/ML controls only
apply to PES5/WE9, since PES4/WE8I didn't have those problems. Also, most
of the League/Cup/ML controls are so far only available for PC-version of 
PES5, with the exception of weather, which also works for Xbox.


2. INTRODUCTION
---------------
This tool was initially just for Level-of-Detail adjustment for player and
goalkeeper models, but now has evolved into more general graphics/gameplay
tweak tool. In addition to LOD, you can now turn on the crowd rendering on
all cameras, and also control various aspects of League/Cup/Master League
matches, such as: weather, stadium selection, crowd attendance, and more.


3. THE IDEA OF LOD
------------------
This is by no means a formal definition of Level-of-Detail (LOD) idea, but
merely a short introduction to the concept, which will hopefully be helpful 
in understanding what the LOD Mixer program does, and how to use it.
Typically (and PES5 is no exception), a rendering engine in 3D games is
designed to take advantage of the fact that objects that look small need
simpler models than objects that look big. A close-up view of a car may
require thousands of polygons to look convincing, but may only need a few
hundred when the distance between it and the camera is big and the amount
of pixels it occupies on the screen is small. By using simpler models for 
far-away objects, the game engine saves GPU cycles that could be used for
creating additional visual effects, or just to maintaining a smooth frame
rate. Level-Of-Detail algorithm is the logic that decides when to use
a particular model for a particular object. 

For the task of rendering players and goalkeepers, PES5 uses 5 different 
levels of detail, which are dynamically switched depending on a set of 
factors: distance from camera to player, playing mode, active player, etc.

#1: Very high-detail models, big (512x256) textures",
#2: High-detail models, medium (256x128) textures",
#3: Medium-detail models, medium (256x128) textures",
#4: Low-detail models, small (128x64) textures",
#5: Very low-detail models, small (128x64) textures",

LOD #1 is used for cut-scenes and more in replays than in actual game. It's
the only one that uses best quality kit textures - 512x256. LOD #2 and #3
are typically used for many cameras, including "Normal Long" and "Wide". 
LOD #2 is also used for some cut-scenes - like pre-match photos, for example,
and also in replays. LOD #4 - on "Wide" and "Bird's Eye", and LOD #5 i was 
only able to see with "Bird's-Eye" cameras.

To illustrate the concept, consider this scenario:
The camera is really close to the player, so the game engine selects LOD #1
to show the very well detailed model of the player with nice-looking textures
As the player moves away from the camera, at some point the game engine 
decides that the very high level of detail is not needed anymore, and it 
should be switched to next one - LOD #2, since the camera is now 
far enough and the user won't notice the difference. Here's where the problem
occurs with PES5, PES4 and WE8I: we actually DO see the 
difference, because KONAMI implemented a very aggressive LOD algorithm and it 
switches to lower-detail models way too early, and that's why we see annoying 
artifacts, as player balding and low-quality kit textures. I can only guess 
why they did it, and i think it's because the lowest denominator system 
- PS2 - couldn't sustain the smooth frame rate with a more concervative 
LOD switching algorithm.


4. HOW TO USE THE PROGRAM
-------------------------

LOD Mixer should automatically find your PES5/PES4/WE8I installations and 
display the pathnames in a drop-down list. If the program for some reason
cannot find your game, or if you simply want to manually specify the filename,
type it into the "EXE:" textbox, and press [Tab] key. (the filename that you
enter doesn't have to be a full absolute pathname, but can be short one -
LOD mixer will treat it relative to its current directory.) 

For XBOX users: put the "default.xbe" file into the same directory where
you have lodmixer.exe. Start lodmixer.exe. In the "EXE:" textbox, manually 
type in "default.xbe", then press [Tab] key. You should see the EXE Info text
change to "PES5 XBOX" (or "PES5 XBOX Beta" or "WE9 XBOX"), and the 
LOD configuration should get displayed in the drop-down lists.


4.1. LOD LEVELS
---------------

If you running the program the very first time, you'll see the default LOD 
configuration: 1,2,3,4,5. This means that the LOD algorithm will use #1-models 
for LOD #1, #2-models for LOD #2, and so on. Change that configuration by 
selecting different items in drop-down boxes, press "Save" button, and 
that's it. You can now close the LOD Mixer and start the game.

The idea of changing the default setup, is to force the game engine to 
use higher-detail models for the cases when it used simpler ones, and vice 
versa. For example, you can force it to use best-detail models for LOD #1,2,3
and keeping the defaults for LOD#4 and 5.  
This is the configuration for such example: 1,1,1,4,5. 
Or you can choose 5,5,5,5,5 which will result in really simplified models 
used for all cases. (This extreme config looks kinda funny :-), if for 
instance your PC is not powerful and you want to sacrifice some visual 
quality for smooth frame rate. Typically though, if you have a good PC, you'd
want to use something like 1,1,2,2,3.

Now, for all of you, who have honestly read the above sections and has
difficulty understanding the concept, don't be discouraged. LOD Mixer is a
very simple program: you can use it in a purely "black-box" experimental way:
select a random configuration, press "Save" button, close the LOD Mixer, 
start the game and see how your changes affect the display. 

You can also try 1,1,1,1,1, if you have a powerful machine. That will give
you the configuration when the most detailed models and largest textures
are used always.

For me the following configuration works quite well: 1,1,1,2,2

(Which means that the five drop-downs look like this:

LOD #1 - #1: Very high-detail models, big (512x256) textures",
LOD #2 - #1: Very high-detail models, big (512x256) textures",
LOD #3 - #1: Very high-detail models, big (512x256) textures",
LOD #4 - #2: High-detail models, medium (256x128) textures",
LOD #5 - #2: High-detail models, medium (256x128) textures",

Numbers on shorts (and - for national teams - on players chests) are 
always there, no more "fake" baldness, captain armbands are always visible, 
cut-scenes (pre-match photos and etc.) have the highest quality. On my PC, 
i didn't experience any slowdown with this configuration, but you'll 
have to try it on yours to see how it deals with the task of rendering 
more polygons.

In a way this is a fun experiment, which gives you an opportunity to
affect the game engine.


4.2. CROWD SWITCH
-----------------
Yes, you can now see the crowd on all the cameras! 
PES5 rendering engine disables crowd display on the following cameras:
Normal Medium/Long, Wide, Bird's-Eye, Vertical Medium/Long. This was really
annoying, because Normal Long and Wide are probably the two most popular
cameras, and without the crowd, the match athmosphere is lost. By setting the
"Show crowd on all cameras" checkbox, you can fix that problem. Just check 
the checkbox, and hit the "Save" button. Not much more to say here. :)


4.3. LEAGUE/CUP/MASTER LEAGUE controls
--------------------------------------

I. Weather
~~~~~~~~~~

Although it hasn't been formally proved, but so far the evidence shows
that in PES5, the weather is always fine in Cups, Leagues, and 
Master League. There appears to be a bug in the program, which disables
weather changes in those modes, and as a result, the only way to see the
weather effects - such as snow and rain - is to use Match Mode (exhibition
game). 
LOD Mixer allows you to fix that: with the "Weather" drop-down, you can 
either enforce specific weather that you want to occur in Cup/League/ML 
modes, or use one of the 3 random weather algorithms:

1. Konami Random/Seasonal - weather selection algorithm (which seems
to had been disabled by default, but LOD mixer unlocks it). I haven't
spend much time to try to understand the logic of this algorithm, but from
experiements, it looks like the rain and snow do occur, but very rarely.

2. Mild Random/Seasonal - random weather selection algorithm. This is 
actually a true random algorithm (not based on pseudo-random number generator),
which uses frame counter as its source of randomness. The algorithm picks
the weather based on these probabilities:
SUMMER: Fine - 10/16, Rain - 6/16
WINTER: Fine - 8/16, Rain - 4/16, Snow - 4/16

3. Cold Random/Seasonal - same algorithm as "Mild", but with a different
set of probabilities, so that the weather on average is more miserable.
(Meaning that you should get rain and snow more often)
SUMMER: Fine - 8/16, Rain - 8/16
WINTER: Fine - 4/16, Rain - 5/16, Snow - 7/16

NOTE: Starting with version 1.8, the weather control is also supported on
Xbox, but only Konami random weather algorithm is available. So the 
choices are: Fine, Rain, Snow, Konami Random/Seasonal.

II. Other settings
~~~~~~~~~~~~~~~~~~

Sometimes you may want to enforce a particular set of settings in a league 
or cup or master league match. For example, if you're playing the World Cup
(a.k.a. International Cup), and you managed to get to the final, you'd
probably want to make sure that the stadium is full of spectators, and
maybe you want to pick a particular stadium or time-of-the-day to play
the final match. The game doesn't provide the means to do all that, but
with LOD Mixer you can. For each setting, you can either force a particular
value, of let the game logic decide.

(BTW, if you want to force snow on Oita Big Eye stadium - you're out of luck,
because it's a stadium with a closed roof! Even though the weather outside
will be snowy and cold, the pitch will still be in perfect condition. Just
thought it was a nice little detail, well implemented by Konami. :-)

All of the controls are pretty much self-explanatory, so the best way to
learn them is to try and see what happens. After you make changes, don't
forget to press "Save" button, then exit LOD mixer, and you're ready to
play.

Should things go wrong, or if you simply want to go back to original
settings, simply choose "Default game logic" item in all controls, then
press "Save", and you're done. (Of course it never hurts to have a
backup copy of your PES5.exe - just in case - but that you should do
before you make any changes with LOD Mixer.)


5. The "Defaults" button
------------------------

Pressing "Defaults" button will reset everything to original state, as
things were in original exe-file: LODs will become 1,2,3,4,5, Camera fix
will be disabled, and all advanced controls for League/Cup/ML modes will
be back to "Default game logic" selections. Remember, that you still 
need to press "Save" to write those changes into the executable file.
Doing so effectively uninstalls any LOD mixer mods.


6. CREDITS
----------
Game algorithms analysis and programming by Juce.
Beta testing: Biker Jim, The Wolf

