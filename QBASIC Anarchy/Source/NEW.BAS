   xangle = 0: zangle = 1: leave = 0: angle = 360: PI = 3.141592654#: zoom = 100
   radplus = (90 * PI) / 180: major = .1: minor = .1: halfx = 170: halfy = 100
   adder = 1.5: x = 0
   DIM x(10), y(10), viewx(10), viewy(10)

   SCREEN 7
   INPUT "How many sides? "; sides
   adder = 360 / sides
   adder = (adder * PI) / 180
  
   DO WHILE leave < 1
     GOSUB 10: REM pointworkout
     GOSUB 20: REM display
     GOSUB 30: REM keypresses
     GOSUB 40: REM update
   LOOP
   GOTO 60: REM end

10 REM pointworkout
 
   radangle = (angle * PI) / 180
  
   FOR sdes = 0 TO sides
     x(sdes) = x + (zoom * major) * COS(radangle + (sdes * adder))
     y(sdes) = y + (zoom * minor) * SIN(radangle + (sdes * adder))
  
     viewx(sdes) = halfx - x(sdes)
     viewy(sdes) = halfy - y(sdes)
   NEXT sdes
  
   RETURN

20 REM display
   SCREEN 7, 1, actscr, visscr
  
   FOR sdes = 0 TO sides - 1
     LINE (viewx(sdes), viewy(sdes))-(viewx(sdes + 1), viewy(sdes + 1))
   NEXT sdes
   LINE (viewx(sdes), viewy(sdes))-(viewx(0), viewy(0))
  
   PAINT (halfx - x, halfy), 2, 15
  
   LINE (0, 0)-(319, 0)
   LINE (319, 0)-(319, 199)
   LINE (319, 199)-(0, 199)
   LINE (0, 199)-(0, 0)
  
   CIRCLE (c1x, c1y), major * 2
   CIRCLE (c2x, c2y), major * 1.5
   CIRCLE (c3x, c3y), major * 1.5
  
   REM LINE (212, 0)-(212, 199)
   REM LINE (212, 199)-(319, 0)
   REM LINE (212, 0)-(319, 199)
  
   SCREEN 7, 1, visscr, actscr
   CLS
   IF actscr = 0 THEN actscr = 1: visscr = 0: RETURN
   IF actscr = 1 THEN actscr = 0: visscr = 1: RETURN

30 REM keypresses
   a$ = INKEY$
   IF a$ = CHR$(27) THEN leave = 1
   IF a$ = "+" THEN major = major + .01
   IF a$ = "-" THEN major = major - .01
   RETURN

40 REM update
   IF major > 5 THEN GOTO 41
   REM major = major + .01
   minor = (1 / 7) * major
41 IF adder >= .1 THEN angle = angle + adder
   adder = adder - .001
   RETURN

50 REM WIZARD 1 SYNTAX
   wiz1bx = x + (zoom * major) * cradangle
   wiz1by = 10 + y + (zoom * minor) * sradangle
   wiz1l1x = x + ((zoom - 1) * major) * cradangle
   wiz1l1y = 3 + ((zoom - 1) * minor) * sradangle
   wiz1l2x = x + ((zoom - 1) * major) * cradangle
   wiz1l2y = 3 + ((zoom + 1) * minor) * sradangle
  
   c1x = halfx - wiz1bx
   c1y = halfy - wiz1by
   c2x = halfx - wiz1l1x
   c2y = halfy - wiz1l1y
   c3x = halfx - wiz1l2x
   c3y = halfy - wiz1l2y
   RETURN

60 REM end
   SCREEN 7, 1, visscr, visscr

