   leave = 0: angle = 360: PI = 3.141592654#: zoom = 100
   radplus = (90 * PI) / 180: major = 4: minor = (1 / 6) * major
   adder = 0: x = 0: DIM cradangle(4), sradangle(4): filled = 1: players = 4
   mode = 9: halfx = 640 / 2: halfy = 350 / 2: scrx = 639: scry = 349: turn = 1
   bcol = 2: bkcol = 8: wiz1x = 10: wiz1y = 10
   curx = 0: cury = 0: angle = 45

   DO WHILE leave < 1
     GOSUB 10: REM pointworkout
     GOSUB 30: REM keypresses
     GOSUB 20: REM display
     GOSUB 30: REM keypresses
     GOSUB 40: REM update
     GOSUB 30: REM keypresses
   LOOP
   GOTO 900: REM end

10 REM pointworkout

   radangle = (angle * PI) / 180
   sradangle(0) = SIN(radangle)
   cradangle(0) = COS(radangle)
   sradangle(1) = SIN(radangle + radplus)
   cradangle(1) = COS(radangle + radplus)
   sradangle(2) = SIN(radangle + (2 * radplus))
   cradangle(2) = COS(radangle + (2 * radplus))
   sradangle(3) = SIN(radangle + (3 * radplus))
   cradangle(3) = COS(radangle + (3 * radplus))
  
   gx1 = x + major * cradangle(0)
   gy1 = y + minor * sradangle(0)
 
   gx2 = x + major * cradangle(1)
   gy2 = y + minor * sradangle(1)
 
   gx3 = x + major * cradangle(2)
   gy3 = y + minor * sradangle(2)

   gx4 = x + major * cradangle(3)
   gy4 = y + minor * sradangle(3)

   x1 = x + major * (cradangle(0) - cradangle(1))
   y1 = y + minor * (sradangle(0) - sradangle(1))

   x2 = x + major * (cradangle(1) - cradangle(2))
   y2 = y + minor * (sradangle(1) - sradangle(2))

   x3 = x + major * (cradangle(2) - cradangle(3))
   y3 = y + minor * (sradangle(2) - sradangle(3))

   x4 = x + major * (cradangle(3) - cradangle(0))
   y4 = y + minor * (sradangle(3) - sradangle(0))
  
   newx = x + (x1 - x3)
   newy = y + (y1 - y3)
   dif = newx / newy
  
   viewx1 = halfx - ((zoom - 50) * x1)
   viewy1 = halfy - ((zoom - 50) * y1)
   viewx2 = halfx - ((zoom - 50) * x2)
   viewy2 = halfy - ((zoom - 50) * y2)
   viewx3 = halfx - ((zoom - 50) * x3)
   viewy3 = halfy - ((zoom - 50) * y3)
   viewx4 = halfx - ((zoom - 50) * x4)
   viewy4 = halfy - ((zoom - 50) * y4)
  
   gviewx1 = halfx - (zoom * gx1)
   gviewy1 = halfy - (zoom * gy1)
   gviewx2 = halfx - (zoom * gx2)
   gviewy2 = halfy - (zoom * gy2)
   gviewx3 = halfx - (zoom * gx3)
   gviewy3 = halfy - (zoom * gy3)
   gviewx4 = halfx - (zoom * gx4)
   gviewy4 = halfy - (zoom * gy4)
   
   GOSUB 50: REM wizard 1
   GOSUB 60: REM wizard 2
   GOSUB 70: REM wizard 3
   GOSUB 80: REM wizard 4
  
   RETURN

20 REM display
   SCREEN mode, 1, actscr, visscr
  
   COLOR 15, bkcol
  
   LINE (gviewx1, gviewy1)-(gviewx2, gviewy2)
   LINE (gviewx2, gviewy2)-(gviewx3, gviewy3)
   LINE (gviewx3, gviewy3)-(gviewx4, gviewy4)
   LINE (gviewx4, gviewy4)-(gviewx1, gviewy1)
  
   IF filled = 1 THEN PAINT (halfx - x, halfy), bcol, 15
  
   GOTO 5
   FOR grid = 0 TO 100 STEP 10
     sviewx1 = halfx - ((grid + 0) * x1)
     sviewy1 = halfy - ((grid + 0) * y1)
     sviewx2 = halfx - ((grid + 0) * x2)
     sviewy2 = halfy - ((grid + 0) * y2)
     sviewx3 = halfx - ((grid + 0) * x3)
     sviewy3 = halfy - ((grid + 0) * y3)
     sviewx4 = halfx - ((grid + 0) * x4)
     sviewy4 = halfy - ((grid + 0) * y4)
 
     LINE (sviewx1, sviewy1)-(sviewx2, sviewy2)
     LINE (sviewx2, sviewy2)-(sviewx3, sviewy3)
     LINE (sviewx3, sviewy3)-(sviewx4, sviewy4)
     LINE (sviewx4, sviewy4)-(sviewx1, sviewy1)
   NEXT grid
  
