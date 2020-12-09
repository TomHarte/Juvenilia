   xangle = 0: zangle = 1: leave = 0: angle = 360: PI = 3.141592654#: zoom = 100
   radplus = (90 * PI) / 180: major = 4: minor = (1 / 7) * major
   adder = .2: x = 0: DIM cradangle(4), sradangle(4): filled = 1
   mode = 9: halfx = 320: halfy = 175: scrx = 639: scry = 349: turn = 1
   DIM cursx(3), cursy(3), cuscrx(3), cuscry(3): curx = halfx: cury = halfy

   DO WHILE leave < 1
     GOSUB 30: REM keypresses
     GOSUB 10: REM pointworkout
     GOSUB 30: REM keypresses
     GOSUB 20: REM display
     GOSUB 30: REM keypresses
     GOSUB 40: REM update
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
  
   x1 = x + (zoom * major) * cradangle(0)
   y1 = y + (zoom * minor) * sradangle(0)
 
   x2 = x + (zoom * major) * cradangle(1)
   y2 = y + (zoom * minor) * sradangle(1)
 
   x3 = x + (zoom * major) * cradangle(2)
   y3 = y + (zoom * minor) * sradangle(2)

   x4 = x + (zoom * major) * cradangle(3)
   y4 = y + (zoom * minor) * sradangle(3)
 
   REM GOSUB 130: REM cursor
  
   viewx1 = halfx - x1
   viewy1 = halfy - y1
   viewx2 = halfx - x2
   viewy2 = halfy - y2
   viewx3 = halfx - x3
   viewy3 = halfy - y3
   viewx4 = halfx - x4
   viewy4 = halfy - y4
  
   cviewx1 = halfx - cx1
   cviewy1 = halfy - cy1
   cviewx2 = halfx - cx2
   cviewy2 = halfy - cy2
   cviewx3 = halfx - cx3
   cviewy3 = halfy - cy3
   cviewx4 = halfx - cx4
   cviewy4 = halfy - cy4
  
   GOSUB 50
   GOSUB 60
   GOSUB 70
   GOSUB 80
  
   RETURN

20 REM display
   SCREEN mode, 1, actscr, visscr
  
   COLOR 15, 1
  
   LINE (viewx1, viewy1)-(viewx2, viewy2)
   LINE (viewx2, viewy2)-(viewx3, viewy3)
   LINE (viewx3, viewy3)-(viewx4, viewy4)
   LINE (viewx4, viewy4)-(viewx1, viewy1)
  
   IF filled = 1 THEN PAINT (halfx - x, halfy), 8, 15
  
   REM LINE (cviewx1, cviewy1)-(cviewx2, cviewy2)
   REM LINE (cviewx2, cviewy2)-(cviewx3, cviewy3)
   REM LINE (cviewx3, cviewy3)-(cviewx4, cviewy4)
   REM LINE (cviewx4, cviewy4)-(cviewx1, cviewy1)
  
   REM LINE (cuscr(0), cuscr(0))-(cuscr(1), curscr(1))
   REM LINE (cuscr(1), cuscr(1))-(cuscr(2), curscr(2))
   REM LINE (cuscr(2), cuscr(2))-(cuscr(3), curscr(3))
   REM LINE (cuscr(3), cuscr(3))-(cuscr(1), curscr(1))
  
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
  
   REM LINE (212, 0)-(212, 199)
   REM LINE (212, 199)-(319, 0)
   REM LINE (212, 0)-(319, 199)
  
   LINE (scrx - curx, 0)-(scrx - curx, scry)
   LINE (0, scry - cury)-(scrx, scry - cury)
 
   LINE (480, 0)-(480, 40)
   LINE (480, 40)-(scrx, 40)
  
   IF cmode = 0 THEN LOCATE (2), (64): PRINT "Rotate  /  Scale"
   IF cmode = 1 THEN LOCATE (2), (64): PRINT "Character select"
  
   SCREEN mode, 1, visscr, actscr
   CLS
   IF actscr = 0 THEN actscr = 1: visscr = 0: RETURN
   IF actscr = 1 THEN actscr = 0: visscr = 1: RETURN

