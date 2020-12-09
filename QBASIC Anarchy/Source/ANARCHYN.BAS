   leave = 0: PI = 3.141592654#: zoom = 100: moves = 3: change = 1 / 6
   radplus = (90 * PI) / 180: major = 3.2: minor = change * major
   adder = 0: filled = 1: bcol = 2: lcol = 14: angle = 270: dumber = .01
   DIM viewx(4), viewy(4), gone(7)
   DIM cirx(9), ciry(9), wizx(7), wizy(7), gridx1(15), gridy1(15)
   DIM gridx2(15), gridy2(15), gridx3(15), gridy3(15), gridx4(15), gridy4(15)
   DIM psadangle(360), pcadangle(360), curxs(4), curys(4), txer1(360)
   DIM wiz(7, 50)

   TIMER ON
  
   FOR temp = 0 TO 360
     radangle = (temp * PI) / 180
    
     psadangle(temp) = minor * (SIN(radangle))
     pcadangle(temp) = major * (COS(radangle))
  
   NEXT temp
  
   FOR temp = 0 TO 360
     temper = temp + 90
     IF temper > 360 THEN temper = temper - 360
     txer1(temp) = (pcadangle(temp) - pcadangle(temper)) / 150
   NEXT temp
  
   INPUT "HOW MANY PLAYERS"; players
   IF players < 2 THEN PRINT "DUH!": GOTO 910
   IF players > 8 THEN PRINT "TOO MANY, but Jay would have won anyway": GOTO 910
  
   INPUT "HIGH OR LOW SCREEN RESOLUTION (0=HIGH/1=LOW)"; res
   IF res = 0 THEN mode = 9: scrx = 639: scry = 349: bkcol = 8
   IF res = 1 THEN mode = 7: scrx = 319: scry = 199: bkcol = 1
   halfx = scrx / 2: halfy = scry / 2:

   FOR temp = 0 TO players - 1
     RANDOMIZE TIMER
     wizatt(temp) = 1 + INT(RND * 5)
     wizdef(temp) = 1 + INT(RND * 5)
    
     RESTORE 800
    
     READ wiz(temp, 0)
     FOR temp2 = 1 TO wiz(temp, 0)
       READ wiz(temp, temp2)
     NEXT temp2
   NEXT temp
  
   plays = 4
   edge = (10 * ((plays * 4) - 1)) - 5
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
     GOSUB 40: REM update
     GOSUB 10: REM pointworkout
     GOSUB 20: REM display
     GOSUB 40: REM update
     GOSUB 30: REM keypresses
   LOOP
   GOTO 900: REM end

10 REM pointworkout

   IF angle < 0 THEN angle = 360 + angle
   IF angle > 360 THEN angle = angle - 360
  
   FOR temp = 0 TO 3
     ang = angle + (temp * 90)
     IF ang < 0 THEN ang = 360 + ang
     IF ang > 360 THEN ang = ang - 360
    
     viewx(temp) = x - (zoom * pcadangle(ang))
     viewy(temp) = y - (zoom * psadangle(ang))
   NEXT temp
  
   divide = ((plays * 4) - 1) * 10
   REM tx1 = (viewx(0) - viewx(1)) / divide
   tx1 = zoom * txer1(angle)
   ty1 = (viewy(0) - viewy(1)) / divide
   tx2 = (viewx(1) - viewx(2)) / divide
   ty2 = (viewy(1) - viewy(2)) / divide
   tx3 = (viewx(2) - viewx(3)) / divide
   ty3 = (viewy(2) - viewy(3)) / divide
   tx4 = (viewx(3) - viewx(0)) / divide
   ty4 = (viewy(3) - viewy(0)) / divide
 
   FOR temp = 0 TO (plays * 4) - 1
     temp10 = 10 * temp
    
     gridx1(temp) = viewx(0) - (tx1 * temp10)
     gridy1(temp) = viewy(0) - (ty1 * temp10)
  
     gridx2(temp) = viewx(1) - (tx2 * temp10)
     gridy2(temp) = viewy(1) - (ty2 * temp10)
    
     gridx3(temp) = viewx(2) - (tx3 * temp10)
     gridy3(temp) = viewy(2) - (ty3 * temp10)
  
     gridx4(temp) = viewx(3) - (tx4 * temp10)
     gridy4(temp) = viewy(3) - (ty4 * temp10)
   NEXT temp
  
   GOSUB 140: REM cursor
  
   RETURN

