#!/bin/bash

python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/3elt_dual.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/3elt_dual.forceatlas2_600.gml.txt >> /data/morahma/ResearchProjects/SMU/results/3elt_dual.forceatlas2_600.measures.txt

python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/3elt_dual.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/3elt_dual.forceatlas2_BH_600.gml.txt >> /data/morahma/ResearchProjects/SMU/results/3elt_dual.forceatlas2_BH_600.measures.txt

python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/grid2_dual.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/grid2_dual.forceatlas2_600.gml.txt >> /data/morahma/ResearchProjects/SMU/results/grid2_dual.forceatlas2_600.measures.txt

python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/grid2_dual.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/grid2_dual.forceatlas2_BH_600.gml.txt >> /data/morahma/ResearchProjects/SMU/results/grid2_dual.forceatlas2_BH_600.measures.txt

python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/power.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/power.forceatlas2_600.gml.txt >> /data/morahma/ResearchProjects/SMU/results/power.forceatlas2_600.measures.txt

python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/power.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/power.forceatlas2_BH_600.gml.txt >> /data/morahma/ResearchProjects/SMU/results/power.forceatlas2_BH_600.measures.txt

python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/sf_ba6000.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/sf_ba6000.forceatlas2_600.gml.txt >> /data/morahma/ResearchProjects/SMU/results/sf_ba6000.forceatlas2_600.measures.txt

python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/sf_ba6000.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/sf_ba6000.forceatlas2_BH_600.txt >> /data/morahma/ResearchProjects/SMU/results/sf_ba6000.forceatlas2_BH_600.measures.txt

##############openord

python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/3elt_dual.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/3elt_dual.openord_600.gml.txt >> /data/morahma/ResearchProjects/SMU/results/3elt_dual.openord_600.measures.txt

 
python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/grid2_dual.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/grid2_dual.openord_600.gml.txt >> /data/morahma/ResearchProjects/SMU/results/grid2_dual.openord_600.gml.measures.txt
 

python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/power.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/power.openord_600.gml.txt >> /data/morahma/ResearchProjects/SMU/results/power.openord_600.measures.txt


python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/sf_ba6000.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/sf_ba6000.openord_600.gml.txt >> /data/morahma/ResearchProjects/SMU/results/sf_ba6000.openord_600.measures.txt

###############SMU

python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/3elt_dual.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/3elt_dual.mtxCACHEMINB128PARAOUTRAND600.txt >> /data/morahma/ResearchProjects/SMU/results/3elt_dual.mtxCACHEMINB128PARAOUTRAND600.measures.txt


python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/grid2_dual.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/grid2_dual.mtxCACHEMINB128PARAOUTRAND600.txt >> /data/morahma/ResearchProjects/SMU/results/grid2_dual.mtxCACHEMINB128PARAOUTRAND600.measures.txt


python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/power.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/power.mtxCACHEMINB128PARAOUTRAND600.txt >> /data/morahma/ResearchProjects/SMU/results/power.mtxCACHEMINB128PARAOUTRAND600.measures.txt


python -u /data/morahma/ResearchProjects/SMU/metrics/metrics.py -f /data/morahma/ResearchProjects/SMU/datasets/input/sf_ba6000.mtx -l /data/morahma/ResearchProjects/SMU/datasets/output/sf_ba6000.mtxCACHEMINB128PARAOUTRAND600.txt >> /data/morahma/ResearchProjects/SMU/results/sf_ba6000.mtxCACHEMINB128PARAOUTRAND600.measures.txt

