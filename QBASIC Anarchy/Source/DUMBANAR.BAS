   leave = 0: PI = 3.141592654#: zoom = 100: moves = 1: change = 6
   major = 3.2: minor = (1 / change) * major: crswiz = 9: adder = 2
   filled = 1: bcol = 2: lcol = 14: angle = 270: wid = 10
   DIM viewx(4), viewy(4), gone(7)
   DIM cirx(9), ciry(9), wizx(7), wizy(7), gridx1(15), gridy1(15)
   DIM gridx2(15), gridy2(15), gridx3(15), gridy3(15), gridx4(15), gridy4(15)
   DIM psadangle(360), pcadangle(360), curxs(4), curys(4), col(7)
   DIM wiz(7, 50), newx(360), newy(360)

   TIMER ON
  
   FOR temp = 0 TO 360
     radangle = (temp * PI) / 180
    
     psadangle(temp) = minor * (SIN(radangle))
     pcadangle(temp) = major * (COS(radangle))
   NEXT temp
   
   GOSUB 240: REM start variables
 
   DO WHILE leave < 1
     GOSUB 30: REM keypresses
     GOSUB 40: REM update
     GOSUB 10: REM point workout
     GOSUB 20: REM display
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
  
   tx1 = (viewx(0) - viewx(1)) / 150
   ty1 = (viewy(0) - viewy(1)) / 150
   tx2 = (viewx(1) - viewx(2)) / 150
   ty2 = (viewy(1) - viewy(2)) / 150
   tx3 = (viewx(2) - viewx(3)) / 150
   ty3 = (viewy(2) - viewy(3)) / 150
   tx4 = (viewx(3) - viewx(0)) / 150
   ty4 = (viewy(3) - viewy(0)) / 150
                   
   FOR temp = 0 TO 15
     temp10 = temp * 10

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
  
   tcol = col(turn)
 
   COLOR 14, bkcol
  
   IF filled = 0 THEN GOTO 21
  
   FOR temp = 0 TO 2
     LINE (viewx(temp), viewy(temp))-(viewx(temp + 1), viewy(temp + 1))
   NEXT temp
   LINE (viewx(3), viewy(3))-(viewx(0), viewy(0))
  
   PAINT (x, y), bcol, 14
  
21 COLOR lcol
  
   FOR temp = 0 TO 15
     tempy = 15 - temp
     LINE (gridx1(temp), gridy1(temp))-(gridx3(tempy), gridy3(tempy))
     LINE (gridx2(temp), gridy2(temp))-(gridx4(tempy), gridy4(tempy))
   NEXT temp
  
   IF moving = 1 THEN GOTO 25
   IF filled = 1 THEN GOTO 22
  
   COLOR col(turn)
   LINE (curxs(0), curys(0))-(curxs(1), curys(1))
   LINE (curxs(1), curys(1))-(curxs(2), curys(2))
   LINE (curxs(2), curys(2))-(curxs(3), curys(3))
   LINE (curxs(3), curys(3))-(curxs(0), curys(0))

22 IF filled = 1 THEN PAINT (curxs(4), curys(4)), col(turn), 14

25 FOR cwiz = 0 TO players - 1
     IF gone(cwiz) = 1 THEN GOTO 23
     GOSUB 50
     COLOR col(cwiz)
    
     IF cwiz <> turn THEN GOTO 24
    
     COLOR 15
     REM CIRCLE (cirx(0), ciry(0)), wiz(cwiz, 4) * zoom
     COLOR col(cwiz)
     IF start = 0 THEN GOSUB 270: REM weird
24   CIRCLE (cirx(1), ciry(1)), wiz(cwiz, 8) * zoom
     CIRCLE (cirx(0), ciry(0)), wiz(cwiz, 4) * zoom
     CIRCLE (cirx(2), ciry(2)), wiz(cwiz, 12) * zoom
     CIRCLE (cirx(3), ciry(3)), wiz(cwiz, 16) * zoom
     CIRCLE (cirx(4), ciry(4)), wiz(cwiz, 20) * zoom
     CIRCLE (cirx(5), ciry(5)), wiz(cwiz, 24) * zoom
    
     LINE (cirx(0), ciry(0))-(cirx(1), ciry(1))
     LINE (cirx(0), ciry(0))-(cirx(2), ciry(2))
     LINE (cirx(0), ciry(0))-(cirx(3), ciry(3))
     LINE (cirx(0), ciry(0))-(cirx(4), ciry(4))
    