30 REM keypresses
   a$ = INKEY$
   IF a$ = CHR$(27) THEN leave = 1
   IF a$ = "p" THEN GOSUB 100: REM right
   IF a$ = "o" THEN GOSUB 90: REM left
   IF a$ = "q" THEN GOSUB 110: REM up
   IF a$ = "a" THEN GOSUB 120: REM down
   IF a$ = " " THEN IF cmode = 1 THEN cmode = 0:  ELSE cmode = 1
   RETURN

40 REM update
   REM IF major > 3 THEN GOTO 41
   REM major = major + .01: minor = (1 / 7) * major
41 IF adder >= .1 THEN angle = angle + adder
   adder = adder - .005
   RETURN

50 REM WIZARD 1 SYNTAX
   wiz1bx = x + (zoom * major) * cradangle(0)
   wiz1by = 10 + y + (zoom * minor) * sradangle(0)
   wiz1l1x = x + ((zoom - 1) * major) * cradangle(0)
   wiz1l1y = 3 + ((zoom - 1) * minor) * sradangle(0)
   wiz1l2x = x + ((zoom - 1) * major) * cradangle(0)
   wiz1l2y = 3 + ((zoom + 1) * minor) * sradangle(0)
  
   c1x = halfx - wiz1bx
   c1y = halfy - wiz1by
   c2x = halfx - wiz1l1x
   c2y = halfy - wiz1l1y
   c3x = halfx - wiz1l2x
   c3y = halfy - wiz1l2y
   RETURN

60 REM WIZARD 2 SYNTAX
   wiz2bx = x + (zoom * major) * cradangle(1)
   wiz2by = 10 + y + (zoom * minor) * sradangle(1)
   wiz2l1x = x + ((zoom - 1) * major) * cradangle(1)
   wiz2l1y = 3 + ((zoom - 1) * minor) * sradangle(1)
   wiz2l2x = x + ((zoom - 1) * major) * cradangle(1)
   wiz2l2y = 3 + ((zoom + 1) * minor) * sradangle(1)
 
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
   wiz3l1x = x + ((zoom - 1) * major) * cradangle(2)
   wiz3l1y = 3 + ((zoom - 1) * minor) * sradangle(2)
   wiz3l2x = x + ((zoom - 1) * major) * cradangle(2)
   wiz3l2y = 3 + ((zoom + 1) * minor) * sradangle(2)

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
   wiz4l1x = x + ((zoom - 1) * major) * cradangle(3)
   wiz4l1y = 3 + ((zoom - 1) * minor) * sradangle(3)
   wiz4l2x = x + ((zoom - 1) * major) * cradangle(3)
   wiz4l2y = 3 + ((zoom + 1) * minor) * sradangle(3)

   c10x = halfx - wiz4bx
   c10y = halfy - wiz4by
   c11x = halfx - wiz4l1x
   c11y = halfy - wiz4l1y
   c12x = halfx - wiz4l2x
   c12y = halfy - wiz4l2y
   RETURN

90 REM left
   IF cmode = 0 THEN angle = angle - 2
   IF cmode = 1 THEN curx = curx + 5
   RETURN

100 REM left
   IF cmode = 0 THEN angle = angle + 2
   IF cmode = 1 THEN curx = curx - 5
   RETURN

110 REM up
   IF cmode = 0 AND major < 6 THEN major = major + .1: minor = (1 / 7) * major
   IF cmode = 1 THEN cury = cury + 5
   RETURN

120 REM down
   IF cmode = 0 AND major > 1 THEN major = major - .1: minor = (1 / 7) * major
   IF cmode = 1 THEN cury = cury - 5
   RETURN

130 REM cursor
   cx1 = curx + ((zoom / 20) * major) * cradangle(0)
   cy1 = cury + ((zoom / 20) * minor) * sradangle(0)

   cx2 = curx + ((zoom / 20) * major) * cradangle(1)
   cy2 = cury + ((zoom / 20) * minor) * sradangle(1)

   cx3 = curx + ((zoom / 20) * major) * cradangle(2)
   cy3 = cury + ((zoom / 20) * minor) * sradangle(2)

   cx4 = curx + ((zoom / 20) * major) * cradangle(3)
   cy4 = cury + ((zoom / 20) * minor) * sradangle(3)
   RETURN

900 REM end
   SCREEN mode, 1, visscr, visscr