20 REM display
   SCREEN mode, 1, actscr, visscr
  
   tcol = turn + 3
 
   COLOR 14, bkcol
  
   IF filled = 0 THEN GOTO 21
  
   FOR temp = 0 TO 2
     LINE (viewx(temp), viewy(temp))-(viewx(temp + 1), viewy(temp + 1))
   NEXT temp
   LINE (viewx(temp), viewy(temp))-(viewx(0), viewy(0))
  
   PAINT (x, y), bcol, 14
  
21 COLOR lcol
  
   FOR temp = 0 TO (4 * plays) - 1
     tempy = ((4 * plays) - 1) - temp
     LINE (gridx1(temp), gridy1(temp))-(gridx3(tempy), gridy3(tempy))
     LINE (gridx2(temp), gridy2(temp))-(gridx4(tempy), gridy4(tempy))
   NEXT temp
  
   REM IF cmode = 2 GOTO 24
  
   COLOR 9
  
   IF filled = 1 THEN GOTO 22
  
   LINE (curxs(0), curys(0))-(curxs(1), curys(1))
   LINE (curxs(1), curys(1))-(curxs(2), curys(2))
   LINE (curxs(2), curys(2))-(curxs(3), curys(3))
   LINE (curxs(3), curys(3))-(curxs(0), curys(0))

   COLOR 4
   LINE (gridx(0), gridy(0))-(gridx(1), gridy(1))
   LINE (gridx(1), gridy(1))-(gridx(2), gridy(2))
   LINE (gridx(2), gridy(2))-(gridx(3), gridy(3))
   LINE (gridx(3), gridy(3))-(gridx(0), gridy(0))
  
22 IF filled = 1 THEN PAINT (curxs(4), curys(4)), tcol, 14

   COLOR 15
  
   LINE (0, 0)-(scrx, 0)
   LINE (scrx, 0)-(scrx, scry)
   LINE (scrx, scry)-(0, scry)
   LINE (0, scry)-(0, 0)
  
   FOR cwiz = 0 TO players - 1
     IF gone(cwiz) = 1 THEN REM done
     GOSUB 50
     COLOR cwiz + 3
    
     CIRCLE (cirx(0), ciry(0)), wiz(cwiz, 4) * zoom
     CIRCLE (cirx(1), ciry(1)), wiz(cwiz, 8) * zoom
     CIRCLE (cirx(2), ciry(2)), wiz(cwiz, 12) * zoom
     CIRCLE (cirx(3), ciry(3)), wiz(cwiz, 16) * zoom
     CIRCLE (cirx(4), ciry(4)), wiz(cwiz, 20) * zoom
     CIRCLE (cirx(5), ciry(5)), wiz(cwiz, 24) * zoom
    
     CIRCLE (cirx(6), ciry(6)), wiz(cwiz, 28) * zoom
     CIRCLE (cirx(7), ciry(7)), wiz(cwiz, 32) * zoom
     CIRCLE (cirx(8), ciry(8)), wiz(cwiz, 36) * zoom
    
     LINE (cirx(0), ciry(0))-(cirx(1), ciry(1))
     LINE (cirx(0), ciry(0))-(cirx(2), ciry(2))
     LINE (cirx(0), ciry(0))-(cirx(3), ciry(3))
     LINE (cirx(0), ciry(0))-(cirx(4), ciry(4))
    
     LINE (cirx(6), ciry(6))-(cirx(7), ciry(7))
     LINE (cirx(7), ciry(7))-(cirx(8), ciry(8))
     LINE (cirx(8), ciry(8))-(cirx(6), ciry(6))
   NEXT cwiz
 
   GOSUB 200
  
   COLOR 15
  
   LINE (180, 0)-(180, 40)
   LINE (0, 40)-(180, 40)
  
   IF cmode = 0 THEN LOCATE (2), (2): PRINT "Rotate  /  Scale"
   IF cmode = 1 THEN LOCATE (2), (2): PRINT "Cursor  Movement"
   IF cmode = 2 THEN LOCATE (2), (2): PRINT "Wizard  Movement"
  
   SCREEN mode, 1, visscr, actscr
   CLS
   IF actscr = 0 THEN actscr = 1: visscr = 0: RETURN
   IF actscr = 1 THEN actscr = 0: visscr = 1: RETURN

