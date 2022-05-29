
module Uebung7 where

import Control.Monad

{-
Übung zur Vorlesung
Funktionale Programmierung
Sommersemester 2022
Übungsblatt 7
-}

{-
Ausgabe: 27.05.2022
Abgabe: keine
-}

{-
Aufgabe 7.1 - Listenmonade
-}

{-
a)
Implementieren Sie die Funktion
map :: (a -> b) -> [a] -> [b]
aus dem Prelude jeweils mittels Listenkomprehension,
Do- und Bind-Notation.
Nennen Sie die Funktionen mapLK, mapDo und mapBind.
-}

mapLK :: (a -> b) -> [a] -> [b]
mapLK f xs = [f x | x<-xs]

mapDo :: (a -> b) -> [a] -> [b]
mapDo f xs = do
  y <- xs
  return $ f y

mapBind :: (a -> b) -> [a] -> [b]
mapBind f xs = xs >>= (\x -> return $ f x)

{-
b)
Übersetzen Sie die Liste
pyTriples :: [(Int, Int, Int)]
in die Do- und Bind-Notation.
Nennen Sie die Listen pyTriplesDo und pyTriplesBind.
-}

pyTriples :: [(Int, Int, Int)]
pyTriples = [(a,b,c) |c <- [0..], b <- [0.. c], a <- [0.. b], (a < b) && (b < c), (a^2 + b^2) == c^2]

pyTriplesDo :: [(Int, Int, Int)]
pyTriplesDo = do
  c <- [0..]
  b <- [0..c]
  a <- [0..b]
  guard (a*a+b*b==c*c && (a<b) && (b<c))
  return $ (a, b, c)
  
pyTriplesBind :: [(Int, Int, Int)]
pyTriplesBind =
  [0..] >>= \c ->
  [0..c] >>= \b ->
  [0..b] >>= \a ->
  if (a*a+b*b==c*c && (a<b) && (b<c)) then [(a, b, c)] else []
  
{-
Aufgabe 7.2 - Monadeninstanzen
-}

{-
Gegeben sei der Datentyp Baum für nicht leere binäre Bäume, ohne innere Knoten.
-}

data Baum a = Blatt a | Knoten (Baum a) (Baum a) deriving Show

{-
a)
Machen Sie Baum zu einer Instanz der Typklassen Functor, Applicative und Monad.
-}

instance Functor Baum where
  fmap f (Blatt x) = Blatt $ f x
  fmap f (Knoten x y) = Knoten (fmap f x) (fmap f y)

instance Applicative Baum where
  pure = Blatt

  (<*>) :: Baum (a -> b) -> Baum a -> Baum b
  (<*>) (Blatt f) (Blatt x) = Blatt $ f x
  (<*>) (Blatt f) (Knoten x y) = Knoten ((Blatt f) <*> x) ((Blatt f) <*> y)

instance Monad Baum where
  (>>=) :: Baum a -> (a -> Baum b) -> Baum b
  (>>=) (Blatt x) f = f x
  (>>=) (Knoten x y) f = Knoten (x >>= f) (y >>= f)

{-
b)
Implementieren Sie die Faltung
foldBaum :: (a -> b) -> (b -> b -> b) -> Baum a -> b
für den Datentyp Baum.

Anmerkung: Wenn in einer Aufgabenstellung gefordert wird die Faltung für einen
Datentyp zu implementieren, dann ist immer das Schema von Übungsblatt 6
gemeint, sofern explizit nichts anderes in der Aufgabenstellung steht.
-}

foldBaum :: (a -> b) -> (b -> b -> b) -> Baum a -> b
foldBaum blatt knoten (Blatt x) = blatt x
foldBaum blatt knoten (Knoten x y) =
  knoten (foldBaum blatt knoten x) (foldBaum blatt knoten y)


