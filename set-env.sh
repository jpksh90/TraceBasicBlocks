#Warning: This script has to be invoked from the prject root

echo "WARNING: ALWAYS INVOKE THIS SCRIPT FROM PROJECT ROOT"
PROJ_ROOT=$(pwd)
LLVM_BIN_DIR="$PROJ_ROOT/llvm/build/bin"
alias opt=$LLVM_BIN_DIR/opt
alias llc=$LLVM_BIN_DIR/llc
alias llvm-diff=$LLVM_BIN_DIR/llvm-diff
alias clang++=$LLVM_BIN_DIR/clang++
alias clang="$LLVM_BIN_DIR/clang -isysroot \"/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk\" -I/usr/local/include"
alias llvm-dis=$LLVM_BIN_DIR/llvm-dis
which opt
which llc
which llvm-diff
which clang++
which clang
which llvm-dis

