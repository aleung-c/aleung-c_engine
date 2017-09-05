# aleung-c_engine
Basic high-level OpenGL engine

This C++ game engine is an overlay of classes and functions that can be put in code to
create simple 3d openGL programs. It will handle openGL related calls and
memory handling, to allow the gameplay side to just call some lines of code to
put 3d models and 2d pictures on the screen.

It comes in the form a .a static library, that must be compiled on its own and then linked
to the game program.

This engine revolves around a simple idea: to put several 3d models into an
openGL program, it requires quite a lot of lines.

In this engine, those lines are enough:
```
  GameEngineController    GameEngine;

  // this initialize the engine and a window
  GameEngine.Init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
```

```
  GameObject *Character = new GameObject("Character", "./ressources/Model.obj");

  Character->Transform.Position = glm::vec3(3.0, 0.0, 0.0);
```

```
  // in your main game loop;
  while (!glfwWindowShouldClose(GameEngine.Window))
  {
    GameEngine.Draw();
  }
```

If there is a texture with the same name as "Model.obj" but with bmp extension,
it will load that texture and put it on the model.

This means that if you have several textured models, and you want to create
a simple game application, you can do it more simply than by recreating all your openGL pipeline.
Note however that this is a code engine, meaning that everything you do is through code, and not
graphical interfaces.

Among other things, the engine implements the following functionalities:
- 3d obj model.
- simple bmp textures.
- UI pictures displayed on screen.
- Dynamic text on screen.
- Camera displacement.
- Basic morph targeting animations.

It uses OpenGL 4, GLFW, Freetype2 and GLM.

For more details on how to use this, I redirect you to the [USAGE_README](./USAGE_README).

It could be more than that but the idea was to make simple games for educational purpose, and some advanced
functionalities require just too much time to implement. Note that the installation is far from optimized
and "key in hand", and is likely to be only compatible to MacOS systems.

I used this engine to create the [42run](https://github.com/aleung-c/42run) and [ParticleSystem](https://github.com/aleung-c/ParticleSystem) projects.

Examples Screens:

