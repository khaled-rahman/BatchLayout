java -jar othertools/GephiLayouts-1.0.jar forceatlas2 -i datasets/input/3elt_dual.gml -o datasets/output/3elt_dual.forceatlas2_600.gml -threads 32 -maxiters 600 >> results/3elt_dual.forceatlas2_600.txt
java -jar othertools/GephiLayouts-1.0.jar forceatlas2 -i datasets/input/3elt_dual.gml -o datasets/output/3elt_dual.forceatlas2_BH_600.gml -threads 1 -maxiters 600 -barneshut true >> results/3elt_dual.forceatlas2_BH_600.txt

java -jar othertools/GephiLayouts-1.0.jar forceatlas2 -i datasets/input/grid2_dual.gml -o datasets/output/grid2_dual.forceatlas2_600.gml -threads 32 -maxiters 600 >> results/grid2_dual.forceatlas2_600.txt
java -jar othertools/GephiLayouts-1.0.jar forceatlas2 -i datasets/input/grid2_dual.gml -o datasets/output/grid2_dual.forceatlas2_BH_600.gml -threads 1 -maxiters 600 -barneshut true >> results/grid2_dual.forceatlas2_BH_600.txt

java -jar othertools/GephiLayouts-1.0.jar forceatlas2 -i datasets/input/power.gml -o datasets/output/power.forceatlas2_600.gml -threads 32 -maxiters 600 >> results/power.forceatlas2_600.txt
java -jar othertools/GephiLayouts-1.0.jar forceatlas2 -i datasets/input/power.gml -o datasets/output/power.forceatlas2_BH_600.gml -threads 1 -maxiters 1  -barneshut true >> results/power.forceatlas2_BH_600.txt

java -jar othertools/GephiLayouts-1.0.jar forceatlas2 -i datasets/input/sf_ba6000.gml -o datasets/output/sf_ba6000.forceatlas2_600.gml -threads 32 -maxiters 600 >> results/sf_ba6000.forceatlas2_600.txt
java -jar othertools/GephiLayouts-1.0.jar forceatlas2 -i datasets/input/sf_ba6000.gml -o datasets/output/sf_ba6000.forceatlas2_BH_600.gml -threads 1 -maxiters 600  -barneshut true >> results/sf_ba6000.forceatlas2_BH_600.txt

java -jar othertools/GephiLayouts-1.0.jar openord -i datasets/input/3elt_dual.gml -o datasets/output/3elt_dual.openord_600.gml -threads 32 -maxiters 600 >> results/3elt_dual.openord_600.txt
java -jar othertools/GephiLayouts-1.0.jar openord -i datasets/input/grid2_dual.gml -o datasets/output/grid2_dual.openord_600.gml -threads 32 -maxiters 600 >> results/grid2_dual.forceatlas2_600.txt
java -jar othertools/GephiLayouts-1.0.jar openord -i datasets/input/power.gml -o datasets/output/power.openord_600.gml -threads 32 -maxiters 600 >> results/power.openord_600.txt
java -jar othertools/GephiLayouts-1.0.jar openord -i datasets/input/sf_ba6000.gml -o datasets/output/sf_ba6000.openord_600.gml -threads 32 -maxiters 600 >> results/sf_ba6000.openord_600.txt

./SMU datasets/input/3elt_dual.mtx 32 600 >> results/3elt_dual.mtx.SMU.txt
./SMU datasets/input/grid2_dual.mtx 32 600 >> results/grid2_dual.mtx.SMU.txt
./SMU datasets/input/power.mtx 32 600 >> results/power.mtx.SMU.txt
./SMU datasets/input/sf_ba6000.mtx 32 600 >> results/sf_ba6000.mtx.SMU.txt 
