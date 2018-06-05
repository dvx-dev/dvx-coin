## Dependencies

```
sudo add-apt-repository ppa:bitcoin/bitcoin && \
sudo apt-get update && \
sudo apt-get install libqt4-dev libminiupnpc-dev libssl-dev libboost-all-dev libdb4.8-dev libdb4.8++-dev \
 libgtk2.0-dev libqrencode-dev g++-multilib libc6-dev-i386 p7zip-full autoconf automake autopoint bash bison \
 bzip2 cmake flex gettext git g++ gperf intltool libffi-dev libtool libltdl-dev libssl-dev libxml-parser-perl \
 make openssl patch perl pkg-config python ruby scons sed unzip wget xz-utils autoconf automake autopoint bash \
 bison bzip2 flex gettext  git g++ gperf intltool libffi-dev libgdk-pixbuf2.0-dev libtool libltdl-dev libssl-dev \
 libxml-parser-perl make openssl p7zip-full patch perl pkg-config python ruby scons sed unzip wget xz-utils g++-multilib \
 libc6-dev-i386 libtool-bin clang clang-3.8 libqt4-dev libminiupnpc-dev libdb4.8-dev libdb4.8++-dev
```

git clone von mxe unter /opt/ ausfuehren und Kompilieren lassen

siehe: http://mxe.cc/#tutorial


## Kompilieren

```
PATH=/opt/mxe/usr/bin:$PATH
```

Erst unter der ```src/leveldb/``` die ```build_win.sh``` ausfuehren

Danach in ```src/```

```make -j PROZESSOR_ANZAHL -f makefile.linux-mingw```

Und anschliessend noch im main Ordner ```./compile.sh``` ausfuehren


