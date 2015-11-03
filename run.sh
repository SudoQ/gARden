# Sandbox run script with preset settings
PROJDIR="/home/sandbox/src/SARndbox"
VRUICFG="$PROJDIR/etc/SARndbox-1.5/Vrui.cfg"
RAIN_STR="0.0"
EVR="-0.01"
HYDR_RANGE="0.24"
HYDR_DT="0.001"
HYDR_VELOCITY="0.005"
HYDR_SS="4.0"
VEG_RANGE="0.2 0.8"
BWL="-3.0"
OPTS="-fpv -rs $RAIN_STR -evr $EVR -bwl $BWL -hr $HYDR_RANGE -dt $HYDR_DT -hv $HYDR_VELOCITY -hss $HYDR_SS -vr $VEG_RANGE"
$PROJDIR/bin/SARndbox -loadInputGraph $PROJDIR/inputgraphs/Sandbox1.inputgraph -mergeConfig $VRUICFG $OPTS
