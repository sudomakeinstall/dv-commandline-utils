#!/usr/bin/env bash

readonly THIRD_PARTY_DIR=${HOME}/Developer/thirdparty
mkdir -p ${THIRD_PARTY_DIR}
cd ${THIRD_PARTY_DIR}

## CGAL

rm -rf cgal/
mkdir -p cgal/bin
cd cgal
git clone git@github.com:cgal/cgal.git src
cd bin
cmake ../src \
  -DCMAKE_BUILD_TYPE="RelWithDebInfo"
cmake --build .
cd ${THIRD_PARTY_DIR}

## VTK

rm -rf vtk/
mkdir -p vtk/bin
cd vtk
git clone git@github.com:Kitware/vtk.git src
cd bin
cmake ../src \
  -DCMAKE_BUILD_TYPE="RelWithDebInfo" \
  -DBUILD_SHARED_LIBS=OFF \
  -DVTK_BUILD_EXAMPLES=OFF \
  -DVTK_BUILD_TESTING=OFF \
  -DVTK_GROUP_ENABLE_Qt=YES
cmake --build .
cd ${THIRD_PARTY_DIR}

## ITK

rm -rf itk/
mkdir -p itk/bin
cd itk
git clone git@github.com:InsightSoftwareConsortium/itk.git src
cd src
git remote rename origin upstream
git remote add origin git@github.com:dvigneault/itkcuberille.git
cd ../bin
cmake ../src \
  -DCMAKE_BUILD_TYPE="RelWithDebInfo" \
  -DBUILD_EXAMPLES=OFF \
  -DBUILD_TESTING=OFF \
  -DModule_ITKReview=ON \
  -DModule_ITKVtkGlue=ON
cmake --build .
cd ${THIRD_PARTY_DIR}

# ITK remotes

REMOTES=("cuberille"
         "genericlabelinterpolator"
         "iomeshstl"
         "meshnoise"
         "subdivisionquadedgemeshfilter"
        )

for REMOTE in ${REMOTES[*]};
do

  rm -rf itk${REMOTE}/
  mkdir -p itk${REMOTE}/bin
  cd itk${REMOTE}
  git clone git@github.com:InsightSoftwareConsortium/itk${REMOTE}.git src
  cd src
  git remote rename origin upstream
  git remote add origin git@github.com:dvigneault/itk${REMOTE}.git
  cd ../bin
  cmake ../src \
    -DCMAKE_BUILD_TYPE="RelWithDebInfo" \
    -DITK_DIR=${HOME}/Developer/thirdparty/itk/bin
  cmake --build .
  cd ${THIRD_PARTY_DIR}

done

## Checkout specific remote

cd itkcuberille/src
git fetch origin RemoveUnwantedPixels
git checkout origin/RemoveUnwantedPixels
