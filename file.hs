import Development.Shake
import Development.Shake.Command
import Development.Shake.FilePath
import Development.Shake.Util

main :: IO ()
main = shakeArgs shakeOptions {shakeFiles = "_build"} $ do
  want ["all"]

  phony "clean" $ do
    putInfo "Cleaning files in _build"
    removeFilesAfter "_build" ["//*"]

  phony "_build/examples" $ do
    examples <- getDirectoryFiles "" ["examples//*.c"]
    need examples
    let bins = ["_build/bin" </> dropExtension e | e <- examples]
    need ["_build/lib/libump.a"]
    need bins

  "_build/lib//*.a" %> \out -> do
    cs <- getDirectoryFiles "" ["src//*.c"]
    let os = ["_build" </> c -<.> "o" | c <- cs]
    need os
    cmd_ "ar rcs" [out] os

  "_build/bin//*" %> \out -> do
    need ["_build/lib/libump.a"]
    let c = dropDirectory1 (dropDirectory1 out) <.> "c"
    cmd_ "gcc -O3" [c] "-o" [out] "-I./include -L./_build/lib -lump"

  "_build//*.o" %> \out -> do
    let c = dropDirectory1 $ out -<.> "c"
    let m = out -<.> "m"
    cmd_ "gcc -O3 -I./include -c" [c] "-o" [out] "-MMD -MF" [m]
    neededMakefileDependencies m

  "_build/include//*.h" %> \out -> do
    let inc = dropDirectory1 out
    copyFile' inc out

  phony "all" $ do
    need ["_build/examples"]
    hdrs <- getDirectoryFiles "" ["include//*.h"]
    let bhdrs = ["_build" </> h | h <- hdrs]
    need bhdrs
