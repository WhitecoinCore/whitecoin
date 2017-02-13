#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/whitecoin.ico

convert ../../src/qt/res/icons/whitecoin-16.png ../../src/qt/res/icons/whitecoin-32.png ../../src/qt/res/icons/whitecoin-48.png ${ICON_DST}
