# Descartes [1997/8]

An attempt to clone the Descent engine, based on an article I'd read.

Source is lost, as are versions adapted to the 3dfx Glide API and for Windows — this was my very first Windows program, using the free LCC-Win32 compiler and dynamically grabbing Glide symbols from its DLL. 

Specifically it:
* describes a level as a series of connected deformed cubes; and
* renders recursively from front to back, clipping geometry against the convex window defined by the aggregate of the connections, until all pixels are filled.

i.e. it's a vanilla portal renderer.    

There is at least one flaw in the renderer: for working out the aggregate portals I would clip the new portal polygon window against the existing portal planes and then build new planes from its outline.

This means that when portals intersect the camera, the content behind them is often improperly clipped. I hid this problem by just not clearing the graphics buffer.  

By now I have definitely had access to some web sites with discussion of 3d mathematics and polygon rasterisation, but topics like clipping remained homespun solutions.

![Media/Descartes1.png]
![Media/Descartes2.png]
![Media/Descartes3.png]

## Original Readme

```
"Descartes : Deceased" by SoftySoft 1997/8

	The controls are :

		Action			Button	

		forwards		up arrow
		rotate left		left arrow
		rotate right		right arrow
		look up			a
		look down		z
		float up		keypad minus
		float down		keypad plus

	so just walk around for a bit, then get very bored and draw over my    
	textures (they are all pcx). Then try to reverse engineer the level.map
	file (it is ASCII). Then shout at me via T.Harte@btinternet.com
```
