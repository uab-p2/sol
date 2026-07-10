# Clonación/descarga del proyecto SOL

Obtén todo el código fuente del proyecto SOL.

El [proyecto SOL](https://github.com/uab-p2/sol) está alojado
en github. La dirección del repositorio git es

```https://github.com/uab-p2/sol.git```

El objetivo es crear una carpeta en tu disco duro con todos los contenidos.
Puedes descargarlos de varias maneras:

## Opción 1: clonación con CLion

Clona el repositorio con CLion, introduce la dirección del respositorio
git y la carpeta donde quieres guardar el código.

{{ img("screenshot_clion_git_clone.png") }}

## Opción 2: clonación con Visual Studio

Introduce la dirección del respositorio git y
la carpeta donde quieres guardar el código.

{{ img("screenshot_vs_git_clone.png") }}

## Opción 3: clonación manual con git

Clona el repositorio con `git clone https://github.com/uab-p2/sol.git`.

 ```bash
 $ git clone https://github.com/uab-p2/sol.git
 Cloning into 'sol'...
 remote: Enumerating objects: 492, done.
 remote: Counting objects: 100% (472/472), done.
 remote: Compressing objects: 100% (235/235), done.
 remote: Total 492 (delta 202), reused 423 (delta 160), pack-reused 20 (from 1)
 Receiving objects: 100% (492/492), 6.09 MiB | 6.51 MiB/s, done.
 Resolving deltas: 100% (204/204), done.
 
 $ ls sol/
 doc  Makefile  quest  src  README.md  CMakeLists.txt
 ```

## Opción 4: Descarga manual del zip

!!! warning
Esta opción es menos recomendable que las otras alternativas.

Descarga y descomprime el [`.zip` del proyecto sol.](https://github.com/uab-p2/sol/archive/refs/heads/main.zip)

{{ img("screenshot_github_zip.png") }}