5 LINE (gviewx1, gviewy1)-(gviewx2, gviewy2)
   LINE (gviewx2, gviewy2)-(gviewx3, gviewy3)
   LINE (gviewx3, gviewy3)-(gviewx4, gviewy4)
   LINE (gviewx4, gviewy4)-(gviewx1, gviewy1)
  
   LINE (viewx1, viewy1)-(viewx3, viewy3)
   LINE (viewx2, viewy2)-(viewx4, viewy4)
  
   LINE (cvx1, cvy1)-(cvx2, cvy2)
   LINE (cvx2, cvy2)-(cvx3, cvy3)
   LINE (cvx3, cvy3)-(cvx4, cvy4)
   LINE (cvx4, cvy4)-(cvx1, cvy1)
  
   LINE (0, 0)-(scrx, 0)
   LINE (scrx, 0)-(scrx, scry)
   LINE (scrx, scry)-(0, scry)
   LINE (0, scry)-(0, 0)
  
   IF turn = 1 THEN COLOR 12
   CIRCLE (c1x, c1y), major * 2
   CIRCLE (c2x, c2y), major * 1.5
   CIRCLE (c3x, c3y), major * 1.5
  
   IF turn = 2 THEN COLOR 12:  ELSE COLOR 15
   CIRCLE (c4x, c4y), major * 2
   CIRCLE (c5x, c5y), major * 1.5
   CIRCLE (c6x, c6y), major * 1.5
  
   IF turn = 3 THEN COLOR 12:  ELSE COLOR 15
   CIRCLE (c7x, c7y), major * 2
   CIRCLE (c8x, c8y), major * 1.5
   CIRCLE (c9x, c9y), major * 1.5
  
   IF turn = 4 THEN COLOR 12:  ELSE COLOR 15
   CIRCLE (c10x, c10y), major * 2
   CIRCLE (c11x, c11y), major * 1.5
   CIRCLE (c12x, c12y), major * 1.5
  
   COLOR 15
  
   LINE (480, 0)-(480, 40)
   LINE (480, 40)-(scrx, 40)
  
   IF cmode = 0 THEN LOCATE (2), (64): PRINT "Rotate  /  Scale"
   IF cmode = 1 THEN LOCATE (2), (64): PRINT "Movement   Mode"
  
   SCREEN mode, 1, visscr, actscr
   CLS
   IF actscr = 0 THEN actscr = 1: visscr = 0: RETURN
   IF actscr = 1 THEN actscr = 0: visscr = 1: RETURN

30 REM keypresses
   a$ = INKEY$
   IF a$ = CHR$(27) THEN leave = 1
   IF a$ = "s" THEN GOSUB 130: REM change turn
   IF a$ = "d" THEN GOSUB 100: REM right
   IF a$ = "a" THEN GOSUB 90: REM left
   IF a$ = "w" THEN GOSUB 110: REM up
   IF a$ = "x" THEN GOSUB 120: REM down
   IF a$ = "e" THEN GOSUB 110: GOSUB 100: REM up/right
   IF a$ = "q" THEN GOSUB 110: GOSUB 90: REM up/left
   IF a$ = "z" THEN GOSUB 120: GOSUB 90: REM down/left
   IF a$ = "c" THEN GOSUB 120: GOSUB 100: REM down/right
   IF a$ = "m" THEN IF cmode = 1 THEN cmode = 0:  ELSE cmode = 1
   IF a$ = "b" THEN LET bcol = bcol + 1: IF bcol = 15 THEN bcol = 1
   IF a$ = "k" THEN LET bkcol = bkcol + 1: IF bkcol = 15 THEN bkcol = 1
   IF a$ = " " THEN GOSUB 140: REM movement

   RETURN

40 REM update
   REM IF major > 3 THEN GOTO 41
   REM major = major + .01: minor = (1 / 7) * major
41 IF adder >= .1 THEN angle = angle + adder
   adder = adder - .005
   wiz1x = wiz1x + 1: wiz2x = wiz2x + 1
   wiz1y = wiz1y + 1: wiz2y = wiz2y + 1
   RETURN

50 REM WIZARD 1 SYNTAX
   wiz1bx = x + ((zoom - wiz1x) * major) * (cradangle(1) - cradangle(0))
   wiz1by = 10 + y + ((zoom - wiz1y) * minor) * (sradangle(1) - sradangle(0))
   wiz1l1x = x + ((zoom - wiz1x + 3) * major) * (cradangle(1) - cradangle(0))
   wiz1l1y = 3 + y + ((zoom - wiz1y + 3) * minor) * (sradangle(1) - sradangle(0))
   wiz1l2x = x + ((zoom - wiz1x - 3) * major) * (cradangle(1) - cradangle(0))
   wiz1l2y = 3 + y + ((zoom - wiz1y - 3) * minor) * (sradangle(1) - sradangle(0))
  
   c1x = halfx - wiz1bx
   c1y = halfy - wiz1by
   c2x = halfx - wiz1l1x
   c2y = halfy - wiz1l1y
   c3x = halfx - wiz1l2x
   c3y = halfy - wiz1l2y
   RETURN

