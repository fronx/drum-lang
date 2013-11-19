module WhitespaceParser where

import Text.ParserCombinators.Parsec

data Tree a = Node a | Branches [Tree a] deriving Show

whitespaceFile :: GenParser Char Int A
whitespaceFile = do
  result <- as
  eof
  return

as :: GenParser Char Int A
as = do
  a <|> b

ws n = concat $ replicate n "  "

a :: GenParser Char Int A
a = do
  indentation <- ws getState
  return string $ indentation ++ "a\n" ++ (ws (getState + 1))

b :: GenParser Char Int A
b = do
  indentation <- ws getState
  return string $ indentation ++ "b\n"

parseA :: String -> Either ParseError A
parseA input = parse whitespaceFile 0 input


main = do
  print (parseA
"a
  a
    a")
