# il2cpp-cpp-class-generator
A program that generates C++ classes for Il2cpp games to make modding them easier, automatically generating most of the boilerplate code necessary for calling in game functions, modifying fields etc.

# To-do list
- [x] Make a function to detect the Unity version of an Il2cpp game
- [ ] Make a file type specific class that loads an executable, gets the file information and 
- [x] Add basic global-metadata.dat related structs into the project 
- [x] Create a function to get the Il2cppMetadataRegistration pointer (necessary for generating C++ classes from Il2cpp data)
- [ ] Parse metadata information (such as classes, their fields, methods etc.) into our own organized structs/vectors
- [ ] Put each class into their own folder
- [ ] Generate boilerplate code that will initialize some of the Il2cpp backend functions
- [ ] Generate code to initialize all of the game classes
- [ ] Generate each class into their own file
- [ ] ???
- [ ] Make everything in this list work across multiple platforms

