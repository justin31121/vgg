module Uebung6 where

{-
Übung zur Vorlesung
Funktionale Programmierung
Sommersemester 2022
Übungsblatt 6
-}

{-
Block: 1
Ausgabe: 20.05.2022
Abgabe: keine
-}

{-
!!!!!!!!!!!
Dieses Blatt führt einen neuen Datentyp Nat für natürliche Zahlen ein.
Viele der Aufgaben können natürlich gelöst werden, indem man z.B. Funktionen
nat2Int und int2Nat implementiert.
Das ist aber nicht Sinn der Aufgaben und würde in der Klausur explizit
ausgeschlossen werden (keine Punkte geben).
Sie sollen hier lernen/üben Faltungen für beliebige algebraische Datentypen
zu implementieren und anzuwenden. Nutzen Sie also die Gelegenheit.
Falls Sie die Faltungen tatsächlich nicht implementiert bekommen, können Sie
die Aufgaben trotzdem per Pattern Matching lösen. Dann raten wir Ihnen aber
dringend die Übungen wahrzunehmen und Fragen zu stellen.
!!!!!!!!!!!
-}

{-
Aufgabe 6.1 - Nat
-}

data Nat = Z | S Nat deriving Show

{-
Gegeben sei der Haskell-Datentyp Nat für natürliche Zahlen.
Die natürlichen Zahlen 0,1,2,3 entsprechen dann z.B. den folgenden Nat-Termen:
-}

nullN :: Nat
nullN = Z

einsN :: Nat
einsN = S Z

zweiN :: Nat
zweiN = S $ S Z

dreiN :: Nat
dreiN = S $ S $ S Z

int2nat :: Int -> Nat
int2nat n = foldr (\_ acc -> S acc) Z [1..n]

nat2int :: Nat -> Int
nat2int = foldNat 0 (+1)

{-
a)
Implementieren Sie eine Haskell-Funktion
foldNat :: b -> (b -> b) -> Nat -> b,
die als erstes Argument eine Interpretation des Z-Konstruktors und
als zweites Argument eine Interpretation des S-Konstruktors nimmt und
eine Funktion zurückgibt, die einen Nat-Term unter dieser Interpretation
auswertet.
Die Faltung foldNat soll also alle Vorkommen des Z-Konstruktors durch das erste
Argument und alle Vorkommen des S-Konstruktors durch das zweite Argument
ersetzen.

Beispielauswertungen:

foldNat f g (Z) ~> f
foldNat f g (S $ Z) ~> g f
foldNat f g (S $ S Z) ~> g $ g f
...

-}

foldNat :: b -> (b -> b) -> Nat -> b
foldNat f g Z = f
foldNat f g (S x) = g $ foldNat f g x

{-
b)
Implementieren Sie eine Haskell-Funktion
add :: Nat -> Nat -> Nat,
die zwei natürliche Zahlen addiert.
Nutzen Sie hierfür sinnvoll die Faltung foldNat.


3 + 4 = 7

S $ S $ S $ Z 'add' S $ S $ S $ S $ Z = S $ S $ S $ S $ S $ S $ S $ Z 
-}

add :: Nat -> Nat -> Nat
add a b = foldNat a S b

{-
c)
Implementieren Sie eine Haskell-Funktion
nat2binär :: Nat -> [Bool],
die eine natürliche Zahl in die Binärzahldarstellung aus Übung 3
(Listen von Bools, LSB 0) übersetzt.
Nutzen Sie hierfür sinnvoll die Faltung foldNat.
-}

nat2bin :: Nat -> [Bool]
nat2bin = foldNat [False] f
  where
    f :: [Bool] -> [Bool]
    f [] = [True]
    f (x:xs)
      | x == False = True : xs
      | otherwise = False : f xs
{-
Aufgabe 6.2 - Maybe & Either
-}

{-
a)
Implementieren Sie eine Haskell-Funktion
predNat :: Nat -> Maybe Nat,
die den Vorgägner einer natürlichen Zahl berechnet, sofern dieser existiert.
Wenn kein Vorgänger existiert soll Nothing zurückgegeben werden.

Anmerkung: predNat darf hier gerne mittels Pattern Matching implementiert werden.
Außer Sie wollen die *-Aufgabe vorziehen.
-}

predNat :: Nat -> Maybe Nat
predNat Z = Nothing
predNat (S xs) = Just xs

{-
b)
Implementieren Sie eine Haskell-Funktion
minus :: Nat -> Nat -> Maybe Nat,
die zwei natürliche Zahlen subtrahiert. Sollte das zweite Argument größer
als das erste sein, so soll Nothing zurückgegeben werden.
Nutzen Sie hierfür sinnvoll die Faltung foldNat.

Tipp:
Guards oder ähnliches sind nicht notwendig, der "Fehlerfall" kann durch
foldNat korrekt gehandhabt werden.

S (S ( S Z)) - S (S Z) => S Z
-}

