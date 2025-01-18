gcc -fsanitize=address Build.c buildsystem/BIOSBootloader.c -g

./a.out build i686 Release BIOS
./a.out run i686 Release BIOS