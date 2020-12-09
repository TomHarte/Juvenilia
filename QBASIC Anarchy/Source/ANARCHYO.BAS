   leave = 0: PI = 3.141592654#: zoom = 100: moves = 3
   radplus = (90 * PI) / 180: major = 3.200001: minor = (1 / 6) * major
   adder = 0: acr = 0: filled = 1: mode = 9: scrx = 639: scry = 349
   halfx = 640 / 2: halfy = 350 / 2: bcol = 2: bkcol = 8: lcol = 14
   DIM x(4), y(4), viewx(4), viewy(4), linx(24), liny(24)
   DIM cirx(24), ciry(24), wizx(8), wizy(8), gridx1(31), gridy1(31)
   DIM gridx2(31), gridy2(31), gridx3(31), gridy3(31), gridx4(31), gridy4(31)
   DIM psadangle(360), pcadangle(360), curxs(4), curys(4)
  
   FOR temp = 0 TO 360
     radangle = (temp * PI) / 180
     psadangle(temp) = SIN(radangle)
     pcadangle(temp) = COS(radangle)
   NEXT temp
  
   INPUT "HOW MANY PLAYERS"; players
   IF players > 8 THEN PRINT "TOO MANY, but Jay wins": GOTO 910
  
   edge = (10 * ((players * 4) - 1)) - 5
   hedge = INT(edge / 2) - 2
   wizx(0) = 5: wizy(0) = 5
   wizx(1) = 5: wizy(1) = edge
   wizx(2) = edge: wizy(2) = edge
   wizx(3) = edge: wizy(3) = 5
   wizx(4) = hedge: wizy(4) = 5
   wizx(5) = 5: wizy(5) = hedge
   wizx(6) = edge: wizy(6) = hedge
   wizx(7) = hedge: wizy(7) = edge
  
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

   IF angle < 0 THEN angle = 360 + angle
   IF angle > 360 THEN angle = angle - 360
  
   FOR temp = 0 TO 3
     ang = angle + (temp * 90)
     IF ang < 0 THEN ang = 360 + ang
     IF ang > 360 THEN ang = ang - 360
    
     x(temp) = acr + major * pcadangle(ang)
     y(temp) = dwn + minor * psadangle(ang)
  
     viewx(temp) = halfx - (zoom * x(temp))
     viewy(temp) = halfy - (zoom * y(temp))
  
     REM gridx(temp) = x + halfx - ((zoom / 16) * x(temp))
     REM gridy(temp) = y + halfy - ((zoom / 16) * y(temp))
   NEXT temp
  
   divide = ((players * 4) - 1) * 10
   tx1 = (viewx(0) - viewx(1)) / divide
   ty1 = (viewy(0) - viewy(1)) / divide
   tx2 = (viewx(1) - viewx(2)) / divide
   ty2 = (viewy(1) - viewy(2)) / divide
   tx3 = (viewx(2) - viewx(3)) / divide
   ty3 = (viewy(2) - viewy(3)) / divide
   tx4 = (viewx(3) - viewx(0)) / divide
   ty4 = (viewy(3) - viewy(0)) / divide
 
   FOR temp = 0 TO (players * 4) - 1
     gridx1(temp) = viewx(0) - (tx1 * 10 * temp)
     gridy1(temp) = viewy(0) - (ty1 * 10 * temp)
  
     gridx2(temp) = viewx(1) - (tx2 * 10 * temp)
     gridy2(temp) = viewy(1) - (ty2 * 10 * temp)
    
     gridx3(temp) = viewx(2) - (tx3 * 10 * temp)
     gridy3(temp) = viewy(2) - (ty3 * 10 * temp)
  
     gridx4(temp) = viewx(3) - (tx4 * 10 * temp)
     gridy4(temp) = viewy(3) - (ty4 * 10 * temp)
   NEXT temp
  
   GOSUB 140: REM cursor
  
   RETURN

