module JSON where

import Control.Applicative
import Data.Char
import Numeric

data JSONValue
  = JSONNull
  | JSONBool Bool
  | JSONNumber Double
  | JSONString String
  | JSONArray [JSONValue]
  | JSONObject [(String, JSONValue)] deriving (Show, Eq)

newtype Parser a = Parser
  { runParser:: String -> Maybe (a, String)
  }

instance Functor Parser where
  fmap f (Parser p) = Parser $ \input -> do
                             (x, input') <- p input
                             return $ (f x, input')

instance Applicative Parser where
  pure p = Parser $ \input -> Just $ (p, input)
  (Parser p1) <*> (Parser p2) = Parser $ \input -> do
                             (f, input') <- p1 input
                             (x, input'') <- p2 input'
                             return (f x, input'')
    
instance Alternative Parser where
  empty = Parser $ \input -> Nothing
  (Parser p1) <|> (Parser p2) = Parser $ \input -> (p1 input) <|> (p2 input)

charP :: Char -> Parser Char
charP c = parseIf (==c)

parseIf :: (Char -> Bool) -> Parser Char
parseIf g = Parser $ \input -> f input
  where
    f (x:xs)
      | g x = Just (x, xs)
      | otherwise = Nothing
    f _ = Nothing

stringP :: String -> Parser String
stringP = sequenceA . map charP

spanP :: (Char -> Bool) -> Parser String
spanP = many . parseIf

escapeUnicode :: Parser Char
escapeUnicode = chr . fst . head . readHex <$> sequenceA (replicate 4 (parseIf isHexDigit))

escapeChar :: Parser Char
escapeChar = ('"' <$ stringP "\\\"") <|>
             ('\\' <$ stringP "\\\\") <|>
             ('/' <$ stringP "\\/") <|>
             ('\b' <$ stringP "\\b") <|>
             ('\f' <$ stringP "\\f") <|>
             ('\n' <$ stringP "\\n") <|>
             ('\r' <$ stringP "\\r") <|>
             ('\t' <$ stringP "\\t") <|>
             (stringP "\\u" *> escapeUnicode)

normalChar :: Parser Char
normalChar = parseIf ((&&) <$> (/= '"') <*> (/= '\\'))

stringLiteral :: Parser String
stringLiteral = charP '"' *> many (normalChar <|> escapeChar) <* charP '"'

ws :: Parser String
ws = spanP isSpace <|> pure []

sepBy :: Parser a -> Parser b -> Parser [b]
sepBy sep element = (:) <$> element <*> many (sep *> element) <|> pure []

doubleLiteral :: Parser Double
doubleLiteral =
  doubleFromParts
    <$> (minus <|> pure 1)
    <*> (read <$> digits)
    <*> ((read <$> (('0':) <$> ((:) <$> charP '.' <*> digits))) <|> pure 0)
    <*> ((e *> ((*) <$> (plus <|> minus <|> pure 1) <*> (read <$> digits))) <|> pure 0)
  where
    digits = some $ parseIf isDigit
    minus = (-1) <$ charP '-'
    plus = 1 <$ charP '+'
    e = charP 'e' <|> charP 'E'

doubleFromParts :: Integer -> Integer -> Double -> Integer -> Double
doubleFromParts sign int dec expo =
  fromIntegral sign * (fromIntegral int + dec) * (10 ^^ expo)

jsonNull :: Parser JSONValue
jsonNull = (\_ -> JSONNull) <$> stringP "null"

jsonBool :: Parser JSONValue
jsonBool = f <$> (stringP "true" <|> stringP "false")
  where
    f "true" = JSONBool True
    f "false" = JSONBool False

jsonNumber :: Parser JSONValue
jsonNumber = JSONNumber <$> doubleLiteral

jsonString :: Parser JSONValue
jsonString = JSONString <$> stringLiteral

jsonArray :: Parser JSONValue
jsonArray =  JSONArray <$> (charP '[' *> ws *> elements <* ws <* charP ']')
  where elements = sepBy (ws *> charP ',' <* ws) jsonValue

jsonObject :: Parser JSONValue
jsonObject = JSONObject <$> (charP '{' *> ws *> sepBy (ws *> charP ',' <* ws) pair <* ws <* charP '}')
  where pair = (\key _ value -> (key, value)) <$>
                                stringLiteral <*> (ws *> charP ':' <* ws) <*>
                                jsonValue

jsonValue :: Parser JSONValue
jsonValue = jsonNull <|> jsonBool <|> jsonNumber <|> jsonString <|> jsonArray <|> jsonObject