23 NEXT cwiz
 
   GOSUB 200
  
   COLOR 15
  
   LINE (180, 0)-(180, 40)
   LINE (0, 40)-(180, 40)
  
   IF cmode = 0 THEN LOCATE (2), (2): PRINT "Rotate  /  Scale"
   IF cmode = 1 THEN LOCATE (2), (2): PRINT "Cursor  Movement"
   IF cmode = 2 THEN LOCATE (2), (2): PRINT "Wizard  Movement"
  
   LINE (0, 0)-(scrx, 0)
   LINE (scrx, 0)-(scrx, scry)
   LINE (scrx, scry)-(0, scry)
   LINE (0, scry)-(0, 0)
   SCREEN mode, 1, visscr, actscr
   CLS
   IF actscr = 0 THEN actscr = 1: visscr = 0: RETURN
   IF actscr = 1 THEN actscr = 0: visscr = 1: RETURN

30 REM keypresses
31 DO
     a$ = INKEY$
   LOOP UNTIL a$ <> ""
   IF INKEY$ <> "" GOTO 31
   GOTO 33

32 a$ = INKEY$
33 oldx = wizx(turn)
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
   IF a$ = "*" THEN GOSUB 210: REM character stats screen . . . by Jay
   IF a$ = "+" THEN IF change >= 6 THEN change = change - .5: GOSUB 230
   IF a$ = "-" THEN IF change <= 15 THEN change = change + .5: GOSUB 230
   IF a$ = "#" THEN DO: LOOP UNTIL INKEY$ <> ""
   RETURN

40 REM update
   x = acr + halfx
   y = dwn + halfy
  
   crswiz = 9
   FOR temp = 0 TO players - 1
     IF curx = wizx(temp) AND cury = wizy(temp) THEN crswiz = temp: temp = players - 1
   NEXT temp
41 RETURN

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
   IF cmode = 0 THEN angle = angle - adder: RETURN
   IF cmode = 1 AND NOT curn = 1 AND curx < 136 THEN curx = curx + 10: RETURN
   IF cmode = 2 AND wizx(turn) < 140 THEN wizx(turn) = wizx(turn) + 10: done = done + 1
   RETURN

100 REM right
   IF cmode = 0 THEN angle = angle + adder: RETURN
   IF cmode = 1 AND NOT curn = 1 AND curx > 14 THEN curx = curx - 10: RETURN
   IF cmode = 2 AND wizx(turn) > 9 THEN wizx(turn) = wizx(turn) - 10: done = done + 1
   RETURN

110 REM up
   IF cmode = 0 AND zoom < 900 THEN zoom = zoom + 10: RETURN
   IF cmode = 1 AND NOT curn = 1 AND cury < 136 THEN cury = cury + 10: RETURN
   IF cmode = 2 AND wizy(turn) < 140 THEN wizy(turn) = wizy(turn) + 10: done = done + 1
   RETURN

120 REM down
   IF cmode = 0 AND zoom > 50 THEN zoom = zoom - 10: RETURN
   IF cmode = 1 AND NOT curn = 1 AND cury > 14 THEN cury = cury - 10: RETURN
   IF cmode = 2 AND wizy(turn) > 9 THEN wizy(turn) = wizy(turn) - 10: done = done + 1
   RETURN

130 REM change turn
131 turn = turn + 1: IF turn > players - 1 THEN turn = 0
   IF gone(turn) = 1 THEN GOTO 131
  
   DO
     a$ = INKEY$
   LOOP UNTIL a$ = "": REM clear buffer
  
   curx = wizx(turn): cury = wizy(turn)
   RETURN

140 REM cursor
   IF filled = 1 THEN GOTO 141
  
   curxs(0) = (viewx(0) - ((curx - 5) * tx1)) - (cury + 5) * tx2
   curys(0) = (viewy(0) - ((curx - 5) * ty1)) - (cury + 5) * ty2
   curxs(1) = (viewx(0) - ((curx + 5) * tx1)) - (cury + 5) * tx2
   curys(1) = (viewy(0) - ((curx + 5) * ty1)) - (cury + 5) * ty2
   curxs(2) = (viewx(0) - ((curx + 5) * tx1)) - (cury - 5) * tx2
   curys(2) = (viewy(0) - ((curx + 5) * ty1)) - (cury - 5) * ty2
   curxs(3) = (viewx(0) - ((curx - 5) * tx1)) - (cury - 5) * tx2
   curys(3) = (viewy(0) - ((curx - 5) * ty1)) - (cury - 5) * ty2
   RETURN

141 curxs(4) = (viewx(0) - ((curx + 0) * tx1)) - (cury + 0) * tx2
   curys(4) = (viewy(0) - ((curx + 0) * ty1)) - (cury + 0) * ty2
  
   RETURN

150 REM movement (not)
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

160 REM Real Movement
   IF crswiz = turn THEN GOTO 161
   RETURN