{-
c)
Auf den Doberkat-Folien 325 - 327 sind Eigenschaften/Gesetze aufgeführt, die von
Monadeninstanzen erfüllt werden müssen.
Überlegen Sie sich ein paar sinnvolle Tests, um diese Eigenschaften für
ihre Instanziierung zu überprüfen.

                 5
            3       8
         1    4   7   9
-}


baum :: Baum Int
baum = Knoten (Knoten (Blatt 1) (Blatt 4)) (Knoten (Blatt 7) (Blatt 9))

-- 1. Regel
-- return 3 >>= double
-- double 3
double :: Int -> Baum Int
double x = Knoten (Blatt x) (Blatt x)

-- 2. Regel
-- Blatt 3 >>= return
-- Blatt 3

-- 3. Regel
-- p >>= (\x -> (f x >>= g)
--                Blatt 1 >>= (\x -> (double x >>= double))
-- (p >>= (\x -> f x)) >>= g
--                (Blatt 1 >>= (\x -> double x)) >>= double


{-
Aufgabe 7.3 - Maybe & Either Monade
-}

{-
a)
Die Either-Monade kann als "optimierte" Maybe-Monade angesehen werden, wenn man im ersten
Typargument Fehlernachrichten abbildet.
So kann (Either String Float) bei der Implementierung von partiellen Funktionen im Gegensatz
zu (Maybe Float) in dem Sinne als Optimierung verstanden werden, dass
Left "Man darf nicht durch 0 teilen!"
Informationen über die nicht-definierte Eingabe enthalten kann. Im Gegensatz zu
Nothing.

Implementieren Sie die Funktionen
logarithmus :: Float -> Either String Float
quadratwurzel :: Float -> Either String Float
kehrwert :: Float -> Either String Float
noch einmal mit sinnvollen Fehlermeldungen.

Implementieren Sie eine Funktion
try :: (Show a, Show b) => Either a b -> String,
sodass Sie Either sinnvoll zum debuggen
nutzen könnten. Z.B.:
try (Left "Hier ging etwas schief!") ~> "FEHLER: Hier ging etwas schief!"
try (Right 5.5) ~> "5.5"

Implementieren Sie folgende Funktionen mittels (>>=):
f = logarithmus . kehrwert . quadratwurzel
g = quadratwurzel . logarithmus . kehrwert
(Die Komposition (.) ist hier natürlich nicht Typkorrekt, sondern soll nur "die Idee"
wiedergeben. Ansonsten bräuchte man ja auch kein (>>=) ;) ...) 
-}

logarithmus :: Float -> Either String Float
logarithmus x
  | x <= 0 = Left "ERROR: Eingabe kleiner gleich Null (log)"
  | otherwise = Right $ log x

quadratwurzel :: Float -> Either String Float
quadratwurzel x 
  | x < 0 = Left "ERROR: Eingabe kleiner Null (sqrt)"
  | otherwise = Right $ sqrt x

kehrwert :: Float -> Either String Float
kehrwert x
  | x == 0 = Left "ERROR: Eingabe ist Null (1/x)"
  | otherwise = Right $ 1/x

try :: (Show a, Show b) => Either a b -> String
try (Right s) = show s
try (Left s)  = "FEHLER: " ++ show s 

--

f :: Float -> Either String Float
f x = do
  k <- logarithmus x
  p <- kehrwert k
  quadratwurzel p

g :: Float -> Either String Float
g x = do
  k <- quadratwurzel x
  p <- logarithmus k
  kehrwert p

{-
b)
Implementieren Sie eine Funktion
foo :: [Maybe a] -> Maybe [a],
die Nothing zurückgibt, wenn die Argumentliste ein Nothing enthält und sonst
alle Elemente der Liste aus dem Maybe "auspackt" und diese Liste wieder in ein
Maybe "einpackt".

Beispielaufrufe:
foo [Just 1, Just 4, Nothing, Just 3] ~> Nothing
foo [Just 1, Just 4, Just 3] ~> Just [1,4,3]
foo [] ~> Just []

