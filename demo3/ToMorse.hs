module ToMorse where

import           Data.Char
import           Data.List.Split

transToMorseCCode :: String -> String
transToMorseCCode str = unlines $ zipWith als [0..] $ map toBits codes
  where codes = strToMorse str
        toBits it' = let it = take 7 $ it' ++ repeat '0'
                     in "0b1" ++ it
        als i bits = "beep_buf["++show i ++ "] = " ++ bits ++ ";"



strToMorse :: String -> [[Char]]
strToMorse str' = chunksOf 7 ditas
  where str = words str'
        ditas = concat $ map (\x -> wordToMorse x ++ "0000") str

wordToMorse :: String -> [Char]
wordToMorse str = concat $ map (\x -> toMorse x ++ "00") str

toMorse :: Char -> [Char]
toMorse = concat . map toBit . toMorseStep . toUpper
  where toBit True  = "01"
        toBit False = "0111"
        toMorseStep 'A' = [True,False]
        toMorseStep 'B' = [False,True,True]
        toMorseStep 'C' = [False,True,False,True]
        toMorseStep 'D' = [False,True,True]
        toMorseStep 'E' = [True]
        toMorseStep 'F' = [True,True,False,True]
        toMorseStep 'G' = [False,False,True]
        toMorseStep 'H' = [True,True,True,True]
        toMorseStep 'I' = [True,True]
        toMorseStep 'J' = [True,False,False]
        toMorseStep 'K' = [False,True,False]
        toMorseStep 'L' = [True,False,True,True]
        toMorseStep 'M' = [False,False]
        toMorseStep 'N' = [False,True]
        toMorseStep 'O' = [False,False,False]
        toMorseStep 'P' = [True,False,False,True]
        toMorseStep 'Q' = [False,False,True,False]
        toMorseStep 'R' = [True,False,True]
        toMorseStep 'S' = [True,True,True]
        toMorseStep 'T' = [False]
        toMorseStep 'U' = [True,True,False]
        toMorseStep 'V' = [True,True,True,False]
        toMorseStep 'W' = [True,False,False]
        toMorseStep 'X' = [False,True,True,False]
        toMorseStep 'Y' = [False,True,False,False]
        toMorseStep 'Z' = [False,False,True,True]
        toMorseStep '0' = [False,False,False,False,False]
        toMorseStep '1' = [True,False,False,False,False]
        toMorseStep '2' = [True,True,False,False,False]
        toMorseStep '3' = [True,True,True,False,False]
        toMorseStep '4' = [True,True,True,True,False]
        toMorseStep '5' = [True,True,True,True,True]
        toMorseStep '6' = [False,True,True,True,True]
        toMorseStep '7' = [False,False,True,True,True]
        toMorseStep '8' = [False,False,False,True,True]
        toMorseStep '9' = [False,False,False,False,True]
        toMorseStep _   = []
