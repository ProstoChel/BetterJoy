ATTENTION! Temporary you can get last release on TG chat, not in autoUpdater.

You always can discuss BetterJoy in TG chat https://t.me/betterjoy and get some answers

If you like BetterJoy, you can support project with this 
https://www.paypal.me/betterjoy ;)
It's really hard to develop, you can check it. Also you can help with your code, it will be great. Just follow github.com/ProstoChel/BetterJoy

I work on:
-Regulated source of energy mode - it's not just powerbank, it can use any voltage/wattage/TC in the end or connect some devices
-Puff detection - in this mode you do not need press fire button, all you need is start to puff
-Flashlight mode
-Remember last time when battery changes
-Tune key combos
-Reset atomizer resistance (useful for TC)
-Some bugfixes

COONTROLS
I tried to make control better and comfortable
Now it looks like

2xFire - Temp/Power mode in TC and VVlite (vaaryvolt - yeah, we have it) in WV
3xFire  - Profiles
4xFire - Edit screen

-Fire - menu
-Fire - long press  - rotate screen
-Fire - looonger - swap +- buttons (in next releases)

+Fire - stealth
+- - key lock

+-Fire (three buttons) - call brightness control
+-Fire long - call flashlight

In menu:
-Fire - go to parent menu
+- - exit from menu


Some changings from original Sur_Myevic

-Battery protection (charging will be stoped at maxtemperature-21C)
-A LOT of interface changing, some of them still not stable, but we work on it
-Better TC screen (screen refreshes 10 times per second, not 2)
-Less time for some reaction. You know Drag with 0.025s fire delay? We set 0.010 and it works ^_^
-4 level for brightness
-Optimized control
-Fire screen counter changings (it keeps screen alive)

080318 1:56
Banary files provides in TG chat, not in github. Temporary
6 clicks for FUCKOFF screen. Useless, but it is the first wish for BetterJoy :)
Fonts are bigger and better
A lot of interface fixes
For example, no more screen frizes in TC (i guess)
Vaping in the first place on Menu

040318 23:16
New feature:
ChargeTemp Protection 
You can set in settings/expert/MAX/ChT max temp when mod is charging. 
If device become hotter that ChT, it just stops charge until device became colder. It protects you li-ion battery and your mod. 
Default settings: 50°C
Also, there is some changes in charging screen, but it's a little glitchy, I am little lazy. Not it shows every battery percent. I wanna make clock too.

220218 0:29
You don't need to press 3 keys to rotate screen/swap keys, just use 2 buttons -+ to make it
0.4 sec - key (un)lock
0.8 sec - flip screen
2 sec - swap +-

220218 21:01
Flashlight work again
No more .x watt in TC powerline on >10W
No more new coil confirmation, if your coil is really new, hold +Fire


220218 6:21
Continious brightness from flashlight fix

220218 1:19
Flashlight! Just hold +Fire which calls stealth. It's glitchy, but works

220218 1:19
USB voltage in TCR mode. Test

210218 20:26
6 digit fixed.
Honestly, there is a lot of fixes which was not logged, sorry :)

200218 18:18
Charging icon in Amp place
Weakest bat voltage in the midle bottom 
Bigger C and F icon for temp
-Fire (long) - update resistance
-+Fire (long) rotates your screen
-+Fire (longer) swap +- buttons


160218 21:12
Right resistance has 3 signs after point (base resistance)

160218 21:12
99 percent max for better viewing
2.7 volt min
Other fixes

180118 20:23
Pretty battery, board temp in the bottom.


180118 15:15
Finally, I won bug with attached USB screen had some trouble and make improved UI.

140118 1:09
Fix profiles sings in the topchik

140118 0:42
Faster keyreaction on +- buttons, but more 'stable', bigger F/C/W symbols in main line
Why reaction was sooooo sloooow?

130118 11:17

Contrast stealth still disabled, but second contrast does not wokr on firing, coz it makes glitches. I will fix it later.

120118 21:30
More images and Contrast stealth removing. But you still can set up 2Contrast on firing (current fixes can be not very good, but it works)


120118 15
Improved profile selecting, now you can reset puffs from screen edit mode+ Long fire. All counters (puffs, time) will be resetted, it not necessary to erase one of them.

120118 4:39
Okay, we can not 'load' current profile, but we can just exit from menu on one click

120118 4:17
Menus.c reverted to stable version of Sur MyEvic, coz some bugs appears
I will change it later. So, swapbuttons now in miscs, not interface again, you can not select current profile with single click
BUT you current profile will be saved if you switch another profile, you do not need to save it with long fire


120118 3:16
Fixed voltsline, doubleclick (T.Dom click in VW mode means VVlite switch)
Yesterday I lost my sources and some bugfixes :(

12.01.2018
Test readme :)

11.01.2018

Init