20 REM display
   SCREEN mode, 1, actscr, visscr
  
   PRINT moves
   PRINT done
   PRINT curx
   PRINT wizx(turn)
  
   COLOR 15, bkcol
  
   FOR temp = 0 TO 2
     LINE (viewx(temp), viewy(temp))-(viewx(temp + 1), viewy(temp + 1))
   NEXT temp
   LINE (viewx(temp), viewy(temp))-(viewx(0), viewy(0))
  
   IF filled = 1 THEN PAINT (halfx - x, halfy), bcol, 15
  
   COLOR lcol
  
   FOR temp = 0 TO (4 * players) - 1
     tempy = ((4 * players) - 1) - temp
     LINE (gridx1(temp), gridy1(temp))-(gridx3(tempy), gridy3(tempy))
     LINE (gridx2(temp), gridy2(temp))-(gridx4(tempy), gridy4(tempy))
   NEXT temp
  
   IF cmode = 2 GOTO 12
   IF filled = 1 THEN GOTO 11
  
   COLOR 9
  
   LINE (curxs(0), curys(0))-(curxs(1), curys(1))
   LINE (curxs(1), curys(1))-(curxs(2), curys(2))
   LINE (curxs(2), curys(2))-(curxs(3), curys(3))
   LINE (curxs(3), curys(3))-(curxs(0), curys(0))

11 IF filled = 1 THEN PAINT (curxs(4), curys(4)), 0, 14
  
   COLOR 4
   LINE (gridx(0), gridy(0))-(gridx(1), gridy(1))
   LINE (gridx(1), gridy(1))-(gridx(2), gridy(2))
   LINE (gridx(2), gridy(2))-(gridx(3), gridy(3))
   LINE (gridx(3), gridy(3))-(gridx(0), gridy(0))
  
12 COLOR 15
  
   LINE (0, 0)-(scrx, 0)
   LINE (scrx, 0)-(scrx, scry)
   LINE (scrx, scry)-(0, scry)
   LINE (0, scry)-(0, 0)
  
   FOR cwiz = 0 TO players - 1
     GOSUB 50
     IF cwiz = turn THEN COLOR 5:  ELSE COLOR 15
     CIRCLE (cirx(0), ciry(0)), zoom * .08
     CIRCLE (cirx(1), ciry(1)), zoom * .03
     CIRCLE (cirx(2), ciry(2)), zoom * .03
     CIRCLE (cirx(3), ciry(3)), zoom * .03
     CIRCLE (cirx(4), ciry(4)), zoom * .03
     CIRCLE (cirx(5), ciry(5)), zoom * .05
    
     REM PAINT (cirx(3), ciry(3)), 5, lcol
    
     LINE (cirx(0), ciry(0))-(cirx(1), ciry(1))
     LINE (cirx(0), ciry(0))-(cirx(2), ciry(2))
     LINE (cirx(0), ciry(0))-(cirx(3), ciry(3))
     LINE (cirx(0), ciry(0))-(cirx(4), ciry(4))
    
     LINE (linx(0), liny(0))-(linx(1), liny(1))
     LINE (linx(1), liny(1))-(linx(2), liny(2))
     LINE (linx(2), liny(2))-(linx(0), liny(0))
   NEXT cwiz
  
   COLOR 15
  
   LINE (480, 0)-(480, 40)
   LINE (480, 40)-(scrx, 40)
  
   IF cmode = 0 THEN LOCATE (2), (64): PRINT "Rotate  /  Scale"
   IF cmode = 1 THEN LOCATE (2), (64): PRINT "Cursor  Movement"
   IF cmode = 2 THEN LOCATE (2), (64): PRINT "Wizard  Movement"
  
   SCREEN mode, 1, visscr, actscr
   CLS
   IF actscr = 0 THEN actscr = 1: visscr = 0: RETURN
   IF actscr = 1 THEN actscr = 0: visscr = 1: RETURN

30 REM keypresses
   a$ = INKEY$
   IF a$ = CHR$(27) THEN leave = 1
   IF a$ = "0" THEN GOSUB 130: REM change turn
   IF a$ = "1" THEN GOSUB 120: GOSUB 90: REM down/left
   IF a$ = "2" THEN GOSUB 120: REM down
   IF a$ = "3" THEN GOSUB 120: GOSUB 100: REM down/right
   IF a$ = "4" THEN GOSUB 90: REM left
   IF a$ = "5" THEN GOSUB 160: REM movement
   IF a$ = "6" THEN GOSUB 100: REM right
   IF a$ = "7" THEN GOSUB 110: GOSUB 90: REM up/left
   IF a$ = "8" THEN GOSUB 110: REM up
   IF a$ = "9" THEN GOSUB 110: GOSUB 100: REM up/right
   IF a$ = "." THEN IF cmode = 1 THEN cmode = 0:  ELSE cmode = cmode + 1
   IF a$ = "b" THEN LET bcol = bcol + 1: IF bcol = 16 THEN bcol = 1
   IF a$ = "k" THEN LET bkcol = bkcol + 1: IF bkcol = 16 THEN bkcol = 1
   IF a$ = "f" THEN filled = filled + 1: IF filled = 2 THEN filled = 0
   RETURN

