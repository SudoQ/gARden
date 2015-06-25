PROJDIR="/home/sandbox/src/SARndbox"
VRUICFG="$PROJDIR/etc/SARndbox-1.5/Vrui.cfg"
SBOPT="-rs 0.0 -evr -0.01 -fpv"
$PROJDIR/bin/SARndbox -loadInputGraph $PROJDIR/SavedInputGraph0001.inputgraph -mergeConfig $VRUICFG $SBOPT
