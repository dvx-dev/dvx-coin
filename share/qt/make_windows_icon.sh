#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/DVXCoin.ico

convert ../../src/qt/res/icons/DVXCoin-16.png ../../src/qt/res/icons/DVXCoin-32.png ../../src/qt/res/icons/DVXCoin-48.png ${ICON_DST}
