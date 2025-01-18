gcc -fsanitize=address Build.c buildsystem/BIOSBootloader.c -g

./a.out build i686 Debug BIOS
./a.out run i686 Debug BIOS