#!/usr/bin/env bash

set -e

echo "::group::e-antic"

source $(dirname "$0")/common.sh

if [ "$GMP_INSTALLDIR" != "" ]; then
    export CPPFLAGS="${CPPFLAGS} -I${GMP_INSTALLDIR}/include"
    export LDFLAGS="${LDFLAGS} -L${GMP_INSTALLDIR}/lib"
fi

## script for the installation of e-antic for the use in libnormaliz
E_ANTIC_VERSION=1.0.0-rc.15
E_ANTIC_URL="https://github.com/flatsurf/e-antic/releases/download/${E_ANTIC_VERSION}/e-antic-${E_ANTIC_VERSION}.tar.gz"
E_ANTIC_SHA256=429a8e95d2a9b4b10249f905102531af08caba1f9c054270cbe68d5bcfb48376

CONFIGURE_FLAGS="--prefix=${PREFIX}"

CONFIGURE_FLAGS="--prefix=${PREFIX} --without-benchmark --disable-silent-rules"

echo "Installing E-ANTIC..."

mkdir -p ${NMZ_OPT_DIR}/E-ANTIC_source/
cd ${NMZ_OPT_DIR}/E-ANTIC_source

../../download.sh ${E_ANTIC_URL} ${E_ANTIC_SHA256}
if [ ! -d e-antic-${E_ANTIC_VERSION} ]; then
    tar -xvf e-antic-${E_ANTIC_VERSION}.tar.gz
fi

cd e-antic-${E_ANTIC_VERSION}/libeantic

sed -i -e s/fmpq_poly_add_fmpq/fmpq_poly_add_fmpq_eantic/g upstream/patched/fmpq_poly_add_fmpq.c 
sed -i -e s/fmpq_poly_add_fmpq/fmpq_poly_add_fmpq_eantic/g upstream/patched/nf_elem_add_fmpq.c

if [ ! -f config.status ]; then
    ./configure ${CONFIGURE_FLAGS} --without-byexample --without-doc --without-benchmark --without-pyeantic
fi
make -j4
make install
