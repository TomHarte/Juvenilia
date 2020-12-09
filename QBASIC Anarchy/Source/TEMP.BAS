   leave = 0: angle = 360: PI = 3.141592654#: zoom = 100: cury = 0: angle = 0
   radplus = (90 * PI) / 180: major = 4: minor = (1 / 6) * major
   adder = 0: acr = 0: filled = 1: players = 4
   mode = 9: halfx = 640 / 2: halfy = 350 / 2: scrx = 639: scry = 349
   bcol = 2: bkcol = 8: lcol = 14: curx1 = 10: cury1 = 10
   DIM x(4), y(4), viewx(4), viewy(4), cradangle(4), sradangle(4), gridx(4)
   DIM cirx(24), ciry(24), wizx(8), wizy(8), gridx1(16), gridy1(16), gridy(4)
   DIM gridx2(16), gridy2(16), gridx3(16), gridy3(16), gridx4(16), gridy4(16)

   FOR temp = 0 TO 8
     wizx(temp) = 6
     wizy(temp) = 6
   NEXT temp
  
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
  
   FOR temp = 0 TO 3
     sradangle(temp) = SIN(radangle + (temp * radplus))
     cradangle(temp) = COS(radangle + (temp * radplus))
   
     x(temp) = acr + major * cradangle(temp)
     y(temp) = dwn + minor * sradangle(temp)
  
     viewx(temp) = halfx - (zoom * x(temp))
     viewy(temp) = halfy - (zoom * y(temp))
  
     gridx(temp) = x + halfx - ((zoom / 16) * x(temp))
     gridy(temp) = y + halfy - ((zoom / 16) * y(temp))
   NEXT temp
  
   tx1 = viewx(0) - viewx(1)
   ty1 = viewy(0) - viewy(1)
   tx2 = viewx(1) - viewx(2)
   ty2 = viewy(1) - viewy(2)
   tx3 = viewx(2) - viewx(3)
   ty3 = viewy(2) - viewy(3)
   tx4 = viewx(3) - viewx(0)
   ty4 = viewy(3) - viewy(0)
 
   FOR temp = 1 TO (players * 4) - 1
     gridx1(temp) = viewx(0) - ((tx1 / 16) * temp)
     gridy1(temp) = viewy(0) - ((ty1 / 16) * temp)
  
     gridx2(temp) = viewx(1) - ((tx2 / 16) * temp)
     gridy2(temp) = viewy(1) - ((ty2 / 16) * temp)
    
     gridx3(temp) = viewx(2) - ((tx3 / 16) * temp)
     gridy3(temp) = viewy(2) - ((ty3 / 16) * temp)
  
     gridx4(temp) = viewx(3) - ((tx4 / 16) * temp)
     gridy4(temp) = viewy(3) - ((ty4 / 16) * temp)
   NEXT temp
  
   RETURN

20 REM display
   SCREEN mode, 1, actscr, visscr
  
   COLOR 15, bkcol
  
   FOR temp = 0 TO 2
     LINE (viewx(temp), viewy(temp))-(viewx(temp + 1), viewy(temp + 1))
   NEXT temp
   LINE (viewx(temp), viewy(temp))-(viewx(0), viewy(0))
  
   IF filled = 1 THEN PAINT (halfx - x, halfy), bcol, 15
  
   COLOR lcol
  
   FOR temp = 1 TO (4 * players) - 1
     LINE (gridx1(temp), gridy1(temp))-(gridx3(16 - temp), gridy3(16 - temp))
     LINE (gridx2(temp), gridy2(temp))-(gridx4(16 - temp), gridy4(16 - temp))
   NEXT temp
  
   GOSUB 140
  
   COLOR 9
   LINE (gridx(0), gridy(0))-(gridx(1), gridy(1))
   LINE (gridx(1), gridy(1))-(gridx(2), gridy(2))
   LINE (gridx(2), gridy(2))-(gridx(3), gridy(3))
   LINE (gridx(3), gridy(3))-(gridx(0), gridy(0))
  
   REM CIRCLE (curx(0), cury(0)), 15
   REM CIRCLE (curx(1), cury(1)), 15
  
   COLOR 15
  
   LINE (0, 0)-(scrx, 0)
   LINE (scrx, 0)-(scrx, scry)
   LINE (scrx, scry)-(0, scry)
   LINE (0, scry)-(0, 0)
  
   FOR cwiz = 0 TO players - 1
     GOSUB 50
     IF cwiz = turn THEN COLOR 5:  ELSE COLOR 15
     CIRCLE (cirx(0), ciry(0)), major * 2
     CIRCLE (cirx(1), ciry(1)), major * 1.5
     CIRCLE (cirx(2), ciry(2)), major * 1.5
   NEXT cwiz
  
   COLOR 15
  
   LINE (480, 0)-(480, 40)
   LINE (480, 40)-(scrx, 40)
  
   IF cmode = 0 THEN LOCATE (2), (64): PRINT "Rotate  /  Scale"
   IF cmode = 1 THEN LOCATE (2), (64): PRINT "Movement   Mode"
  
   SCREEN mode, 1, visscr, actscr
   CLS
   IF actscr = 0 THEN actscr = 1: visscr = 0: RETURN
   IF actscr = 1 THEN actscr = 0: visscr = 1: RETURN
   RETURN: REM no reason

