# Plat

![null](img/main.png)



## 1. Overview

__Plat is a minecraft clone project using unreal engine.__

Plat focus on how to handle a huge world to make players feel like they belong to the infinite world. Also, It has other features to satisfy seems like minecraft. These are describe `features` section.



## 2. Features

- The world consists of blocks you can interact with.
- Support item combination recipes.
- Player can equip some items and it makes little changes when you interact with specific blocks.



## 3. Perlin noise

Perlin noise is the key algorithm for creating terrain. I have an aim to achieve that code the algorithm myself. You can find source code in `WorldMap`.

The noise function required 2 additional parameters, which  `noiseScale` and `octave` as compared in-build function. Additionally you should appropriate to set `threshold`  value for good result.

This table show meaning each of variables.

![TableGG2](https://user-images.githubusercontent.com/30077320/123218419-a930cd80-d506-11eb-8d7f-970798da28f6.png)

*pseudo-code*

```c++
float density = MyPerlinNoise(x, y, OCTAVES, NOISESCALE);
				
if (density < THRESHOLD) {
// Do something..
}
```

> #### reference
>
> > * <https://adrianb.io/2014/08/09/perlinnoise.html>
> > * <https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/>
> > * <https://www.youtube.com/watch?v=6-0UaeJBumA>
> > * https://www.youtube.com/watch?v=MJ3bvCkHJtE&t=4s

## 4. Generated world example

![GG2](https://user-images.githubusercontent.com/30077320/123113029-2f9ece00-d479-11eb-9182-1e35a108be0d.png)

![GG3](https://user-images.githubusercontent.com/30077320/123113036-31689180-d479-11eb-9620-ff53a10f1140.png)

![GG4](https://user-images.githubusercontent.com/30077320/123113043-3299be80-d479-11eb-9948-94ca55bce62f.png)

![GG5](https://user-images.githubusercontent.com/30077320/123113049-33325500-d479-11eb-86f1-b39463376040.png)

![GG6](https://user-images.githubusercontent.com/30077320/123113053-34638200-d479-11eb-8e21-b4b34653eba1.png)

![GG7](https://user-images.githubusercontent.com/30077320/123113008-2ca3dd80-d479-11eb-824d-587f38ecdd12.png)

![GG8](https://user-images.githubusercontent.com/30077320/123113108-3f1e1700-d479-11eb-8999-932c49f6cafa.png)

- _in-build perlin noise generation below._

![in-build noise](https://user-images.githubusercontent.com/30077320/123218853-2e1be700-d507-11eb-8c99-da8b83ffc0b9.png)


## Mesh source

steve(character):https://sketchfab.com/3d-models/the-perfect-steve-rigged-0cffc39bdab04551bde4f8cdfbc52eca

weapons:https://www.turbosquid.com/3d-models/free-sword-shovel-axe-3d-model/1006797











