#!/usr/bin/env bash

PROG_PATH=${1}
ROOT=$(dirname $(realpath $0))
eval "${PROG_PATH}"