30 REM keypresses
   a$ = INKEY$
   IF a$ = "1" THEN GOSUB 120: GOSUB 90: REM down/left
   IF a$ = "2" THEN GOSUB 120: REM down
   IF a$ = "3" THEN GOSUB 120: GOSUB 100: REM down/right
   IF a$ = "4" THEN GOSUB 90: REM left
   IF a$ = "5" THEN GOSUB 140: REM movement
   IF a$ = "6" THEN GOSUB 100: REM right
   IF a$ = "7" THEN GOSUB 110: GOSUB 90: REM up/left
   IF a$ = "8" THEN GOSUB 110: REM up
   IF a$ = "9" THEN GOSUB 110: GOSUB 100: REM up/right
   IF a$ = "0" THEN GOSUB 130: REM change turn
   IF a$ = CHR$(27) THEN leave = 1
   IF a$ = "." THEN IF cmode = 1 THEN cmode = 0:  ELSE cmode = 1
   IF a$ = "b" THEN LET bcol = bcol + 1: IF bcol = 16 THEN bcol = 1
   IF a$ = "k" THEN LET bkcol = bkcol + 1: IF bkcol = 16 THEN bkcol = 1
   IF a$ = "f" THEN filled = filled + 1: IF filled = 2 THEN filled = 0
   IF a$ = "l" THEN lcol = lcol + 1: IF lcol = 16 THEN lcol = 0
   RETURN

40 REM update
   RETURN

50 REM WIZARD SYNTAX
   cirx(0) = acr + ((zoom - wizx(cwiz)) * major) * cradangle(cwiz)
   ciry(0) = 10 + dwn + ((zoom - wizy(cwiz)) * minor) * sradangle(cwiz)
   cirx(1) = acr + ((zoom - wizx(cwiz) + 3) * major) * cradangle(cwiz)
   ciry(1) = 3 + dwn + ((zoom - wizy(cwiz) + 3) * minor) * sradangle(cwiz)
   cirx(2) = acr + ((zoom - wizx(cwiz) - 3) * major) * cradangle(cwiz)
   ciry(2) = 3 + dwn + ((zoom - wizy(cwiz) - 3) * minor) * sradangle(cwiz)
  
   FOR temp = 0 TO 2
     cirx(temp) = halfx - cirx(temp)
     ciry(temp) = halfy - ciry(temp)
   NEXT temp
   RETURN

90 REM left
   IF cmode = 0 THEN angle = angle - .5: RETURN
   IF cmode = 1 THEN curx1 = curx1 + 1
   RETURN

100 REM right
   IF cmode = 0 THEN angle = angle + .5: RETURN
   IF cmode = 1 THEN curx1 = curx1 - 1
   RETURN

110 REM up
   IF cmode = 0 AND major < 10 THEN major = major + .1: minor = (1 / 6) * major: RETURN
   IF cmode = 1 THEN cury1 = cury1 + 1
   RETURN

120 REM down
   IF cmode = 0 AND major > 1 THEN major = major - .1: minor = (1 / 6) * major: RETURN
   IF cmode = 1 THEN cury1 = cury1 - 1
   RETURN

130 REM change turn
   IF turn < players THEN turn = turn + 1
   IF turn = players THEN turn = 0
   RETURN

140 REM cursor
   tempx = viewx(0) - ((tx1 / 16) * curx1)
   tempy = viewy(0) - ((ty1 / 16) * cury1)
  
   REM gridx(0) = tempx + gridx(0)
   REM gridy(1) = tempy + gridy(0)
  
   RETURN

900 REM end
   SCREEN mode, 1, visscr, visscr

