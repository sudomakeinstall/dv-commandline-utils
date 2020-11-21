#!/usr/bin/env bash

readonly THIRD_PARTY_DIR=${HOME}/Developer/thirdparty
mkdir -p ${THIRD_PARTY_DIR}
cd ${THIRD_PARTY_DIR}

## VTK

rm -rf vtk/
mkdir -p vtk/bin
cd vtk
git clone git@github.com:Kitware/vtk.git
cd src
git remote rename origin upstream
git remote add origin git@github.com:dvigneault/vtk.git
cd ../bin
cmake ../src \
  -DBUILD_SHARED_LIBS=OFF \
  -DVTK_BUILD_EXAMPLES=OFF \
  -DVTK_BUILD_TESTING=OFF \
  -DVTK_GROUP_ENABLE_Qt=YES
cd ${THIRD_PARTY_DIR}

## ITK

rm -rf itk/
mkdir -p itk/bin
cd itk
git clone git@github.com:InsightSoftwareConsortium/itk.git
cd src
git remote rename origin upstream
git remote add origin git@github.com:dvigneault/itkcuberille.git
cd ../bin
cmake ../src \
  -DBUILD_EXAMPLES=OFF \
  -DBUILD_TESTING=ON \
  -DModule_ITKReview=ON \
  -DModule_ITKVtkGlue=ON
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
  git remote add origin git@github.com:dvigneault/itkcuberille.git
  cd ../bin
  ITK_DIR=${HOME}/Developer/thirdparty/itk/bin cmake ../src
  make -j$(nproc)
  cd ${THIRD_PARTY_DIR}

done
