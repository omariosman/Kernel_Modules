cmd_/root/mgnm/Kernel_Modules/Read.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T /usr/src/linux-headers-4.19.0-13-common/scripts/module-common.lds  --build-id  -o /root/mgnm/Kernel_Modules/Read.ko /root/mgnm/Kernel_Modules/Read.o /root/mgnm/Kernel_Modules/Read.mod.o ;  true
