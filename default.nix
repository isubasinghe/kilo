let 
  pkgs = import<nixpkgs> {};
in 
pkgs.stdenv.mkDerivation {
  pname = "kilo";
  version = "0.0.1";

  src = ./.;

  nativeBuildInputs = [];

  buildInputs = [pkgs.gcc (pkgs.haskellPackages.ghcWithPackages (ps: [ ps.shake ]))];

  buildPhase = '' 
    ./build.sh  
  '';

  installPhase = ''
    mkdir $out
    cp -r _build/* $out
  '';
}


