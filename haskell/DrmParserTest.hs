module DrmParserTest where

import DrmParser
import Test.HUnit.Base

main do =
  DrmFunction { name="abc" } ~=? parseDrm("# abc")
