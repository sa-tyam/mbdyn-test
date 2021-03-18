# mbdyn-test
a constitutive law module


I created a module named module-gsoc-satyam. 

Constitutive law  :   
elastic modulus (E) = 2 x rigidity modulus (G) x ( 1 + poisson's ratio (Mu) )

is used in this module.

This module can be used in cases where the rigidity modulus and poisson's ratio of object is known, but the stiffness or elastic modulus is unknown.
Currently it is developed just for 1D objects like rods.

In order to complete this module, I took references from the modules module-constlaw, module-contcontact, module-hunt-crossley and module-muscles.

A sample input file is provided in the folder. We can change the data and see the result changing.
I have tested the input files for four cases, changing the values of rigidity modulud and poisson's ration in each case. The output change can be seen in the plot images available in the folder. In some test cases very large values have been provided as input thus generating large value as fource. a commom value is provided in 'testfour' and it gives us reasonable results.


