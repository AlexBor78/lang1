# shell.nix
{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "lang1-dev";
  
  buildInputs = with pkgs; [
    clang
    cmake
    gnumake
    ninja
    clang-tools
    doxygen 
  ];
}
