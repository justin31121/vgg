module Uebung5 where

{-
Übung zur Vorlesung
Funktionale Programmierung
Sommersemester 2022
Übungsblatt 5
-}

{-
Block: 1
Ausgabe: 13.05.2022
Abgabe: keine
-}


{-
Aufgabe 5.1 - Graphen in Haskell
-}

{-
In den Doberkat-Folien 110-119 wird das Beispiel "Transitive Hülle" erläutert.
Hier wird ein gerichteter Graph als Relation dargestellt und die Relation selber
als Funktion, die jedem Knoten seine Adjazenzliste zuordnet.

Alternativ zur funktionalen Darstellung können wir eine Relation auch als Liste von
Paaren auffassen.
Das heißt ein gerichteter Graph kann als Liste von Tupeln beschrieben werden, deren
zweite Projektion der Adjazenzliste des Knotens aus der ersten Projektion entspricht.
-}

type Graph = [(Int,[Int])]

{-
Implementieren Sie den Algorithmus zur Berechnung der transitiven Hülle von
den Folien einmal selbst, allerdings für die zuvor beschriebene Darstellung gerichteter
Graphen.

Eine wichtige Änderung zu der Version auf den Folien soll sein, dass die Funktion, die die
transitive Hülle berechnet, den Typ
tHull :: Graph -> Graph
haben soll.
-}

{-
Beim Aufruf uInsert x (y:ys) sehen wir nach, ob x == y gilt. Falls ja,
lassen wir die Liste in Ruhe, falls nicht, lassen wir y unangetastet und
versuchen, x in den Reste der Liste einzusetzen.
-}

uInsert :: (Eq t) => t -> [t] -> [t]
uInsert x [] = [x]
uInsert x (y:ys)
  | x==y = (y:ys)
  | otherwise = y : uInsert x ys


{-
Der Operator +++ hat dieselben Eigenschaften wir der Operator ++. Die
Funktion uInsert achtet aber darauf, daß ein Element nur dann in eine Liste
eingefügt wird, wenn es noch nicht vorhanden ist.
-}

infixr 5 +++
(+++) :: (Eq t) => [t] -> [t] -> [t]
(+++) xs ys = foldr uInsert xs ys


--Die Funktion knoten liefert uns alle Knoten des Graphen

knoten :: Graph -> [Int]
knoten graph = foldr (\(f, s) acc -> uInsert f $ foldr uInsert acc s) [] graph
--knoten = map fst
{-
Die Funktion defAdj übersetzt den Graphen, der eine Relation aus Knoten und Adjazenzlisten
ist in eine Funktion, die jedem Knoten des Graphen seine Adjazenzliste zuordnet.
-}

defAdj :: Graph -> Int -> [Int]
defAdj [] x = []
defAdj ((y, adj):ys) x
 | y==x = adj
 | otherwise = defAdj ys x

{-
Die Funktion upd (a, b) adj konstruiert eine neue Abbildung neuAdj, die
für alle Werte außer a und b genauso aussieht wie adj. Falls wir eine Kante
von a nach b haben, fügen wir die Nachbarn von b zu denen von a hinzu; so
entsteht die Liste neuAdj a; analog gehen wir bei b vor.
-}

upd :: (Int, Int) -> (Int -> [Int]) -> Int -> [Int]
upd (a, b) f = neuAdj
  where
    t = f a
    d = f b
    neuAdj x
      | x==a && elem b t = t +++ d
      | x==b && elem a d = d +++ t
      | otherwise = f x
  
{-
Für eine Liste ps von Paaren propagiert die Funktion upd1 die Erweiterungen
durch die Relation.
-}

upd1 :: (Int -> [Int]) -> [(Int, Int)] -> Int -> [Int]
upd1 f ps= foldr (\next acc -> upd next acc) f ps
--upd1 f [] x = f x
--upd1 f (y:ys) x = upd y f x
{-
Die Funktion mkRel ist die Umkehrfunktion von defAdj, die aus der funktionalen
die relationale Darstellung des Typs Graph berechnet.
-}

mkRel :: [Int] -> (Int -> [Int]) -> Graph
mkRel xs f = foldr uInsert [] $ map (\x -> (x, f x)) xs


{-
Wir verschaffen uns die Knoten und die (funktionale) Adjazenzliste des Graphen,
erweitern diese Adjazenzliste mittels upd1 und verwandeln das Resultat zurück
in einen Graph.
-}

tHull :: Graph -> Graph
tHull graph = mkRel dieKnoten (upd1 dieAdj allePaare)
  where dieKnoten = knoten graph
        dieAdj = defAdj graph
        allePaare = [(x, y) | x <- dieKnoten, y <- dieKnoten, x/=y]

{-
Aufgabe 5.2 - Graphen übersetzen
-}

{-
a)
Gegeben sei der folgende Datentyp MyGraph für gerichtete, ungelabelte Graphen.
Der Konstruktor Nil entspricht dem leeren Graphen.
Der Konstruktor Adj nimmt einen Knoten, seine Adjazenzliste
und einen Graphen und konstruiert einen neuen Graphen.

Implementieren Sie zwei Funktionen

myGraphToGraph :: MyGraph -> Graph

und

graphToMyGraph :: Graph -> MyGraph,

für die gelten soll, dass

myGraphToGraph . graphToMyGraph == id

graphToMyGraph . myGraphToGraph == id.

Überlegen Sie sich sinnvolle Testfälle für diese Eigenschaft und notieren Sie eventuelle Probleme
und mögliche Lösungsansätze.
-}

data MyGraph = Nil | Adj Int [Int] MyGraph

myGraphToGraph :: MyGraph -> Graph
myGraphToGraph Nil = []
myGraphToGraph (Adj f s xs) = (f, s) : myGraphToGraph xs 

