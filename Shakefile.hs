import Development.Shake
import Development.Shake.Command
import Development.Shake.FilePath
import Development.Shake.Util

main :: IO ()
main = shakeArgs shakeOptions {shakeFiles = "_build"} $ do
  want ["_build/kilo"]

  phony "clean" $ do
    putInfo "Cleaning files in _build"
    removeFilesAfter "_build" ["//*"]


  "_build//*.o" %> \out -> do 
    let c = dropDirectory1 out -<.> "c"
    putInfo c 
    let m = out -<.> "m"
    cmd_ "gcc -I./include -O3 -Wall -Wextra -pedantic -std=c11 -c" [c] "-o" [out] "-MMD -MF" [m]
    needMakefileDependencies m

  "_build/tests/main" <.> exe %> \out -> do 
    cs <- getDirectoryFiles "" ["tests//*.c", "src//*.c"]
    let os = ["_build" </> c -<.> "o" | c <- cs]
    need os
    cmd_ "gcc -I./include -Wall -Wextra -pedantic -std=c11 -O3" os "-o" [out]

  phony "test" $ do 
    need ["_build/tests/main" <.> exe]
    command_ [EchoStdout True, EchoStderr True] ("_build/tests/main" <.> exe) []
    
  "_build/kilo" %> \out -> do 
    cs <- getDirectoryFiles "" ["src//*.c"]
    need ("main.c":cs)
    let os = ["_build" </> c -<.> "o" | c <- cs]
    need os
    cmd_ "gcc -I./include -Wall -Wextra -pedantic -std=c11 -O3 main.c" os "-o" [out]
  
  phony "run" $ do 
    cmd_ "_build/kilo"
