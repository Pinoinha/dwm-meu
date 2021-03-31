# dwm - dynamic window manager

This is my dwm build, with several patches I've applied for improving my workflow.   

# Patches  

* [activetagindicatorbar](https://dwm.suckless.org/patches/activetagindicatorbar/)  
* [actualfullscreen](https://dwm.suckless.org/patches/actualfullscreen/)  
* [bar heigth](https://dwm.suckless.org/patches/bar_height/)  
* [fullgaps](https://dwm.suckless.org/patches/fullgaps/) (previously)  
* [vanity gaps](https://dwm.suckless.org/patches/vanitygaps/) (actual)  
* [keychain](https://dwm.suckless.org/patches/keychain/)  
* [movestack](https://dwm.suckless.org/patches/movestack/)  
* [pertag](https://dwm.suckless.org/patches/pertag/)  
* [scratchpad](https://github.com/GasparVardanyan/dwm-scratchpad) (not the official one from suckless' website)  
* [sticky](https://dwm.suckless.org/patches/sticky/)  
* [swallow](https://dwm.suckless.org/patches/swallow/)  
* [systray](https://dwm.suckless.org/patches/systray/) (now obsolete)  
* [shiftview](https://lists.suckless.org/dev/1104/7590.html)
  
# Dependencies  
  
* `libx11`  
* `libxinerama`  
* `libxft`  
* `st` (you can use [my fork](https://github.com/Pinoinha/st-meu))  
* `dmenu` (you can also use [my fork](https://github.com/Pinoinha/dmenu-meu))  
  
# Building Instructions  
  
Clone the repository in your local machine and compile it using `make`  
```  
git clone https://github.com/Pinoinha/dwm-meu.git  
cd dwm-meu  
sudo make install  
```  

# Tips

* Learn some C, it will make wonders for your coding skills
* When applying patches, use `patch -p1 < patch-name.diff`: this will generate `.rej` files for you to analyze and make your life easier
* Never forget to add and commit your changes (push if possible) before applying any patches
* Read the documentation at `man dwm` and [Suckless' website](https://suckless.org/); also, the code on its own is also pretty good documentation if you know what's going on
* Have fun :)

# References

* [dwm's official site](https://dwm.suckless.org/)
* [Luke Smith's own build](https://github.com/LukeSmithxyz/dwm)
* Also check out my [other repos](https://github.com/Pinoinha)
