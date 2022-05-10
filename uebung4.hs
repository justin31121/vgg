module Uebung4 where

{-
Übung zur Vorlesung
Funktionale Programmierung
Sommersemester 2022
Übungsblatt 4
-}

{-
Ausgabe: 06.05.2022
Abgabe: keine
-}

{-
Aufgabe 4.1 - Lexikographisches Produkt
-}

{-
a)
Implementieren Sie eine Haskell-Funktion
bag :: Eq a => [a] -> [(a, Int)],
die die Vorkommen einzelner Elemente in der Liste zählt und eine Liste
von Tupeln zurückgibt, die in der ersten Projektion das Element hat und in
der zweiten Projektion die Anzahl dieses Elements in der Argumentliste.

Nutzen Sie zur Implementierung foldl auf sinnvolle Weise.

Beispielaufrufe:
bag [1,1,1,2,2,3,4,4,4,5]
~>
[(1,3),(2,2),(3,1),(4,3),(5,1)]
[(1,3),(2,2),(3,1),(4,3),(5,1)]

bag "Hello World!"
~>
[('H',1),('e',1),('l',3),('o',2),(' ',1),('W',1),('r',1),('d',1),('!',1)]
[('H',1),('e',1),('l',3),('o',2),(' ',1),('W',1),('r',1),('d',1),('!',1)]
-}

bag :: Eq a => [a] -> [(a, Int)]
bag = foldl f [] 
  where
    f :: Eq a =>  [(a, Int)] -> a -> [(a, Int)]
    f [] t = [(t, 1)]
    f ((x, n):ys) t
      | x == t = (x,n+1) : ys
      | otherwise = (x,n) : f ys t

{-
b)
Implementieren Sie eine Haskell-Funktion
lexOrd :: Ord a => [a] -> [a] -> Ordering,
die zwei Listen lexikographisch ordnet
(https://de.wikipedia.org/wiki/Lexikographische_Ordnung).

(x,y) < (x', y') <=> (x < x') v (x==x' und y < y')
(x,y) >= (x', y') <=> (x >= x') und (x/=x' v y >= y')

a b < c d <=> (a < c) v (a==c und b < d)
-}

lexOrd :: Ord a => [a] -> [a] -> Ordering
lexOrd [] [] = EQ
lexOrd xs [] = GT
lexOrd [] xs = LT
lexOrd (x:xs) (y:ys)
  | x == y && lex' == EQ = EQ
  | x <  y || (x==y && lex'==LT) = LT
  | otherwise = GT
  where lex' = lexOrd xs ys

{-
c)
Implementieren Sie eine Haskell-Funktion
bagOrd :: Ord a => [a] -> [a] -> Ordering,
die zwei Listen wie folgt ordnet:
Zähle die Vorkommen aller Elemente und finde das
maximale Vorkommen eines Elements für jede Liste.
Vergleiche die Listen anhand dieser Werte.

Beispielaufrufe:
bagOrd [1,1,1,2,2,3,4,4,4,5] [3,3,3,3] ~> LT

bagOrd [1,1,1,2,2,3,4,4,4,5] [1000,10] ~> GT

bagOrd [1,1,1] [5,5,5,6,7,8] ~> EQ
-}


 
bagOrd :: Ord a => [a] -> [a] -> Ordering
bagOrd ts ts' = compare m m'
  where
    m  = max' $ bag ts
    m' = max' $ bag ts'

    max' :: [(a, Int)] -> Int
    max' = foldl (\acc next -> max acc $ snd next) 0
    
{-
d)
Implementieren Sie eine Haskell-Funktion
lexProd :: Ord a => [a] -> [a] -> Ordering,
die dem lexikographische Produkt von der Längenordnung auf Listen,
bagOrd und lexOrd entspricht.

Das heißt, dass die Listen zuerst anhand ihrer Länge verglichen werden sollen.
Ist ihre Länge gleich, sollen Sie mittels bagOrd verglichen werden und falls Sie unter
bagOrd ebenfalls gleich sind, soll das Ergebnis von lexOrd zurückgegeben werden.

Beispielaufrufe:

lexProd [1,1,1,2,2,3,4,4,4,5] [1,1,1,2,2,3,4,4,4,5] ~> EQ

lexProd [1,1,1,2,2,3,4,4,4,5] [1,1,2,1,2,3,4,4,4,5] ~> LT

lexProd [1,1,1,1,2,3,4,4,4,5] [2,1,1,1,2,3,4,4,4,5] ~> GT
-}

lexProd :: Ord a => [a] -> [a] -> Ordering
lexProd xs ys
  | lOrd/=EQ = lOrd
  | bOrd/=EQ = bOrd
  | otherwise = leOrd
  where lOrd = compare (length xs) (length ys)
        bOrd = bagOrd xs ys
        leOrd = lexOrd xs ys

{-
Aufgabe 4.2 - Matrizen
-}

{-
Wir können Vektoren als Listen von Integern modellieren.
Eine Matrix kann dann als Liste von Spaltenvektoren modelliert werden.

Sie dürfen der Einfachheit halber für alle Matrizen voraussetzen,
dass alle Spaltenvektoren dieselbe Länge haben und nicht leer sind.
-}

type Spalte = [Integer]
type Matrix = [Spalte]

{-
a)
Implementieren Sie eine Haskell-Funktion
skalarProdukt :: [Integer] -> [Integer] -> Integer,
die das Skalarprodukt zweier Vektoren berechnet.
Vektoren werden hier als Listen von Integern modelliert und Sie dürfen
der Einfachheit halber voraussetzen, dass die beiden Argumentlisten
dieselbe Länge haben.
-}

