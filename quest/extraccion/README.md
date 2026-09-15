---
tags: [en_ruta:7, session:7]
---
# Extracción

Ya casi hemos llegado a la estación SOL. No tenemos certeza de qué veremos al llegar,
pero seguro que necesitaremos recursos. Preparemos los medios por anticipado.  

{{ img_badge("puzzle.png") }}

{{ goals(
    "Combina y analiza composición y herencia.",
    "Crea jerarquías de clases.",
    "Gestiona jerarquías de clases mediante polimorfismo.",
) }}

## Naves mineras

Los siguientes {{ def("diagrama de clase", text="diagramas de clase") }}
muestran dos jerarquías: para naves y otra para asteroides.
Estos diagramas usan línea continua para las clases proporcionadas en `src/`,
puntos para las clases trabajadas en {{ quest_link("simulacion") }},
y línea discontinua para clases no proporcionadas.

=== "Naves"
    ```mermaid
    flowchart BT
        Transport --> Ship
        Cargo --> Ship
        Miner --> Ship
        LightTransport --> Transport

        MinerA --> Miner
        MinerB... --> Miner
        ...MinerZ --> Miner

        classDef dashed stroke-dasharray: 5 5
        classDef dotted stroke-dasharray: 2 2
        class Transport,LightTransport dotted
        class Miner,MinerA,MinerB...,...MinerZ dashed
    ```
=== "Asteroides"
    ```mermaid
    flowchart BT
        IceAsteroid --> Asteroid
        RockAsteroid --> Asteroid
        MetalAsteroid --> Asteroid
    ```

Te proponemos crear una jerarquía de naves de minería
partiendo de la clase `Miner` mostrada en el diagrama,
con estas restricciones:

Todas las naves de minería deben tener un 
{{ def("constructor por defecto", text="constructor por defecto") }} 
y un método `mine` que:

- Acepte como único parámetro una {{ def("referencia", text="referencia") }} 
  a cualquier tipo de asteroide.

- Devuelve objetos de tipo `Cargo` (la nave de carga con el material minado).  

- La cantidad de carga en el `Cargo` retornado no puede superar la cantidad
  minada del asteroide mediante su método `mine`.

Al final, deberemos poder implementar una función `describe_mining` 
como la siguiente y usarla con nuestra jerarquía.

:::compile_and_run solution highlight=((7,10),(24,24),(30,30))
:::

!!! questions

      * Crea la clase `Miner` y al menos 2 especializaciones, `MinerA` y `MinerB`
        en `miner.h|cpp` considerando todos los puntos anteriores.

      * ¿Qué métodos y atributos son accesibles para la variable `miner` de `describe_mining`?

      * ¿Qué cambiaría si la variable `miner` fuese tipo `Ship&` en lugar de `Miner&`
         en `describe_mining`?

      * ¿Hace falta cambiar algo para que `describe_mining` permita minar
         diferentemente cada tipo de asteroide?

{{ help_link("class_inheritance") }}

## Minado en dos tiempos

Para completar el paquete de funcionalidad, sólo falta crear la clase `Mine`.
Esta mina usará dos naves mineras de tipo `MinerA` y `MinerB` alternativamente
para trabajar los asteroides cercanos.

!!! questions

    * Implementa la clase `Mine` en `mine.h|cpp` considerando los siguientes 
      puntos, y usando las cabeceras dadas cuando se proporcionan

* Composición.<br/>
  La clase `Mine` usa {{ def("composición", text="composición") }} con la
  clase `Station` del quest anterior para gestionar las coordenadas `x,y,z` y
  el transporte de personal.

{{ snippet_box("Mine::m_station", declaration=True, frame=False, indent=1) }}

* Construcción.<br/>
  Al menos el siguiente {{ def("constructor", text="constructor") }} está
  disponible en `Mine`:

{{ snippet_box("Mine::Mine", declaration=True, frame=False, indent=1) }}

* Naves mineras.<br/>
  Esta mina, también por composición, contiene una nave de 
  cada especialización de `Miner` que hayas implementado antes, p.ej.: 

{{ snippet_box("Mine::m_miner_a", declaration=True, frame=False, indent=1) }}
{{ snippet_box("Mine::m_miner_b", declaration=True, frame=False, indent=1) }}

* Procesado.<br/>
  La clase mine ofrece el método `process` con la siguiente cabecera.
  Este método utiliza alternativamente las naves MinerA y MinerB hasta que 
  el asteroide se consume. Impleméntalo usando el método auxiliar `get_next_miner`
  que devuelve referencias alternativas a MinerA y MinerB.

{{ snippet_box("Mine::process", declaration=True, frame=False, indent=1) }}

{{ snippet_box("Mine::get_next_miner", declaration=True, frame=False, indent=1) }}

{{ help_links("class_composition", "std_vector") }}