161 done = 0: leaver = 0
   moving = 1
   DO WHILE leaver = 0
     cmode = 2
     GOSUB 30: REM keypresses
     GOSUB 170: REM collision detection
     GOSUB 40: REM update
     GOSUB 10: REM pointworkout
     GOSUB 20: REM display
     GOSUB 170: REM collision detection
     IF done >= moves THEN leaver = 1
   LOOP
   GOSUB 170: REM collision detect
   cmode = 1: moving = 0
   GOSUB 130: REM change turn
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
   IF deady = 1 THEN GOSUB 190: gone(playhit) = 1
  
   done = moves
   nomore = 0
 
   FOR temp = 0 TO players - 1
     handz(temp) = zoom * .3
     IF gone(temp) = 1 THEN nomore = nomore + 1
     IF gone(temp) = 0 THEN winner = temp
   NEXT temp
 
   IF nomore = players - 1 THEN GOSUB 250: REM winning
  
   RETURN

190 REM DEATH
   FOR tempmov = 0 TO 6
     FOR temper = 0 TO 9
       wiz(playhit, temper * 4) = wiz(playhit, temper * 4) + .09
     NEXT temper
    
     GOSUB 40: REM update
     GOSUB 20: REM display
   NEXT tempmov
  
   FOR tempmov = 0 TO 4
     FOR temper = 0 TO 9
       wiz(playhit, temper * 4) = wiz(playhit, temper * 4) - .18
       IF wiz(playhit, temper * 4) < 0 THEN wiz(playhit, temper * 4) = 0
     NEXT temper
   
     GOSUB 40: REM update
     GOSUB 20: REM display
   NEXT tempmov
  
   FOR tempmov = 0 TO 3
     FOR temper = 0 TO 9
       wiz(playhit, temper * 3) = wiz(playhit, temper * 3) + .5
   NEXT temper
  
     GOSUB 40: REM update
     GOSUB 20: REM display
   NEXT tempmov
  
   BEEP: BEEP: BEEP
   RETURN

200 REM BLOKEY BLOKEY
   COLOR 15
   LINE (scrx - 50, scry - 57)-(scrx - 50, scry)
   LINE (scrx - 50, scry - 57)-(scrx, scry - 57)
   PAINT (scrx - 45, scry - 55), 0, 15
  
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

210 REM character stats screen
   IF crswiz = 9 THEN RETURN
   SCREEN 2
   PRINT : PRINT
   PRINT "  attack "; wizatt(crswiz)
   PRINT "  defence"; wizdef(crswiz)
   PRINT : PRINT
   PRINT "   Press any key to exit"
   DO
   LOOP UNTIL INKEY$ <> ""
   RETURN

220 REM PLACING
  
   IF players > 2 GOTO 221
   wizx(0) = 25: wizy(0) = 75
   wizx(1) = 125: wizy(1) = 75
   RETURN

221 IF players > 3 GOTO 222
   wizx(0) = 75: wizy(0) = 125
   wizx(1) = 15: wizy(1) = 25
   wizx(2) = 135: wizy(2) = 25
   RETURN

222 IF players > 4 GOTO 223
   wizx(0) = 15: wizy(0) = 15
   wizx(1) = 15: wizy(1) = 135
   wizx(2) = 135: wizy(2) = 135
   wizx(3) = 135: wizy(3) = 15
   RETURN

223 IF players > 5 GOTO 224
   wizx(0) = 75: wizy(0) = 135
   wizx(1) = 135: wizy(1) = 95
   wizx(2) = 15: wizy(2) = 95
   wizx(3) = 105: wizy(3) = 25
   wizx(4) = 45: wizy(4) = 25
   RETURN

224 IF players > 6 GOTO 225
   wizx(0) = 75: wizy(0) = 145
   wizx(1) = 5: wizy(1) = 125
   wizx(2) = 145: wizy(2) = 125
   wizx(3) = 145: wizy(3) = 25
   wizx(4) = 5: wizy(4) = 25
   wizx(5) = 75: wizy(5) = 5
   RETURN

225 IF players > 7 GOTO 226
   wizx(0) = 75: wizy(0) = 145
   wizx(1) = 15: wizy(1) = 135
   wizx(2) = 135: wizy(2) = 135
   wizx(3) = 145: wizy(3) = 45
   wizx(4) = 5: wizy(4) = 45
   wizx(5) = 105: wizy(5) = 5
   wizx(6) = 45: wizy(6) = 5
   RETURN

226
   wizx(0) = 5: wizy(0) = 5
   wizx(1) = 5: wizy(1) = 75
   wizx(2) = 5: wizy(2) = 145
   wizx(3) = 75: wizy(3) = 5
   wizx(4) = 75: wizy(4) = 145
   wizx(5) = 145: wizy(5) = 145
   wizx(6) = 145: wizy(6) = 75
   wizx(7) = 145: wizy(7) = 5
RETURN

