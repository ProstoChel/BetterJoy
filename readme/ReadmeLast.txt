Finally, we have readme and version history. Yeah, you can use github to get changelog, but it's not popular. This is online document
You always can discuss BetterJoy in TG https://t.me/betterjoy and get some answers

If you like BetterJoy, you can support project with this https://www.paypal.me/betterjoy ;)
It's really hard to develop, you can check it. Also you can help with your code, it will be great. Just follow github.com/ProstoChel/BetterJoy

We work on:
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
-4 level for brightness - 0-33-66-100-0...
-Optimized control
-Fire screen counter changings (it keeps screen alive)

140128 1:09
Fix profiles sings in the topchik

140128 0:42
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