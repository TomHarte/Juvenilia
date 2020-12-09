   xangle = 0: zangle = 1: leave = 0: angle = 360: PI = 3.141592654#: zoom = 20
   radplus = (90 * PI) / 180: major = 1: minor = 1: halfx = 170: halfy = 100
   adder = 10.5: x = 63
 
   DO WHILE leave < 1
     GOSUB 10: REM pointworkout
     GOSUB 20: REM display
     GOSUB 30: REM keypresses
     GOSUB 40: REM update
   LOOP
   GOTO 50: REM end

10 REM pointworkout
 
   radangle = (angle * PI) / 180
   x1 = x + (zoom * major) * COS(radangle)
   PRINT x1
   y1 = y + (zoom * minor) * SIN(radangle)
 
   x2 = x + (zoom * major) * COS(radangle + radplus)
   PRINT x2
   y2 = y + (zoom * minor) * SIN(radangle + radplus)
 
   x3 = x + (zoom * major) * COS(radangle + (2 * radplus))
   PRINT x3
   y3 = y + (zoom * minor) * SIN(radangle + (2 * radplus))

   x4 = x + (zoom * major) * COS(radangle + (3 * radplus))
   y4 = y + (zoom * minor) * SIN(radangle + (3 * radplus))
 
   p1x = x + (5 * major) * COS(radangle)
   p1y = 10 + y + (5 * minor) * SIN(radangle)
   p2x = x + (4 * major) * COS(radangle)
   p2y = 3 + (4 * minor) * SIN(radangle)
   p3x = x + (6 * major) * COS(radangle)
   p3y = 3 + (4 * minor) * SIN(radangle)
  
   viewx1 = halfx - x1
   viewy1 = halfy - y1
   viewx2 = halfx - x2
   viewy2 = halfy - y2
   viewx3 = halfx - x3
   viewy3 = halfy - y3
   viewx4 = halfx - x4
   viewy4 = halfy - y4
   pos1x = halfx - p1x
   pos1y = halfy - p1y
   pos2x = halfx - p2x
   pos2y = halfy - p2y
   pos3x = halfx - p3x
   pos3y = halfy - p3y
  
   RETURN

20 REM display
   SCREEN 7, 1, actscr, visscr
  
   LINE (viewx1, viewy1)-(viewx2, viewy2)
   LINE (viewx2, viewy2)-(viewx3, viewy3)
   LINE (viewx3, viewy3)-(viewx4, viewy4)
   LINE (viewx4, viewy4)-(viewx1, viewy1)
  
   PAINT (halfx - x, halfy), 2, 15
  
   LINE (0, 0)-(319, 0)
   LINE (319, 0)-(319, 199)
   LINE (319, 199)-(0, 199)
   LINE (0, 199)-(0, 0)
  
   COLOR 1
   CIRCLE (pos1x, pos1y), major * 2
   PAINT (pos1x, pos1y), 1, 1
  
   CIRCLE (pos2x, pos2y), major * 1.5
   CIRCLE (pos3x, pos3y), major * 1.5
  
   COLOR 15
   LINE (212, 0)-(212, 199)
   LINE (212, 199)-(319, 0)
   LINE (212, 0)-(319, 199)
  
   SCREEN 7, 1, visscr, actscr
   CLS
   IF actscr = 0 THEN actscr = 1: visscr = 0: RETURN
   IF actscr = 1 THEN actscr = 0: visscr = 1: RETURN

30 REM keypresses
   a$ = INKEY$
   IF a$ = CHR$(27) THEN leave = 1
   RETURN

40 REM update
   IF major > 5 THEN GOTO 41
   major = major + .1: minor = (1 / 4) * major
41 IF adder >= .1 THEN angle = angle + adder
   adder = adder - .01
   RETURN

50 REM end
   SCREEN 7, 1, visscr, visscr

