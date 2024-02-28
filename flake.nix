{
  inputs = {
    hyprland.url = "github:hyprwm/Hyprland/tags/v0.36.0";
    nix-filter.url = "github:numtide/nix-filter";
  };

  outputs = {
    self,
    hyprland,
    nix-filter,
    ...
  }: let
    inherit (hyprland.inputs) nixpkgs;
    forHyprlandSystems = fn: nixpkgs.lib.genAttrs (builtins.attrNames hyprland.packages) (system: fn system nixpkgs.legacyPackages.${system});
  in {
    packages = forHyprlandSystems (system: pkgs: let
      hyprlandPackage = hyprland.packages.${system}.hyprland;
    in rec {
      Hypr-DarkWindow = pkgs.gcc13Stdenv.mkDerivation {
        pname = "Hypr-DarkWindow";
        version = "0.1";
        src = nix-filter.lib {
          root = ./.;
          include = [
            "src"
            ./Makefile
          ];
        };

        nativeBuildInputs = with pkgs; [ pkg-config ];
        buildInputs = [hyprlandPackage.dev] ++ hyprlandPackage.buildInputs;

        installPhase = ''
          mkdir -p $out/lib
          install ./hyprfocus.so $out/lib/libhyprfocus.so
        '';

        meta = with pkgs.lib; {
          homepage = "https://github.com/micha4w/Hypr-DarkWindow";
          description = "Invert the colors of specific Windows";
          license = licenses.bsd3;
          platforms = platforms.linux;
        };
      };

      default = Hypr-DarkWindow;
    });

    devShells = forHyprlandSystems (system: pkgs: {
      default = pkgs.mkShell {
        name = "Hypr-DarkWindow";

        nativeBuildInputs = with pkgs; [
          clang-tools_16
        ];

        inputsFrom = [self.packages.${system}.Hypr-DarkWindow];
      };
    });
  };
}
