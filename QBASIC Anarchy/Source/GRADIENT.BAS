PI = 3.141592654#
DIM x(360), y(360)

FOR temp = 0 TO 360
   tempang = (temp * PI) / 180
   x(temp) = SIN(tempang)
   y(temp) = COS(tempang)
NEXT temp

SCREEN 9
COLOR 8
FOR temp = 0 TO 360
   tempang = (temp * PI) / 180
   xer = 50 * x(temp)
   yer = 50 * y(temp)

   LINE (100 - xer, 100 - yer)-(100 - xer, 100 - yer)
NEXT temp
PAINT (100, 100), 8, 8

GOTO 20
COLOR 7
FOR temp = 0 TO 360 STEP .5
   tempang = (temp * PI) / 180
  
   x = 30 * SIN(tempang)
   y = 30 * COS(tempang)

   LINE (100 - x, 100 - y)-(100 - x, 100 - y)
NEXT temp
PAINT (100, 100), 7, 7

COLOR 15
FOR temp = 0 TO 360 STEP .5
   tempang = (temp * PI) / 180
 
   x = 10 * SIN(tempang)
   y = 10 * COS(tempang)

   LINE (100 - x, 100 - y)-(100 - x, 100 - y)
NEXT temp
PAINT (100, 100), 15, 15


20 REM wiggly bits

COLOR 7
FOR bloke = .5 TO 50 STEP .09
x = INT(RND * 360)
FOR temp = x TO x + 360 + bloke STEP bloke
   temper = temp
   IF temp > 360 THEN temper = temp - 360
   IF temp > 720 THEN temper = temp - 720

   xer = (1 * bloke) * x(temper)
   yer = (1 * bloke) * y(temper)

   LINE (100 - xer, 100 - yer)-(100 - xer, 100 - yer)
NEXT temp
NEXT bloke

COLOR 15
FOR bloke = .5 TO 15 STEP .5
x = INT(RND * 360)
FOR temp = x TO x + 360 + bloke STEP bloke
   temper = temp
   IF temp > 360 THEN temper = temp - 360
   IF temp > 720 THEN temper = temp - 720
  
   xer = (2 * bloke) * x(temper)
   yer = (2 * bloke) * y(temper)

   LINE (100 - xer, 100 - yer)-(100 - xer, 100 - yer)
NEXT temp
NEXT bloke

