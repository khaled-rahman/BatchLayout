## File conversion

Gephi can work with .gml input file. So, it will open all possible options that Gephi provides if we just convert .mtx file to .gml file. User can do this using following command:

```
python mtx2gml.py 3elt_dual.mtx
```

When ForceAtlas2 of Gephi produces a layout in .gml format, that also needs to be converted which should be equivalent to output of BatchLayout. User can do this using following command:
```
python gml2txt.py 3elt_dual.forceatlas2_500.gml 
```

User will also need to convert output of OpenOrd to same format of BatchLayout. It can be done by following:
```
python ico2txt.py 3elt_dual.icoord  
```