minus :: Nat -> Nat -> Maybe Nat
minus a = foldNat (Just a) (maybe Nothing predNat')
{-
minus x Z = Just x
minus Z (S x) = Nothing
minus (S x) (S y) = minus x y

minus a b = foldNat (Just a) predNatMaybe b
  where
    predNatMaybe :: Maybe Nat -> Maybe Nat
    predNatMaybe Nothing = Nothing
    predNatMaybe (Just x) = predNat x
-}


{-
c)
Implementieren Sie eine Haskell-Funktion
elemAtIndex :: Int -> [a] -> Either String a,
die als erstes Argument einen Index und als zweites Argument eine Liste nimmt
und das Element der Liste an dem Index zurückgibt.

Hierbei können Fehler auftreten, die durch die Verwendung von (Either String)
abgefangen und durch sinnvolle Fehlermeldungen beschrieben werden sollen.
Sollte der Index größer als die Länge der Liste sein, so soll der String
"Fehler: Index zu groß" im Either zurückgegeben werden.
Sollte der Index negativ sein, so soll der String
"Fehler: Negativer Index" im Either zurückgegeben werden.
-}

elemAtIndex :: Int -> [a] -> Either String a
elemAtIndex _ []     = Left "Fehler: Index zu gross"
elemAtIndex 0 (x:xs) = Right x
elemAtIndex n (x:xs)
  | n < 0 = Left "Fehler: Negativer Index"
  | otherwise = elemAtIndex (n-1) xs
  
{-
elemAtIndex n xs =
  fst $ foldl f (Left "Fehler: Index zu gross", n) xs
  where
    f :: (Either String a, Int) -> a -> (Either String a, Int)
    f (eth, n) x
      | n < 0 = (Left "Fehler: Index ist zu klein", n-1)
      | n == 0 = (either (\l -> Right x) (\r -> Right r) $ eth, 0)
      | otherwise = (eth, n-1)
Aufgabe 6.3 - Binäre Bäume

In der Vorlesung wurde auf den Doberkat-Folien S.224ff ein Datentyp für
binäre Bäume vorgestellt und genutzt.
Wir nennen den Datentyp entgegen der Folien hier BinBaum und die Konstruktoren
BinLeer und BinKnoten.

Des weiteren sei der Datentyp Baum für Bäume beliebigen Ausgrads gegeben.
-}

data BinBaum a = BinLeer | BinKnoten a (BinBaum a) (BinBaum a) deriving Show

data Baum a = Leer | Knoten a [Baum a] deriving Show

binBaum :: BinBaum Int
binBaum = BinKnoten 4
  (BinKnoten 2 (BinKnoten 1 (BinLeer) (BinLeer)) (BinKnoten 3 (BinLeer) (BinLeer)))
  (BinKnoten 6 (BinKnoten 5 (BinLeer) (BinLeer)) (BinKnoten 7 (BinLeer) (BinLeer)))

{-
a)
Implementieren Sie eine Haskell-Funktion
convert :: BinBaum a -> Baum a,
die einen binären Baum in einen Baum beliebigen Ausgrads übersetzt.
-}

convert :: BinBaum a -> Baum a
convert = preFold Leer (\x l r -> Knoten x [l, r])

{-
b)
Implementieren Sie eine Haskell-Funktion
preFold :: b -> (a -> b -> b -> b) -> BinBaum a -> b,
die als erstes Argument eine Interpretation des BinLeer-Konstruktors und als
zweites Argumgent eine Interpretation des BinKnoten-Konstruktors nimmt und
eine Funktion zurückgibt, die einen BinBaum-Term unter dieser Interpretation
auswertet.

Beispielauswertung:
preFold f g BinLeer ~> f
preFold f g (BinKnoten x BinLeer BinLeer) ~> g x f f
...
-}

preFold :: b -> (a -> b -> b -> b) -> BinBaum a -> b
preFold f g BinLeer = f
preFold f g (BinKnoten x as bs) = g x (preFold f g as) (preFold f g bs)

{-
c)
Implementieren Sie eine Haskell-Funktion
preorder :: BinBaum a -> [a],
die einen binären Baum in preorder-Reihenfolge druchläuft und die Knotenelemente
in eine Liste schreibt.
Nutzen Sie hierfür sinnvoll die Faltung preFold.

Beispielaufruf:
preorder binBaum ~>
[4,2,1,3,6,5,7]
-}

preorder :: BinBaum a -> [a]
preorder = preFold [] (\x l r -> x : l ++ r)

{-
d)
Implementieren Sie eine Haskell-Funktion
knotenSumme :: Num a => BinBaum a -> a,
die die Summe aller Knoten eines binären Baums berechnet.
Nutzen Sie hierfür sinnvoll die Faltung preFold.

Beispielaufruf:
knotenSumme binBaum ~>
28
-}

knotenSumme :: Num a => BinBaum a -> a
knotenSumme = preFold 0 (\x l r -> x + l + r)
 
{-
Aufgabe 6.* - Destruktor
-}

{-
Implementieren Sie die Funktion
predNat :: Nat -> Maybe Nat
aus Aufgabe 6.2 a) erneut, nutzen Sie aber dieses Mal
foldNat auf sinnvolle Weise und nennen Sie die Funktion
predNat'.
-}

predNat' :: Nat -> Maybe Nat
predNat' = foldNat Nothing (maybe (Just Z) (Just . S))
