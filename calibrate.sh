PROJDIR="/home/sandbox/src/SARndbox"
VRUICFG="$PROJDIR/etc/SARndbox-1.5/Vrui.cfg"
OPTS="-tp 8 6"
$PROJDIR/bin/CalibrateProjector -loadInputGraph $PROJDIR/Calibration1.inputgraph -mergeConfig $VRUICFG $OPTS