40 REM update
   IF angle = 360 THEN angle = 0
   RETURN

50 REM WIZARD SYNTAX
   linx(0) = acr + ((viewx(0) - ((wizx(cwiz) + 0) * tx1)) - ((wizy(cwiz) + 5) * tx2))
   liny(0) = (dwn + ((viewy(0) - ((wizx(cwiz) + 0) * ty1)) - ((wizy(cwiz) + 5) * ty2))) - (.5 * zoom)
   linx(1) = acr + ((viewx(0) - ((wizx(cwiz) + 0) * tx1)) - ((wizy(cwiz) + 6) * tx2))
   liny(1) = (dwn + ((viewy(0) - ((wizx(cwiz) + 0) * ty1)) - ((wizy(cwiz) + 6) * ty2))) - (.3 * zoom)
   linx(2) = acr + ((viewx(0) - ((wizx(cwiz) + 0) * tx1)) - ((wizy(cwiz) + 4) * tx2))
   liny(2) = (dwn + ((viewy(0) - ((wizx(cwiz) + 0) * ty1)) - ((wizy(cwiz) + 4) * ty2))) - (.3 * zoom)
  
   cirx(0) = acr + ((viewx(0) - ((wizx(cwiz) + 0) * tx1)) - ((wizy(cwiz) + 0) * tx2))
   ciry(0) = (dwn + ((viewy(0) - ((wizx(cwiz) + 0) * ty1)) - ((wizy(cwiz) + 0) * ty2))) - (.15 * zoom)
   cirx(1) = acr + ((viewx(0) - ((wizx(cwiz) + 0) * tx1)) - ((wizy(cwiz) + 5) * tx2))
   ciry(1) = (dwn + ((viewy(0) - ((wizx(cwiz) + 0) * ty1)) - ((wizy(cwiz) + 5) * ty2))) - (.3 * zoom)
   cirx(2) = acr + ((viewx(0) - ((wizx(cwiz) - 0) * tx1)) - ((wizy(cwiz) - 7) * tx2))
   ciry(2) = (dwn + ((viewy(0) - ((wizx(cwiz) - 0) * ty1)) - ((wizy(cwiz) - 7) * ty2))) - (.15 * zoom)
   cirx(3) = acr + ((viewx(0) - ((wizx(cwiz) - 0) * tx1)) - ((wizy(cwiz) + 2) * tx2))
   ciry(3) = (dwn + ((viewy(0) - ((wizx(cwiz) - 0) * ty1)) - ((wizy(cwiz) + 2) * ty2))) - (0 * zoom)
   cirx(4) = acr + ((viewx(0) - ((wizx(cwiz) - 0) * tx1)) - ((wizy(cwiz) - 2) * tx2))
   ciry(4) = (dwn + ((viewy(0) - ((wizx(cwiz) - 0) * ty1)) - ((wizy(cwiz) - 2) * ty2))) - (0 * zoom)
   cirx(5) = acr + ((viewx(0) - ((wizx(cwiz) - 0) * tx1)) - ((wizy(cwiz) - 0) * tx2))
   ciry(5) = (dwn + ((viewy(0) - ((wizx(cwiz) - 0) * ty1)) - ((wizy(cwiz) - 0) * ty2))) - (.25 * zoom)
  
   RETURN

90 REM left
   IF cmode = 0 THEN angle = angle - 1: RETURN
   IF cmode = 1 AND NOT curn = 1 AND curx < edge - 10 THEN curx = curx + 10: RETURN
   IF cmode = 2 AND wizx(turn) < edge - 10 THEN wizx(turn) = wizx(turn) + 10: done = done + 1
   RETURN

100 REM right
   IF cmode = 0 THEN angle = angle + 1: RETURN
   IF cmode = 1 AND NOT curn = 1 AND curx > 9 THEN curx = curx - 10: RETURN
   IF cmode = 2 AND wizx(turn) > 9 THEN wizx(turn) = wizx(turn) - 10: done = done + 1
   RETURN

