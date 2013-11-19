module DrmParser where

import DrmTypes
import Text.ParserCombinators.Parsec

drmFile :: GenParser Char st [DrmFunction]
drmFile = do
  result <- functions
  eof
  return result

functions :: GenParser Char st [DrmFunction]
functions = do
  functionStart
  spaces
  name <- functionName
  next <- remainingFunctions
  return (DrmFunction { name=name } : next)

functionStart :: GenParser Char st Char
functionStart = char '#'

functionName :: GenParser Char st String
functionName = many alphaNum

remainingFunctions :: GenParser Char st [DrmFunction]
remainingFunctions =
      (skipMany1 space >> functions)
  <|> (return [])

eol :: GenParser Char st Char
eol = char '\n'

parseDrm :: String -> Either ParseError [DrmFunction]
parseDrm input = parse drmFile "" input

main = do
  print "done."