30 REM keypresses
   a$ = INKEY$
   oldx = wizx(turn)
   oldy = wizy(turn)
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
   x = acr + halfx
   y = dwn + halfy
  
   minor = change * major
   RETURN

50 REM WIZARD SYNTAX
   wizxtx = wizx(cwiz) * tx1
   wizxty = wizy(cwiz) * tx2
   wizytx = wizx(cwiz) * ty1
   wizyty = wizy(cwiz) * ty2
   
   temp2 = 0
   FOR temp = 0 TO wiz(cwiz, 0) STEP 4
     cirx(temp2) = viewx(0) - wizxtx + (wiz(cwiz, temp + 1) * tx1) - (wizxty + (wiz(cwiz, temp + 2) * tx2))
     ciry(temp2) = viewy(0) - wizytx + (wiz(cwiz, temp + 1) * ty1) - (wizyty + (wiz(cwiz, temp + 2) * ty2)) - wiz(cwiz, temp + 3) * zoom
     temp2 = temp2 + 1
   NEXT temp
 
   RETURN

90 REM left
   IF cmode = 0 THEN angle = angle - 1: RETURN
   IF cmode = 1 AND NOT curn = 1 AND curx < edge - 10 THEN curx = curx + 10: RETURN
   IF cmode = 2 AND wizx(turn) < edge - 1 THEN wizx(turn) = wizx(turn) + 10: done = done + 1
   RETURN

100 REM right
   IF cmode = 0 THEN angle = angle + 1: RETURN
   IF cmode = 1 AND NOT curn = 1 AND curx > 9 THEN curx = curx - 10: RETURN
   IF cmode = 2 AND wizx(turn) > 9 THEN wizx(turn) = wizx(turn) - 10: done = done + 1
   RETURN

110 REM up
   IF cmode = 0 AND zoom < 900 THEN zoom = zoom + 10: RETURN
   IF cmode = 1 AND NOT curn = 1 AND cury < edge - 10 THEN cury = cury + 10: RETURN
   IF cmode = 2 AND wizy(turn) < edge - 1 THEN wizy(turn) = wizy(turn) + 10: done = done + 1
   RETURN

120 REM down
   IF cmode = 0 AND zoom > 50 THEN zoom = zoom - 10: RETURN
   IF cmode = 1 AND NOT curn = 1 AND cury > 9 THEN cury = cury - 10: RETURN
   IF cmode = 2 AND wizy(turn) > 9 THEN wizy(turn) = wizy(turn) - 10: done = done + 1
   RETURN

130 REM change turn
131 turn = turn + 1: IF turn > players - 1 THEN turn = 0
   IF gone(turn) = 1 THEN GOTO 131
   curx = wizx(turn) - 5: cury = wizy(turn) - 5: REM done = 0
   RETURN

140 REM cursor
   curxs(0) = (viewx(0) - ((curx + 0) * tx1)) - (cury + 10) * tx2
   curys(0) = (viewy(0) - ((curx + 0) * ty1)) - (cury + 10) * ty2
   curxs(1) = (viewx(0) - ((curx + 10) * tx1)) - (cury + 10) * tx2
   curys(1) = (viewy(0) - ((curx + 10) * ty1)) - (cury + 10) * ty2
   curxs(2) = (viewx(0) - ((curx + 10) * tx1)) - (cury + 0) * tx2
   curys(2) = (viewy(0) - ((curx + 10) * ty1)) - (cury + 0) * ty2
   curxs(3) = (viewx(0) - ((curx + 0) * tx1)) - (cury + 0) * tx2
   curys(3) = (viewy(0) - ((curx + 0) * ty1)) - (cury + 0) * ty2
   curxs(4) = (viewx(0) - ((curx + 5) * tx1)) - (cury + 5) * tx2
   curys(4) = (viewy(0) - ((curx + 5) * ty1)) - (cury + 5) * ty2
  
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
     GOSUB 170: REM collision detection
     GOSUB 10: REM pointworkout
     GOSUB 20: REM display
     GOSUB 170: REM collision detection
     IF done >= moves THEN leaver = 1
   LOOP
   GOSUB 170: REM collision detect
   cmode = 1: GOSUB 130: REM change turn
   RETURN

