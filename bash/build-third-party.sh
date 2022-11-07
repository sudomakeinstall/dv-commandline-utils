#!/usr/bin/env bash

# Enable debugging (print out commands before executing)
set -x

# Disallow unset variables
set -u

# Exit on any non-zero condition
set -e

readonly THIRD_PARTY_DIR=${HOME}/Developer/thirdparty
#mkdir -p ${THIRD_PARTY_DIR}
#
### VTK
#
#rm -rf ${THIRD_PARTY_DIR}/vtk/
#mkdir -p ${THIRD_PARTY_DIR}/vtk/bin
#mkdir -p ${THIRD_PARTY_DIR}/vtk/src
#git clone --depth 1 git@github.com:Kitware/vtk.git ${THIRD_PARTY_DIR}/vtk/src
#cd ${THIRD_PARTY_DIR}/vtk/bin
#cmake ../src \
#  -DCMAKE_BUILD_TYPE="RelWithDebInfo" \
#  -DBUILD_SHARED_LIBS=OFF \
#  -DVTK_BUILD_EXAMPLES=OFF \
#  -DVTK_BUILD_TESTING=OFF
#cmake --build .
#
### ITK
#
#rm -rf ${THIRD_PARTY_DIR}/itk/
#mkdir -p ${THIRD_PARTY_DIR}/itk/bin
#mkdir -p ${THIRD_PARTY_DIR}/itk/src
#git clone --depth 1 git@github.com:InsightSoftwareConsortium/itk.git ${THIRD_PARTY_DIR}/itk/src
#cd ${THIRD_PARTY_DIR}/itk/src
#git remote rename origin upstream
#git remote add origin git@github.com:dvigneault/itk.git
#cd ${THIRD_PARTY_DIR}/itk/bin
#cmake ../src \
#  -DCMAKE_BUILD_TYPE="RelWithDebInfo" \
#  -DBUILD_EXAMPLES=OFF \
#  -DBUILD_TESTING=OFF \
#  -DModule_ITKReview=ON \
#  -DModule_ITKVtkGlue=ON
#cmake --build .
#
## ITK remotes
#
#REMOTES=("cuberille"
#         "genericlabelinterpolator"
#         "iomeshstl"
#         "meshnoise"
#         "subdivisionquadedgemeshfilter"
#        )
#
#for REMOTE in ${REMOTES[*]};
#do
#
#  rm -rf ${THIRD_PARTY_DIR}/itk${REMOTE}/
#  mkdir -p ${THIRD_PARTY_DIR}/itk${REMOTE}/bin
#  mkdir -p ${THIRD_PARTY_DIR}/itk${REMOTE}/src
#  git clone --depth 1 git@github.com:InsightSoftwareConsortium/itk${REMOTE}.git ${THIRD_PARTY_DIR}/itk${REMOTE}/src
#  cd ${THIRD_PARTY_DIR}/itk${REMOTE}/src
#  git remote rename origin upstream
#  git remote add origin git@github.com:dvigneault/itk${REMOTE}.git
#  cd ${THIRD_PARTY_DIR}/itk${REMOTE}/bin
#  cmake ../src \
#    -DCMAKE_BUILD_TYPE="RelWithDebInfo" \
#    -DBUILD_EXAMPLES=OFF \
#    -DBUILD_TESTING=OFF \
#    -DITK_DIR=${THIRD_PARTY_DIR}/itk/bin
#  cmake --build .
#
#done

## Checkout specific remote

cd ${THIRD_PARTY_DIR}/itkcuberille/src
git fetch origin RemoveUnwantedPixels
git checkout origin/RemoveUnwantedPixels
cd ${THIRD_PARTY_DIR}/itkcuberille/bin
cmake --build .