graphToMyGraph :: Graph -> MyGraph
graphToMyGraph [] = Nil
graphToMyGraph ((f, s):xs) = Adj f s $ graphToMyGraph xs

{-
b)
Ein sehr einfaches, inzwischen aber auch nicht mehr wirklich aktuelles Dateiformat für
Graphen ist das GML-Format (Graph Modelling Language).
https://de.wikipedia.org/wiki/Graph_Modelling_Language

Da mit yEd ein frei verfügbarer Graph-Editor existiert, mit dem Graphen in diesem Format
eingelesen und dann weiter verarbeitet werden können, soll in dieser Aufgabe ein
Haskell-Programm geschrieben werden, das einen Graphen in der Adjazenzlistenrepräsentation
vom zuvor eingeführten Typ Graph in einen String im GML-Format schreibt.
Das folgende einfache Beispiel demonstriert die Repräsentation in diesem GML-Format.
Auf eine formale Definition verzichten wir hier.
Rechts vom GML-Beispiel sind aber Bemerkungen als Kommentare in Großbuchstaben
zum Verständnis eingefügt.
-}

{-
graph [
  comment <String>
  directed 1                   {-1 FÜR GERICHTETE GRAPHEN, 0 FÜR UNGERICHTETE-}
  id 42                        {-KANTEN- UND KNOTENBEZEICHNERN ALS GANZE ZAHL-}
  label <String>               {-DAS LABEL MUSS EIN STRING SEIN-}
  node [                       {-ERST ALLE KNOTEN...-}
    id 1
    label <String>
  ]
  node [
    id 2
    label <String>
  ]
  node [
    id 3
    label <String>
  ]
  edge [                       {-DANN ALLE KANTEN.-}
    source 1
    target 2
    label <String>
  ]
  edge [
    source 2
    target 3
    label <String>
  ]
]
-}

{-
Implementieren Sie eine Funktion toGML :: MyGraph  -> String.
Sie dürfen vorraussetzen, dass MyGraph ein Typ für gerichtete Graphen ist.
Da unsere betrachteten Graphen nicht gelabelt sind, sollen Sie jedes Vorkommen von
<String> im Beispiel durch den leeren String "" ersetzen.
-}

beispiel :: MyGraph
beispiel = Adj 1 [2] $ Adj 2 [3] $ Adj 3 [] Nil 

beispiel' :: Graph
beispiel' = [(1,[2]), (2, [3]), (3,[])]

{-
Beispielaufruf:

putStrLn $ toGML beispiel ~>

graph [
  comment ""
  directed 1
  id 42
  label ""
  node [
    id 1
    label ""
  ]
  node [
    id 2
    label ""
  ]
  node [
    id 3
    label ""
  ]
  edge [
    source 1
    target 2
    label ""
  ]
  edge [
    source 2
    target 3
    label ""
  ]
]

Wir benötigen in diesem Fall putStrLn, um z.B. die Zeilenumbrüche im String interpretieren zu lassen.

toGML beispiel ~>

"graph [\n  comment \"\"\n  directed 1\n  id 42\n  label \"\"\n  node [\n    id 1\n    label \"\"\n  ]\n  node [\n    id 2\n    label \"\"\n  ]\n  node [\n    id 3\n    label \"\"\n  ]\n  edge [\n    source 1\n    target 2\n    label \"\"\n  ]\n  edge [\n    source 2\n    target 3\n    label \"\"\n  ]\n]"
-}

toGML :: MyGraph  -> String
toGML Nil = ""
toGML myGraph = "graph [\n  comment \"\"\n  directed 1\n  id 42\n  label \"\"\n" ++  nodeString  ++ edgeString ++"]"
  where graph = myGraphToGraph myGraph
        dieKnoten = knoten graph
        --nodeString = fst $ foldl (\(acc, pos) next -> (acc ++ "node [\n    id "++(show pos)++"\n    label \"\"\n  ]\n", pos+1)) ("", 1) dieKnoten
        nodeString = fst $ foldl g ("", 1) dieKnoten
        edgeString = foldr f "" graph

        g :: (String, Int) -> Int -> (String, Int)
        g (acc, pos) next =
          let fPos = (show next)
          in (acc ++ "node [\n    id "++(show pos)++"\n    label \""++fPos++"\"\n  ]\n", pos+1)

        f :: (Int, [Int]) -> String -> String
        f (f, s) acc =
          let fPos = show $ indexOf dieKnoten f 1
          in acc ++ (foldl (\acc next -> "edge [\n    source "++fPos++"\n    target "++(show $ indexOf dieKnoten next 1)++"\n    label \"\"\n  ]\n" ++ acc) "" s)
          

        indexOf :: [Int] -> Int -> Int -> Int
        indexOf [] _ n = 0
        indexOf (x:xs) y n
          | x==y = n
          | otherwise = indexOf xs y (n+1)

{-
Bonus-Aufgabe 5.* - Show
-}

{-
Machen Sie MyGraph zu einer Instanz der Typklasse Show.
Elemente vom Typen MyGraph sollen hierbei immer im GML-Format ausgegeben werden.

Beispielaufrufe:

beispiel' ~>

[(1,[2]),(2,[3]),(3,[])]


beispiel ~>

graph [
  comment ""
  directed 1
  id 42
  label ""
  node [
    id 1
    label ""
  ]
  node [
    id 2
    label ""
  ]
  node [
    id 3
    label ""
  ]
  edge [
    source 1
    target 2
    label ""
  ]
  edge [
    source 2
    target 3
    label ""
  ]
]
-}

instance Show MyGraph where
  show myGraph = toGML myGraph


writeGML :: String -> MyGraph -> IO ()
writeGML path graph = do
  writeFile path (show graph)
  