Nutzen Sie zur Implementierung die do-Notation auf sinnvolle Weise.
-}

isJust :: Maybe a -> Bool
isJust (Just _) = True
isJust Nothing = False

foo :: [Maybe Int] -> Maybe [Int]
foo xs = do
  allJust <- foldl (\(Just acc) next -> Just $ acc && isJust next) (Just True) xs
  guard (allJust) 
  return $ [ x | (Just x)<-xs]

{-
Aufgabe 7.* - Kartesisches Produkt
-}

{-
Implementieren Sie eine Funktion
cartesianProduct :: [[a]] -> [[a]],
die das kartesische Produkt aller Elementlisten der Argumentliste berechnet.
Anstatt von n-Tupeln sollen aber Listen verwendet werden, um die Elemente des
n-stelligen kartesischen Produkts einfach abbilden zu können.

Beispielaufrufe:
cartesianProduct [] ~> [[]]
cartesianProduct [[1,2,3]] ~> [[1],[2],[3]]

cartesianProduct [[1,2,3], [4,5,6]] ~>
[[1,4],[1,5],[1,6],[2,4],[2,5],[2,6],[3,4],[3,5],[3,6]]

cartesianProduct [[1,2], [3,4], [5,6]] ~>
[[1,3,5],[1,3,6],[1,4,5],[1,4,6],[2,3,5],[2,3,6],[2,4,5],[2,4,6]]

cartesianProduct [[1,2], [3,4], [5,6], []] ~>
[]

cartesianProduct [[1,2], [3,4], [5,6], [7]] ~>
[[1,3,5,7],[1,3,6,7],[1,4,5,7],[1,4,6,7],[2,3,5,7],[2,3,6,7],[2,4,5,7],[2,4,6,7]]


mapLK :: (a -> b) -> [a] -> [b]
mapLK f xs = [f x | x<-xs]

mapDo :: (a -> b) -> [a] -> [b]
mapDo f xs = do
  y <- xs
  return $ f y
-}

cartesianProduct' :: [[a]] -> [[a]]
cartesianProduct' [] = [[]]
cartesianProduct' (xs:xss) = [x:ys | x<-xs, ys<-yss]
  where yss = cartesianProduct' xss

cartesianProduct :: [[a]] -> [[a]]
cartesianProduct =
  foldr (\next acc -> next >>= (<$> acc) . (:)) [[]]

t :: [a] -> [[a]] -> [[a]]
t next acc = concat (map ((<$> acc) . (:))  next)
  where k = (<$> acc) . (:)

id' :: [[Int]] -> [[Int]]
id' = foldr t [[]]

{-

concat $ map ((<$> [[3], [4]]) . (:)) [1,2]

concat $ ((<$> [[3], [4]]) . (:)) 1 : ((<$> [[3], [4]]) . (:)) 2
~> concat $ [ [[1,3], [1,4]], [[2,3], [2,4]]]
~> [[1,3], [1,4], [2,3], [2,4]]

((<$> [[3], [4]]) . (:)) 1
~> (<$>) ((:) 1) [[3], [4]]
~> [[1,3], [1,4]]

((<$> [[3], [4]]) . (:)) 2
~> (<$>) ((:) 2) [[3], [4]]
~> [[2,3], [2,4]]

=====================================================
concat $ map ((<$> [[]]) . (:)) [3,4]

concat $ ((<$> [[]]) . (:)) 3 : ((<$> [[]]) . (:)) 4
~> concat $[ [[3]], [[4]] ]
~> [[3], [4]]

((<$> [[]]) . (:)) 3
~> (<$>) ((:) 3) [[]]
~> [[3]]

((<$> [[]]) . (:)) 4
~> (<$>) ((:) 4) [[]]
~> [[4]]

============================================================

foldr t [[]] [[1,2], [3,4]]

~> ([1,2] t ([3,4] t [[]]))
~> ([1,2] t [[3], [4]])
~> [[1,3],[1,4],[2,3],[2,4]]

-}