170 REM collision detection
   FOR a = 0 TO players - 1
     IF wizx(turn) = wizx(a) AND wizy(turn) = wizy(a) AND NOT a = turn AND NOT gone(a) = 1 THEN playhit = a: a = players - 1: GOSUB 180: REM collision
   NEXT a
   RETURN

180 REM collision
   BEEP
   wizx(turn) = oldx: wizy(turn) = oldy: deady = 0
  
   fightadd1 = INT(RND * 13): fightadd2 = INT(RND * 15)
   IF wizatt(turn) + fightadd1 > 2 + wizdef(playhit) + fightadd2 THEN dead = playhit: deady = 1
   IF wizatt(turn) + fightadd1 > wizdef(playhit) + fightadd2 THEN wizdef(playhit) = wizdef(playhit) - 1: IF wizdef(playhit) = 0 THEN dead = playhit: deady = 1
   IF deady = 1 THEN gone(playhit) = 1: BEEP: BEEP: BEEP
  
   done = moves
   nomore = 0
 
   FOR temp = 0 TO players - 1
     handz(temp) = zoom * .3
     IF gone(temp) = 1 THEN nomore = nomore + 1
     IF gone(temp) = 0 THEN winner = temp
   NEXT temp
 
   IF nomore = players - 1 THEN GOSUB 250: REM winning
  
   RETURN

200 REM BLOKEY BLOKEY
   COLOR 15
   LINE (scrx - 50, scry - 57)-(scrx - 50, scry)
   LINE (scrx - 50, scry - 57)-(scrx, scry - 57)
   PAINT (scrx - 45, scry - 55), 14, 15
  
   COLOR tcol
  
   CIRCLE (scrx - 21, scry - 20), 8
   CIRCLE (scrx - 6, scry - 35), 3
   CIRCLE (scrx - 42, scry - 20), 3
   CIRCLE (scrx - 15, scry - 5), 3
   CIRCLE (scrx - 27, scry - 5), 3
   CIRCLE (scrx - 21, scry - 30), 5
   
   LINE (scrx - 20, scry - 20)-(scrx - 6, scry - 35)
   LINE (scrx - 20, scry - 20)-(scrx - 42, scry - 20)
   LINE (scrx - 20, scry - 20)-(scrx - 15, scry - 5)
   LINE (scrx - 20, scry - 20)-(scrx - 27, scry - 5)
   
   LINE (scrx - 6, scry - 55)-(scrx - 3, scry - 35)
   LINE (scrx - 3, scry - 35)-(scrx - 9, scry - 35)
   LINE (scrx - 9, scry - 35)-(scrx - 6, scry - 55)
   RETURN

250 REM winner
   SCREEN 2
   PRINT "Player "; winner + 1; " wins"
   DO
   LOOP UNTIL INKEY$ <> ""
   STOP
   RETURN


REM !!!!!!WIZARD 3D CO-ORDINATE DATA!!!!!
REM -------------------------------------

800 DATA 36
    DATA 0,0,.15,.08
    DATA 0,5,.3,.03
    DATA 0,-7,.15,.03
    DATA 0,2,0,.03
    DATA 0,-2,0,.03
    DATA 0,0,.25,.05
    DATA 0,5,.5,.001
    DATA 0,6,.3,.001
    DATA 0,4,.3,.001

900 REM end
   SCREEN mode, 1, visscr, visscr
910

