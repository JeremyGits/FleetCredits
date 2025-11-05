#!/bin/bash
cd /mnt/d/seriousprojects/FleetCredits/fleetcredits/src/qt
if [ ! -f fleetcredits.moc ]; then
    cp dogecoin.moc fleetcredits.moc
    sed -i 's/dogecoin.cpp/fleetcredits.cpp/g' fleetcredits.moc
    echo 'Created fleetcredits.moc'
fi