110 REM up
   IF cmode = 0 AND zoom < 500 THEN zoom = zoom + 10: RETURN
   IF cmode = 1 AND NOT curn = 1 AND cury < edge - 10 THEN cury = cury + 10: RETURN
   IF cmode = 2 AND wizy(turn) < edge - 10 THEN wizy(turn) = wizy(turn) + 10: done = done + 1
   RETURN

120 REM down
   IF cmode = 0 AND zoom > 50 THEN zoom = zoom - 10: RETURN
   IF cmode = 1 AND NOT curn = 1 AND cury > 9 THEN cury = cury - 10: RETURN
   IF cmode = 2 AND wizy(turn) > 9 THEN wizy(turn) = wizy(turn) - 10: done = done + 1
   RETURN

130 REM change turn
   IF turn < players THEN turn = turn + 1
   IF turn = players THEN turn = 0
   RETURN

140 REM cursor
   curxs(0) = acr + ((viewx(0) - ((curx + 0) * tx1)) - (cury + 10) * tx2)
   curys(0) = dwn + ((viewy(0) - ((curx + 0) * ty1)) - (cury + 10) * ty2)
   curxs(1) = acr + ((viewx(0) - ((curx + 10) * tx1)) - (cury + 10) * tx2)
   curys(1) = dwn + ((viewy(0) - ((curx + 10) * ty1)) - (cury + 10) * ty2)
   curxs(2) = acr + ((viewx(0) - ((curx + 10) * tx1)) - (cury + 0) * tx2)
   curys(2) = dwn + ((viewy(0) - ((curx + 10) * ty1)) - (cury + 0) * ty2)
   curxs(3) = acr + ((viewx(0) - ((curx + 0) * tx1)) - (cury + 0) * tx2)
   curys(3) = dwn + ((viewy(0) - ((curx + 0) * ty1)) - (cury + 0) * ty2)
   curxs(4) = acr + ((viewx(0) - ((curx + 5) * tx1)) - (cury + 5) * tx2)
   curys(4) = dwn + ((viewy(0) - ((curx + 5) * ty1)) - (cury + 5) * ty2)
   RETURN

150 REM movement (NOT(!)) 8-)
   upx = wizx(turn) - (curx + 5)
   upy = wizy(turn) - (cury + 5)
  
   curn = 1
  
   IF curx < wizx(turn) THEN GOTO 152
   FOR a = wizx(turn) TO curx + 5 STEP 5
     wizx(turn) = a
     GOSUB 10: REM pointworkout
     GOSUB 20: REM display
     GOSUB 30: REM keypresses
   NEXT a: GOTO 151
  
152 FOR a = wizx(turn) TO curx + 5 STEP -5
     wizx(turn) = a
     GOSUB 10: REM pointworkout
     GOSUB 20: REM display
     GOSUB 30: REM keypresses
   NEXT a
  
151 IF cury > wizy(turn) THEN GOTO 153
   FOR b = wizy(turn) TO cury + 5 STEP -5
     wizy(turn) = b
     GOSUB 10: REM pointworkout
     GOSUB 20: REM display
     GOSUB 30: REM keypresses
   NEXT b: GOTO 154

153 FOR b = wizy(turn) TO cury + 5 STEP 5
     wizy(turn) = b
     GOSUB 10: REM pointworkout
     GOSUB 20: REM display
     GOSUB 30: REM keypresses
   NEXT b
  
154 curn = 0
  
   RETURN

160 REM RR-EE-AA-LL movement
   IF curx = wizx(turn) + 5 AND cury = wizy(turn) + 5 THEN GOTO 161
   IF curx = wizx(turn) - 5 AND cury = wizy(turn) - 5 THEN GOTO 161
   IF curx = wizx(turn) + 5 AND cury = wizy(turn) - 5 THEN GOTO 161
   IF curx = wizx(turn) - 5 AND cury = wizy(turn) + 5 THEN GOTO 161
   RETURN

161 done = 0: leaver = 0
   DO WHILE leaver = 0
     cmode = 2
     GOSUB 30: REM keypresses
     GOSUB 10: REM pointworkout
     GOSUB 20: REM display
     IF done >= moves THEN leaver = 1
   LOOP
   cmode = 1: turn = turn + 1: IF turn > players - 1 THEN turn = 0
   curx = INT(edge / 2) - 2: cury = INT(edge / 2) - 2: done = 0
   RETURN

900 REM end
   SCREEN mode, 1, visscr, visscr
910

