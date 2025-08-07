# jvgate - Logic Gate Simulator



<img width="1197" height="851" alt="Screenshot From 2025-08-05 20-17-59" src="https://github.com/user-attachments/assets/c37feeca-2c95-46fa-bd11-2ab67e6d01c5" />



***Still in early development, bugs are expected, issues and pr's are warmly welcomed***

jvgate is a simple digital logic simulator written in c with minimal dependencies, has a simple gui written with raylib that lets the user move elements in the workspace.

## Code Structure

The program is build to be easily extensible, using a `struct` with a `LogicElement` and `GraphicElement` for each circuit `Element`, that further can define it's own Logic/Display 
procedures using the `varargs` function pointers that these two inside structures have.
The outside wrapping `struct` can as well hold other potential metadata depending on the circuit element.


# How To Run It ?

**[Raylib](https://github.com/raysan5/raylib) needs to be locally installed** 

```sh
git clone git@github.com:Jarvx200/jvgate.git #Clone the repo
cd ./jvgate
git submodule init
git submodule update #For raygui init
make
```


## TODO:

- [ ] Macros for consistent enum initialization
- [ ] Serialization/Deserialization for saving and loading circuits 
- [x] Parsing the output of an element to multiple other elements 
- [ ] Abstract user-defined component with an inner logic-gate graph
- [ ] Slow down/ Speed up topsort for aditional rendering 
- [ ] GUI Polish, Multiple collapsable windows to see the data in real time
- [ ] Documentation with gitbook

https://github.com/user-attachments/assets/5c37bf9a-afc8-4fe3-a173-ba4c90f990ae


