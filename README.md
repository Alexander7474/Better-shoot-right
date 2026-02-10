# Better-shoot-right

Shooter 2D / rpg 

## Compile
```
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++
cmake --build . --parallel
```

### Debug flag

```
-DCONTACT_DEBUG
-DITEM_DEBUG
-DPHYSIC_DEBUG
-DIMGUI_DEBUG
```