skalarProdukt :: [Integer] -> [Integer] -> Integer
skalarProdukt xs ys =
  foldl (\acc (a, b) -> acc + a * b) 0 $ zip xs ys
  
{-

b)
Implementieren Sie eine Haskell-Funktion
transpose :: Matrix -> Matrix,
die eine Matrix transponiert.

Tipp: Eine Matrix ohne Einträge (mit leeren Spaltenvektoren) kann gleichbedeutend
zur leeren Liste behandelt werden. Es kann Sinn machen, diesen Fall explizit mittels
Pattern Matching zu behandeln.

Beispielaufruf:
transpose [[1,2,3], [4,5,6], [7,8,9]] ~> [[1,4,7],[2,5,8],[3,6,9]]
                                         [[1,4,7],[2,5,8],[3,6,9]]
transpose [[1,4,7],[2,5,8],[3,6,9]] ~> [[1,2,3],[4,5,6],[7,8,9]]
                                       [[1,2,3],[4,5,6],[7,8,9]]
-}
                           
applyNtimes :: (a -> a) -> Int -> a -> a
applyNtimes f n x = foldl (\acc next -> f acc) x [1..n]

transpose :: Matrix -> Matrix
transpose [] = []
transpose matrix = foldr (\index acc -> nthRow index : acc) [] times
  where
    times = [0..(len-1)]
    len = length $ head $ matrix

    nthRow :: Int -> [Integer]
    nthRow index = foldr (\next acc -> (head next) : acc) [] $ dropNRows index

    dropNRows :: Int -> Matrix
    dropNRows n = map (\x -> applyNtimes tail n x) matrix

{-
c)
Implementieren Sie eine Haskell-Funktion
matrixMult :: Matrix -> Matrix -> Matrix,
die der Multiplikation zweier Matrizen entspricht.
Sie dürfen der Einfachheit halber voraussetzen,
dass die Dimensionen der Argumentmatrizen korrekt sind.

A^(m x n) x B^(n x p) = C^(m x p)
-}

matrixMult :: Matrix -> Matrix -> Matrix
matrixMult a b = [ [ cell x y | x <- [0..m] ] | y <-[0..n] ]
  where
    m = (length $ head a) - 1
    n = (length b) - 1

    a' = transpose a

    cell :: Int -> Int -> Integer
    cell x y = skalarProdukt (b !! y) (a' !! x)

{-
Aufgabe 4.* - Fibonacci mal ganz anders...
-}

{-
Sie können die n-te Fibonacci-Zahl berechnen, indem Sie die Matrix

1 1
1 0

mit n exponenzieren ((n-1)-mal mit sich selbst multiplizieren).

Mathematisch ausgedrückt kann die Fibonacci-Sequenz also über folgendes
lineares Gleichungssystem beschrieben werden:

(F_{n+2} F_{n+1})       (1  1)    (F_{n+1} F_{n}  )
(               )   =   (    ) *  (               )
(F_{n+1} F_{n-1})       (1  0)    (F_{n}   F_{n-1})

Implementieren Sie eine Haskell-Funktion
fib :: Int -> Integer,
die die n-te Fibonacci-Zahl nach diesem Prinzip berechnet und vergleichen
Sie die Laufzeit und den Speicherbedarf zu den Implementierungen von Blatt 3.
-}

fib :: Int -> Integer
fib n = m
  where
    [[_, _], [_, m]] =
      foldl (\acc next -> matrixMult matrix acc) matrix [1..n]
    matrix = [[1,1], [1,0]]