230 REM re-calculation
   minor = (1 / change) * major
  
   FOR temp = 0 TO 360
     radangle = (temp * PI) / 180
   
     psadangle(temp) = minor * (SIN(radangle))
     pcadangle(temp) = major * (COS(radangle))
   NEXT temp
   RETURN

240 REM start variables
   INPUT "HOW MANY PLAYERS"; players
   IF players < 2 THEN PRINT "DUH!": GOTO 901
   IF players > 8 THEN PRINT "TOO MANY, but Jay would have won anyway": GOTO 901
 
   INPUT "HIGH OR LOW SCREEN RESOLUTION (0=HIGH/1=LOW)"; res
   IF res = 0 THEN mode = 9: scrx = 639: scry = 349: bkcol = 8
   IF res = 1 THEN mode = 7: scrx = 319: scry = 199: bkcol = 1
   halfx = scrx / 2: halfy = scry / 2:

   FOR temp = 0 TO 90 STEP 1
     tempang = ((4 * temp) * PI) / 180
     newx(temp) = SIN(tempang)
     newy(temp) = COS(tempang)
   NEXT temp

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

   RESTORE 810
   FOR temp = 1 TO 15
     COLOR 15
     PRINT temp; ":- ";
     COLOR temp
     READ temp$
     PRINT temp$
   NEXT temp

   FOR temp = 1 TO players
     PRINT "Player "; temp; " colour";
     INPUT col(temp - 1)
     IF col(temp - 1) < 1 OR col(temp - 1) > 15 THEN temp = temp - 1
   NEXT temp
  
   GOSUB 220: REM placing

   curx = wizx(0): cury = wizy(0)
   turn = 0
  
   GOSUB 260: REM START THING
   GOSUB 40: REM update
   GOSUB 10: REM workout
   GOSUB 20: REM display
  
   RETURN

250 REM winner
   SCREEN 2
   PRINT "Player "; winner + 1; " wins"
   DO
   LOOP UNTIL INKEY$ <> ""
   STOP
   RETURN

260 REM START THING
   start = 1
   FOR temper = 1 TO 9
     FOR eventemper = 0 TO players - 1
       wiz(eventemper, temper * 4) = wiz(eventemper, temper * 4) + 1
     NEXT eventemper
   NEXT temper
  
   FOR tempmov = 0 TO 9
     FOR temper = 0 TO 9
       FOR eventemper = 0 TO players - 1
         wiz(eventemper, temper * 4) = wiz(eventemper, temper * 4) - .1
       NEXT eventemper
     NEXT temper
  
     GOSUB 40: REM update
     GOSUB 10: REM workout
     GOSUB 20: REM display
   NEXT tempmov
   start = 0
RETURN

270 REM weird
   wid = wiz(cwiz, 4) * zoom
   COLOR 15
   FOR temp = 0 TO 90
     xer = wid * newx(temp)
     yer = wid * newy(temp)
 
     temper = temp + 1
     IF temper > 90 THEN temper = temper - 90
     xer2 = wid * newx(temper)
     yer2 = wid * newy(temper)

     LINE (cirx(0) - xer, ciry(0) - yer)-(cirx(0) - xer2, ciry(0) - yer2)
   NEXT temp
   PAINT (cirx(0), ciry(0)), col(cwiz), 15

   COLOR 15
   FOR bloke = .5 TO wid STEP .5
     bob = INT(RND * 360)
     FOR temp = bob TO bob + 90 STEP bloke
 
       temper = temp
       IF temp > 90 THEN temper = temp - 90
       IF temp > 180 THEN temper = temp - 180
       IF temp > 270 THEN temper = temp - 270
       IF temp > 360 THEN temper = temp - 360

       xer = (bloke) * newx(4 * temper)
       yer = (bloke) * newy(4 * temper)

       LINE (cirx(0) - xer, ciry(0) - yer)-(cirx(0) - xer, ciry(0) - yer)
     NEXT temp
   NEXT bloke
RETURN

REM !!!!!!WIZARD 3D CO-ORDINATE DATA!!!!!
REM -------------------------------------

800 DATA 24
    DATA 0,0,.15,.08
    DATA 0,5,.3,.03
    DATA 0,-7,.15,.03
    DATA 0,2,0,.03
    DATA 0,-2,0,.03
    DATA 0,0,.25,.05
    DATA 0,5,.5,.001
    DATA 0,6,.3,.001
    DATA 0,4,.3,.001

810 DATA "BLUE","GREEN","CYAN","RED","MAGENTA","BROWN","WHITE","GRAY"
    DATA "L BLUE","L GREEN","L CYAN","L RED","L MAGENTA","YELLOW","BRIGHT WHITE"

900 REM end
    SCREEN mode, 1, visscr, visscr

901 END