60 REM WIZARD 2 SYNTAX
   wiz2bx = x + ((zoom - wiz2x) * major) * cradangle(1)
   wiz2by = 10 + y + ((zoom - wiz2y) * minor) * sradangle(1)
   wiz2l1x = x + ((zoom - wiz2x + 3) * major) * cradangle(1)
   wiz2l1y = 3 + y + ((zoom - wiz2y + 3) * minor) * sradangle(1)
   wiz2l2x = x + ((zoom - wiz2x - 3) * major) * cradangle(1)
   wiz2l2y = 3 + y + ((zoom - wiz2y - 3) * minor) * sradangle(1)
 
   c4x = halfx - wiz2bx
   c4y = halfy - wiz2by
   c5x = halfx - wiz2l1x
   c5y = halfy - wiz2l1y
   c6x = halfx - wiz2l2x
   c6y = halfy - wiz2l2y
   RETURN

70 REM WIZARD 3 SYNTAX
   wiz3bx = x + (zoom * major) * cradangle(2)
   wiz3by = 10 + y + (zoom * minor) * sradangle(2)
   wiz3l1x = x + ((zoom + 3) * major) * cradangle(2)
   wiz3l1y = 3 + y + ((zoom + 3) * minor) * sradangle(2)
   wiz3l2x = x + ((zoom - 3) * major) * cradangle(2)
   wiz3l2y = 3 + y + ((zoom - 3) * minor) * sradangle(2)

   c7x = halfx - wiz3bx
   c7y = halfy - wiz3by
   c8x = halfx - wiz3l1x
   c8y = halfy - wiz3l1y
   c9x = halfx - wiz3l2x
   c9y = halfy - wiz3l2y
   RETURN

80 REM WIZARD 4 SYNTAX
   wiz4bx = x + (zoom * major) * cradangle(3)
   wiz4by = 10 + y + (zoom * minor) * sradangle(3)
   wiz4l1x = x + ((zoom + 3) * major) * cradangle(3)
   wiz4l1y = 3 + y + ((zoom + 3) * minor) * sradangle(3)
   wiz4l2x = x + ((zoom - 3) * major) * cradangle(3)
   wiz4l2y = 3 + y + ((zoom - 3) * minor) * sradangle(3)

   c10x = halfx - wiz4bx
   c10y = halfy - wiz4by
   c11x = halfx - wiz4l1x
   c11y = halfy - wiz4l1y
   c12x = halfx - wiz4l2x
   c12y = halfy - wiz4l2y
   RETURN

90 REM left
   IF cmode = 0 THEN angle = angle - .9: RETURN
   IF cmode = 1 THEN curx = curx + 1: cury = cury + 1
   RETURN

100 REM right
   IF cmode = 0 THEN angle = angle + .9: RETURN
   IF cmode = 1 THEN curx = curx - 1: cury = cury - 1
   RETURN

110 REM up
   IF cmode = 0 AND major < 10 THEN major = major + .1: minor = (1 / 6) * major: RETURN
   IF cmode = 1 THEN cury = cury + 30
   RETURN

120 REM down
   IF cmode = 0 AND major > 1 THEN major = major - .1: minor = (1 / 6) * major: RETURN
   IF cmode = 1 THEN cury = cury - 30
   RETURN

130 REM change turn
   IF turn < players + 1 THEN turn = turn + 1
   IF turn = players + 1 THEN turn = 1
   RETURN

140 REM cursor
   cx1 = x + (zoom / 10 * major) * cradangle(0)
   cy1 = y + (zoom / 10 * minor) * sradangle(0)

   cx2 = x + (zoom / 10 * major) * cradangle(1)
   cy2 = y + (zoom / 10 * minor) * sradangle(1)

   cx3 = x + (zoom / 10 * major) * cradangle(2)
   cy3 = y + (zoom / 10 * minor) * sradangle(2)

   cx4 = x + (zoom / 10 * major) * cradangle(3)
   cy4 = y + (zoom / 10 * minor) * sradangle(3)
  
   ncvx1 = halfx - cx1
   ncvy1 = halfy - cy1
   ncvx2 = halfx - cx2
   ncvy2 = halfy - cy2
   ncvx3 = halfx - cx3
   ncvy3 = halfy - cy3
   ncvx4 = halfx - cx4
   ncvy4 = halfy - cy4
  
   poscx1 = (((zoom - curx) * major) * cradangle(0))
   poscy1 = (((zoom - cury) * minor) * sradangle(0))
  
   cvx1 = halfx - poscx1
   cvy1 = halfy - poscx1
   cvx2 = (cvx1 - (ncvx2 - ncvx1))
   cvy2 = (cvy1 - (ncvy2 - ncvy1))
   cvx3 = (cvx1 - (ncvx3 - ncvx1))
   cvy3 = (cvy1 - (ncvy3 - ncvy1))
   cvx4 = (cvx1 - (ncvx4 - ncvx1))
   cvy4 = (cvy1 - (ncvy4 - ncvy1))

   RETURN

900 REM end
   SCREEN mode, 1, visscr, visscr

