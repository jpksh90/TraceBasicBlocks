echo "WARNING: ALWAYS INVOKE THIS SCRIPT FROM PROJECT ROOT"
PROJ_ROOT=$(pwd)
LLVM_BIN_DIR="$LLVM_HOME/build/bin"
TRACER=$PROJ_ROOT/build/tracer/libTracer.so
$LLVM_BIN_DIR/opt -load $TRACER -tracer -time-passes -stats -S $@
