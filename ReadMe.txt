   //Takumi Orion Clone Loading Screen Utility sfmlLoadUtil.h
   // Copyright (C) 2014  George Takumi Crary

   // This program is free software: you can redistribute it and/or modify
   // it under the terms of the GNU General Public License as published by
   // the Free Software Foundation, either version 3 of the License, or
   // any later version.

   // This program is distributed in the hope that it will be useful,
   // but WITHOUT ANY WARRANTY; without even the implied warranty of
   // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   // GNU General Public License for more details.

   // You should have received a copy of the GNU General Public License
   // along with this program.  If not, see <http://www.gnu.org/licenses/>.

sfmlLoadUtil.h is a header with a loadAssets function. This function acts similar to cstring.h strtok().
You pass it a list of loadTargets (whatever you want to load in sfml) and it'll load one at a time.
Everytime you pass it NULL_LOAD_ARG it'll load the next loadTarget in the list.
It should handle errors and what not. At least make it easier to debug.
Yes it is a pile of C++ template shit.

The ultimate goal of this utility is to make loading screens easier. You can call the function every other frame
while updating the loading bar or something for your fancy little SFML game.

OrionClone.cpp is the entry point for the utility demo.
Yes this is a header only library for now. Yes I understand it will be compiled for every include.
We'll cross that bridge when we get there. For now this utility must be completed.

TODO: Move dependancies into its own folder or use a git submodule.
