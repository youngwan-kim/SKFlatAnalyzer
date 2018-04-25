#!/bin/bash
for cy in SKFlatValidation
do
  root -l -b -q src/$cy.C+
  mv src/$cy"_C_ACLiC_dict_rdict.pcm" $SKFlat_LIB_PATH/
  mv src/$cy"_C.d" $SKFlat_LIB_PATH/
  mv src/$cy"_C.so" $SKFlat_LIB_PATH/
done
