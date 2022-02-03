## Initial setup
- Download the repo
- Check that the C++ integration works (C++ build was tested on windows and Visual Studio 2019 (use VS cmd prompt 'x86_x64')):
    - Install python 3.x and latest version of pip 
    - Install scons building manager: `pip install scons`
    - `cd client/godot-cpp` (in VS cmd prompt 'x86_x64')
    - `scons generate_bindings=yes bits=64` - will build the godot-cpp source files with VS compiler (takes a lot of time to finish)
    - `cd client/godot` (in VS cmd prompt 'x86_x64')
    - `scons platform=windows` (need to check the tutorials to get the feeling of the structure for C++ integration)
    - Now when open a project in Godot you should see floating godot logo and a phrase getted printed to the console
- After that you can delete everything inside `client/godot/game` folder and start working on your project (we can attach C++ integration when needed)


## Some helpful tutorials:
- https://docs.godotengine.org/en/stable/tutorials/scripting/gdnative/gdnative_cpp_example.html - (godot docs) C++ intergration
- https://youtu.be/XPcSfXsoArQ - tutorial for this C++ intergration setup
- https://docs.godotengine.org/en/3.4/tutorials/export/exporting_projects.html - (godot docs) exporting the projects 