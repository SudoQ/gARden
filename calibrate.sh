# Simple script to ease the calibration process
# The capture tool is pre-bounded to keys 1 and 2
PROJDIR="/home/sandbox/src/SARndbox"
VRUICFG="$PROJDIR/etc/SARndbox-1.5/Vrui.cfg"
OPTS="-tp 8 6" # More calibration points for greater accuracy
$PROJDIR/bin/CalibrateProjector -loadInputGraph $PROJDIR/inputgraphs/Calibration1.inputgraph -mergeConfig $VRUICFG $OPTS
